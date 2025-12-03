/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

extern "C" {

#include "esp_log.h"
#include "esp_pm.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "nvs_flash.h"
#include "bsp/esp-bsp.h"
#include "settings.h"
#include "rgb_matrix.h"
#include "esp_timer.h"
#include "tinyusb_hid.h"
#include "ble_hid.h"
#include "dual_button.h"
#include "iot_button.h"
#include "tinyusb_cdc.h"
#include "adc_detect.h"
#include "driver/gpio.h"
#include "chain_bus.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_server.h"
#include "lwip/inet.h"
#include "string.h"
#include "esp_mac.h"
#include "cJSON.h"
#include "esp_crc.h"

// Web文件嵌入
extern "C" {
extern const char index_html_start[] asm("_binary_index_html_start");
extern const char index_html_end[] asm("_binary_index_html_end");
extern const char styles_css_start[] asm("_binary_styles_css_start");
extern const char styles_css_end[] asm("_binary_styles_css_end");
extern const char script_js_start[] asm("_binary_script_js_start");
extern const char script_js_end[] asm("_binary_script_js_end");
extern const char favicon_ico_start[] asm("_binary_favicon_ico_start");
extern const char favicon_ico_end[] asm("_binary_favicon_ico_end");
extern const char chain_angle_jpg_start[] asm("_binary_Chain_Angle_jpg_start");
extern const char chain_angle_jpg_end[] asm("_binary_Chain_Angle_jpg_end");
extern const char chain_blank_jpg_start[] asm("_binary_Chain_Blank_jpg_start");
extern const char chain_blank_jpg_end[] asm("_binary_Chain_Blank_jpg_end");
extern const char chain_dualKey_png_start[] asm("_binary_Chain_DualKey_png_start");
extern const char chain_dualKey_png_end[] asm("_binary_Chain_DualKey_png_end");
extern const char chain_encoder_jpg_start[] asm("_binary_Chain_Encoder_jpg_start");
extern const char chain_encoder_jpg_end[] asm("_binary_Chain_Encoder_jpg_end");
extern const char chain_joystick_jpg_start[] asm("_binary_Chain_Joystick_jpg_start");
extern const char chain_joystick_jpg_end[] asm("_binary_Chain_Joystick_jpg_end");
extern const char chain_key_jpg_start[] asm("_binary_Chain_Key_jpg_start");
extern const char chain_key_jpg_end[] asm("_binary_Chain_Key_jpg_end");
extern const char chain_mount_jpg_start[] asm("_binary_Chain_Mount_jpg_start");
extern const char chain_mount_jpg_end[] asm("_binary_Chain_Mount_jpg_end");
extern const char chain_tof_jpg_start[] asm("_binary_Chain_ToF_jpg_start");
extern const char chain_tof_jpg_end[] asm("_binary_Chain_ToF_jpg_end");
}

static const char *TAG = "main";

// WiFi配置
#define AP_SSID       "DualKey_mac"
#define AP_PASSWORD   "12345678"
#define MAXIMUM_RETRY 5

// WiFi配置数据结构
typedef struct {
    char ssid[32];       // SSID
    char password[64];   // 密码
    bool use_static_ip;  // 是否使用静态IP
    char static_ip[16];  // 静态IP地址
    char netmask[16];    // 子网掩码
    char gateway[16];    // 网关
    uint32_t crc32;      // CRC校验
} wifi_config_saved_t;

static keyboard_btn_handle_t kbd_handle        = NULL;
static TaskHandle_t light_progress_task_handle = NULL;
static led_strip_handle_t led_strip            = NULL;
uint64_t last_time                             = 0;
sys_param_t *sys_param;

// WiFi相关变量
static int s_retry_num       = 0;
static httpd_handle_t server = NULL;
static bool wifi_connected   = false;
static bool wifi_ap_mode     = false;  // WiFi运行模式标志

// WebSocket相关变量
static int websocket_fd                   = -1;
static TaskHandle_t websocket_task_handle = NULL;
QueueHandle_t status_refresh_queue        = NULL;  // 状态刷新消息队列
static bool send_bus_all_data             = false;

int switch_pos             = 0;  // 0:center, 1:left, 2:right
bool g_usb_mapping_enabled = true;
bool g_ble_mapping_enabled = true;
uint8_t g_connect_status   = 0;      // 0:未连接, 1:连接中, 2:已连接
bool g_ble_adv_status      = false;  // 蓝牙广播状态: false=未广播, true=正在广播

#define enable_web_cache 1  // 1:启用Web缓存, 60分钟不变,ctrl+F5/cmd+shift+R刷新, 0:禁用Web缓存

// 状态刷新消息类型
typedef enum {
    STATUS_REFRESH_NORMAL,    // 正常刷新
    STATUS_REFRESH_IMMEDIATE  // 立即刷新
} status_refresh_type_t;

// DualKey配置数据结构
typedef struct {
    uint32_t left_key_color;   // 左键颜色
    uint32_t right_key_color;  // 右键颜色
    int current_key_mapping;   // 当前按键映射索引
    bool usb_mapping_enabled;  // USB映射开关
    bool ble_mapping_enabled;  // 蓝牙映射开关
    uint32_t crc32;            // CRC校验
} dualkey_saved_config_t;

// 状态数据结构
typedef struct {
    bool left_key_pressed;
    bool right_key_pressed;
    uint32_t left_key_color;
    uint32_t right_key_color;
    float usb_voltage;
    bool usb_connected;
    int usb_mode;        // 0:HID, 1:BLE, 2:CDC
    int dip_switch_pos;  // 0:center, 1:left, 2:right
    float battery_voltage;
    int charge_status;  // 0:未充电, 1:充电中, 2:充满
    int battery_percentage;
    int switch_1_value;  // 拨码开关1值
    int switch_2_value;  // 拨码开关2值
    // 蓝牙状态
    bool bluetooth_connected;
    char bluetooth_device_name[32];
    int bluetooth_pairing_status;  // 0:未配对, 1:配对中, 2:已配对
    bool bluetooth_adv_status;     // 蓝牙广播状态: false=未广播, true=正在广播
    // HID按键映射
    int current_key_mapping;  // 当前按键映射索引
    // WIFI状态
    char wifi_ssid[32];   // WiFi网络名称
    char wifi_ip[16];     // IP地址
    int wifi_rssi;        // 信号强度
    bool wifi_connected;  // 连接状态
} device_status_t;

device_status_t g_device_status = {.left_key_pressed   = false,
                                   .right_key_pressed  = false,
                                   .left_key_color     = 0x000000,  // 红色
                                   .right_key_color    = 0x000000,  // 绿色
                                   .usb_voltage        = 0.0f,
                                   .usb_connected      = true,
                                   .usb_mode           = 0,  // HID
                                   .dip_switch_pos     = 0,  // center
                                   .battery_voltage    = 3.7f,
                                   .charge_status      = 0,
                                   .battery_percentage = 75,
                                   .switch_1_value     = 0,
                                   .switch_2_value     = 0,
                                   // 蓝牙状态初始化
                                   .bluetooth_connected      = false,
                                   .bluetooth_device_name    = USB_HID_PRODUCT,
                                   .bluetooth_pairing_status = 0,      // 未配对
                                   .bluetooth_adv_status     = false,  // 未广播
                                   // HID按键映射初始化
                                   .current_key_mapping = 9,  // 默认为翻页模式
                                   // WIFI状态初始化
                                   .wifi_ssid      = "",
                                   .wifi_ip        = "",
                                   .wifi_rssi      = 0,
                                   .wifi_connected = false};

static void wifi_start_ap_mode(void);
static httpd_handle_t start_webserver(void);

// WebSocket函数声明
static esp_err_t websocket_handler(httpd_req_t *req);
static void websocket_send_status(void);
static void websocket_task(void *pvParameters);
static void update_device_status(void);

// RGB颜色控制函数声明
static void set_key_rgb_color(int key_index, uint32_t rgb_color);

// DualKey配置保存和加载函数声明
static esp_err_t dualkey_config_save(void);
static esp_err_t dualkey_config_load(void);
static uint32_t dualkey_config_calculate_crc(const dualkey_saved_config_t *config);

// WiFi配置保存和加载函数声明
static esp_err_t wifi_config_save(const wifi_config_saved_t *config);
static esp_err_t wifi_config_load(wifi_config_saved_t *config);
static esp_err_t wifi_config_reset(void);
static uint32_t wifi_config_calculate_crc(const wifi_config_saved_t *config);

// WiFi事件处理
static void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        wifi_connected = false;
        if (s_retry_num < MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            ESP_LOGI(TAG, "Failed to connect to WiFi, starting AP mode");
            // 停止STA模式
            esp_wifi_stop();
            // 切换到AP模式
            wifi_start_ap_mode();
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num    = 0;
        wifi_connected = true;
        wifi_ap_mode   = false;
        start_webserver();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d", MAC2STR(event->mac), event->aid);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d, reason=%d", MAC2STR(event->mac), event->aid, event->reason);
    }
}

// 启动AP模式
static void wifi_start_ap_mode(void)
{
    ESP_LOGI(TAG, "Starting AP mode");

    wifi_config_t wifi_config = {};
    uint8_t mac[6];
    esp_efuse_mac_get_default(mac);
    char ap_ssid[32] = {0};
    sprintf(ap_ssid, "DualKey_%02X%02X", mac[4], mac[5]);
    strcpy((char *)wifi_config.ap.ssid, ap_ssid);
    wifi_config.ap.ssid_len = strlen(ap_ssid);
    strcpy((char *)wifi_config.ap.password, AP_PASSWORD);
    wifi_config.ap.max_connection = 4;
    wifi_config.ap.authmode       = WIFI_AUTH_WPA_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_wifi_set_max_tx_power(40);  // 设置最大发射功率为40dBm

    wifi_ap_mode   = true;
    wifi_connected = false;

    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);

    char ip_addr[16];
    inet_ntoa_r(ip_info.ip.addr, ip_addr, 16);
    ESP_LOGI(TAG, "AP started with IP: %s", ip_addr);

    start_webserver();
}

// 初始化WiFi
static void wifi_init_sta(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));

    // 从flash加载WiFi配置
    wifi_config_saved_t saved_config;
    esp_err_t ret = wifi_config_load(&saved_config);

    if (ret == ESP_OK && strlen(saved_config.ssid) > 0) {
        // 有保存的配置，尝试连接
        ESP_LOGI(TAG, "Found saved WiFi config, SSID: %s", saved_config.ssid);

        // 配置静态IP（如果启用）
        if (saved_config.use_static_ip) {
            ESP_LOGI(TAG, "Using static IP: %s", saved_config.static_ip);
            esp_netif_dhcpc_stop(sta_netif);

            esp_netif_ip_info_t ip_info;
            memset(&ip_info, 0, sizeof(esp_netif_ip_info_t));
            ip_info.ip.addr      = esp_ip4addr_aton(saved_config.static_ip);
            ip_info.netmask.addr = esp_ip4addr_aton(saved_config.netmask);
            ip_info.gw.addr      = esp_ip4addr_aton(saved_config.gateway);

            esp_netif_set_ip_info(sta_netif, &ip_info);
        }

        wifi_config_t wifi_config = {};
        strcpy((char *)wifi_config.sta.ssid, saved_config.ssid);
        strcpy((char *)wifi_config.sta.password, saved_config.password);
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
        wifi_config.sta.pmf_cfg.capable    = true;
        wifi_config.sta.pmf_cfg.required   = false;

        wifi_ap_mode   = false;
        wifi_connected = false;

        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
        ESP_ERROR_CHECK(esp_wifi_start());

        ESP_LOGI(TAG, "wifi_init_sta finished, connecting to %s", saved_config.ssid);
    } else {
        // 没有保存的配置，直接启动AP模式
        ESP_LOGI(TAG, "No saved WiFi config found, starting AP mode");
        wifi_ap_mode   = false;
        wifi_connected = false;
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
        ESP_ERROR_CHECK(esp_wifi_start());
        wifi_start_ap_mode();
    }
}

// HTTP GET处理器 - 主页
static esp_err_t index_get_handler(httpd_req_t *req)
{
    const uint32_t index_len = index_html_end - index_html_start;
    ESP_LOGI(TAG, "Serve index.html");
    httpd_resp_set_type(req, "text/html; charset=utf-8");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
    // #if enable_web_cache
    //     httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
    // #else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    // #endif
    httpd_resp_send(req, index_html_start, index_len);
    return ESP_OK;
}

// HTTP GET处理器 - CSS样式
static esp_err_t styles_get_handler(httpd_req_t *req)
{
    const uint32_t styles_len = styles_css_end - styles_css_start;
    ESP_LOGI(TAG, "Serve styles.css");
    httpd_resp_set_type(req, "text/css; charset=utf-8");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
    // #if enable_web_cache
    //     httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
    // #else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    // #endif
    httpd_resp_send(req, styles_css_start, styles_len);
    return ESP_OK;
}

// HTTP GET处理器 - JavaScript脚本
static esp_err_t script_get_handler(httpd_req_t *req)
{
    const uint32_t script_len = script_js_end - script_js_start;
    ESP_LOGI(TAG, "Serve script.js");
    httpd_resp_set_type(req, "application/javascript; charset=utf-8");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
    // #if enable_web_cache
    //     httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
    // #else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    // #endif
    httpd_resp_send(req, script_js_start, script_len);
    return ESP_OK;
}

// HTTP 404错误处理器 - 重定向到主页
static esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    httpd_resp_set_status(req, "302 Temporary Redirect");
    httpd_resp_set_hdr(req, "Location", "/");
    httpd_resp_send(req, "Redirect to the main page", HTTPD_RESP_USE_STRLEN);
    ESP_LOGI(TAG, "Redirecting to index");
    return ESP_OK;
}

// HTTP GET处理器 - Favicon
static esp_err_t favicon_get_handler(httpd_req_t *req)
{
    const uint32_t favicon_len = favicon_ico_end - favicon_ico_start;
    ESP_LOGI(TAG, "Serve favicon.ico");
    httpd_resp_set_type(req, "image/x-icon");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, favicon_ico_start, favicon_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Angle图片
static esp_err_t chain_angle_get_handler(httpd_req_t *req)
{
    const uint32_t chain_angle_len = chain_angle_jpg_end - chain_angle_jpg_start;
    ESP_LOGI(TAG, "Serve chain_angle.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_angle_jpg_start, chain_angle_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Blank图片
static esp_err_t chain_blank_get_handler(httpd_req_t *req)
{
    const uint32_t chain_blank_len = chain_blank_jpg_end - chain_blank_jpg_start;
    ESP_LOGI(TAG, "Serve chain_blank.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_blank_jpg_start, chain_blank_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain DualKey图片
static esp_err_t chain_dualKey_get_handler(httpd_req_t *req)
{
    const uint32_t chain_dualKey_len = chain_dualKey_png_end - chain_dualKey_png_start;
    ESP_LOGI(TAG, "Serve Chain_DualKey.png");
    httpd_resp_set_type(req, "image/png");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_dualKey_png_start, chain_dualKey_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Encoder图片
static esp_err_t chain_encoder_get_handler(httpd_req_t *req)
{
    const uint32_t chain_encoder_len = chain_encoder_jpg_end - chain_encoder_jpg_start;
    ESP_LOGI(TAG, "Serve Chain_Encoder.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_encoder_jpg_start, chain_encoder_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Joystick图片
static esp_err_t chain_joystick_get_handler(httpd_req_t *req)
{
    const uint32_t chain_joystick_len = chain_joystick_jpg_end - chain_joystick_jpg_start;
    ESP_LOGI(TAG, "Serve Chain_Joystick.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_joystick_jpg_start, chain_joystick_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Key图片
static esp_err_t chain_key_get_handler(httpd_req_t *req)
{
    const uint32_t chain_key_len = chain_key_jpg_end - chain_key_jpg_start;
    ESP_LOGI(TAG, "Serve Chain_Key.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_key_jpg_start, chain_key_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain Mount图片
static esp_err_t chain_mount_get_handler(httpd_req_t *req)
{
    const uint32_t chain_mount_len = chain_mount_jpg_end - chain_mount_jpg_start;
    ESP_LOGI(TAG, "Serve Chain_Mount.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_mount_jpg_start, chain_mount_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain ToF图片
static esp_err_t chain_tof_get_handler(httpd_req_t *req)
{
    const uint32_t chain_tof_len = chain_tof_jpg_end - chain_tof_jpg_start;
    ESP_LOGI(TAG, "Serve Chain_ToF.jpg");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_set_hdr(req, "X-Content-Type-Options", "nosniff");
#if enable_web_cache
    httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600, immutable");
#else
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
#endif
    httpd_resp_send(req, chain_tof_jpg_start, chain_tof_len);
    return ESP_OK;
}

// HTTP GET处理器 - Chain

// WebSocket处理器
static esp_err_t websocket_handler(httpd_req_t *req)
{
    if (req->method == HTTP_GET) {
        ESP_LOGI(TAG, "WebSocket握手请求");
        return ESP_OK;
    }

    httpd_ws_frame_t ws_pkt;
    uint8_t *buf = NULL;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;

    // 接收WebSocket数据
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame len with %d", ret);
        return ret;
    }

    if (ws_pkt.len) {
        buf = (uint8_t *)malloc(ws_pkt.len + 1);
        if (buf == NULL) {
            ESP_LOGE(TAG, "Failed to allocate memory for buf");
            return ESP_ERR_NO_MEM;
        }
        ws_pkt.payload = buf;
        ret            = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "httpd_ws_recv_frame failed with %d", ret);
            free(buf);
            return ret;
        }
        buf[ws_pkt.len] = '\0';

        // 处理接收到的JSON数据
        cJSON *json = cJSON_Parse((char *)buf);
        if (json) {
            cJSON *type = cJSON_GetObjectItem(json, "type");
            if (type && cJSON_IsString(type)) {
                if (strcmp(type->valuestring, "set_rgb") == 0) {
                    // 设置RGB颜色
                    if (rgb_matrix_get_suspend_state() == false) {
                        rgb_matrix_set_suspend_state(true);
                        vTaskDelay(1 / portTICK_PERIOD_MS);
                    }
                    cJSON *key   = cJSON_GetObjectItem(json, "key");
                    cJSON *color = cJSON_GetObjectItem(json, "color");
                    if (key && color && cJSON_IsString(key) && cJSON_IsNumber(color)) {
                        uint32_t rgb_color = (uint32_t)color->valueint;
                        if (strcmp(key->valuestring, "left") == 0) {
                            g_device_status.left_key_color = rgb_color;
                            ESP_LOGI(TAG, "设置左键颜色: 0x%06lX", rgb_color);
                            set_key_rgb_color(1, rgb_color);  // 左键 LED index 1
                            // 自动保存配置
                            dualkey_config_save();
                        } else if (strcmp(key->valuestring, "right") == 0) {
                            g_device_status.right_key_color = rgb_color;
                            ESP_LOGI(TAG, "设置右键颜色: 0x%06lX", rgb_color);
                            set_key_rgb_color(0, rgb_color);  // 右键 LED index 0
                            // 自动保存配置
                            dualkey_config_save();
                        }
                    }
                } else if (strcmp(type->valuestring, "get_status") == 0) {
                    // 发送状态数据
                    websocket_fd = httpd_req_to_sockfd(req);
                    websocket_send_status();
                } else if (strcmp(type->valuestring, "enumerate_bus") == 0) {
                    // 枚举Chain Bus设备
                    cJSON *bus = cJSON_GetObjectItem(json, "bus");
                    if (bus && cJSON_IsString(bus)) {
                        int bus_idx = (strcmp(bus->valuestring, "left") == 0) ? 0 : 1;
                        ESP_LOGI(TAG, "枚举 %s Bus 设备", bus->valuestring);
                        // 触发Chain Bus扫描
                        chain_bus_scan_single(bus_idx);
                        send_bus_all_data = true;
                    }
                } else if (strcmp(type->valuestring, "set_chain_rgb") == 0) {
                    // 设置Chain设备RGB颜色
                    cJSON *bus       = cJSON_GetObjectItem(json, "bus");
                    cJSON *device_id = cJSON_GetObjectItem(json, "device_id");
                    cJSON *color     = cJSON_GetObjectItem(json, "color");

                    if (bus && device_id && color && cJSON_IsString(bus) && cJSON_IsNumber(device_id) &&
                        cJSON_IsNumber(color)) {
                        int bus_idx        = (strcmp(bus->valuestring, "left") == 0) ? 0 : 1;
                        uint8_t dev_id     = (uint8_t)device_id->valueint;
                        uint32_t rgb_color = (uint32_t)color->valueint;

                        ESP_LOGI(TAG, "设置 %s Bus 设备 %d RGB颜色: 0x%06lX", bus->valuestring, dev_id, rgb_color);

                        // 调用Chain Bus RGB设置函数
                        esp_err_t ret = chain_bus_set_device_rgb(bus_idx, dev_id, rgb_color);
                        if (ret != ESP_OK) {
                            ESP_LOGE(TAG, "设置Chain设备RGB失败: %s", esp_err_to_name(ret));
                        }
                    }
                } else if (strcmp(type->valuestring, "set_chain_bus_rgb") == 0) {
                    // 设置Chain Bus RGB颜色
                    cJSON *bus   = cJSON_GetObjectItem(json, "bus");
                    cJSON *color = cJSON_GetObjectItem(json, "color");
                    if (bus && color && cJSON_IsString(bus) && cJSON_IsNumber(color)) {
                        int bus_idx        = (strcmp(bus->valuestring, "left") == 0) ? 0 : 1;
                        uint32_t rgb_color = (uint32_t)color->valueint;
                        for (int i = 0; i < bus_status[bus_idx].device_count; i++) {
                            if (bus_status[bus_idx].device_status[i].connected) {
                                chain_bus_set_device_rgb(bus_idx, bus_status[bus_idx].device_status[i].id, rgb_color);
                            }
                        }
                    }
                } else if (strcmp(type->valuestring, "set_hid_mapping") == 0) {
                    // 设置HID按键映射
                    cJSON *mapping_index = cJSON_GetObjectItem(json, "mapping_index");
                    if (mapping_index && cJSON_IsNumber(mapping_index)) {
                        // 更新全局状态
                        g_device_status.current_key_mapping = mapping_index->valueint;
                        // 调用按键映射设置函数
                        btn_progress_set_key_mapping(mapping_index->valueint);
                        ESP_LOGI(TAG, "设置HID按键映射为: %d", mapping_index->valueint);
                        // 自动保存配置
                        dualkey_config_save();
                    }
                } else if (strcmp(type->valuestring, "set_key_mapping_switch") == 0) {
                    // 设置按键映射开关
                    cJSON *mapping_type = cJSON_GetObjectItem(json, "mapping_type");
                    cJSON *enabled      = cJSON_GetObjectItem(json, "enabled");
                    if (mapping_type && enabled && cJSON_IsString(mapping_type) && cJSON_IsBool(enabled)) {
                        if (strcmp(mapping_type->valuestring, "usb") == 0) {
                            g_usb_mapping_enabled = cJSON_IsTrue(enabled);
                            ESP_LOGI(TAG, "设置USB映射开关为: %s", g_usb_mapping_enabled ? "启用" : "禁用");
                            // 自动保存配置
                            dualkey_config_save();
                        } else if (strcmp(mapping_type->valuestring, "ble") == 0) {
                            g_ble_mapping_enabled = cJSON_IsTrue(enabled);
                            ESP_LOGI(TAG, "设置蓝牙映射开关为: %s", g_ble_mapping_enabled ? "启用" : "禁用");
                            // 自动保存配置
                            dualkey_config_save();
                        }
                    }
                } else if (strcmp(type->valuestring, "set_device_hid_config") == 0) {
                    // 设置Chain设备HID配置
                    cJSON *bus       = cJSON_GetObjectItem(json, "bus");
                    cJSON *device_id = cJSON_GetObjectItem(json, "device_id");
                    cJSON *config    = cJSON_GetObjectItem(json, "config");

                    if (bus && device_id && config && cJSON_IsString(bus) && cJSON_IsNumber(device_id) &&
                        cJSON_IsObject(config)) {
                        int bus_idx    = (strcmp(bus->valuestring, "left") == 0) ? 0 : 1;
                        uint8_t dev_id = (uint8_t)device_id->valueint;

                        ESP_LOGI(TAG, "设置 %s Bus 设备 %d HID配置", bus->valuestring, dev_id);

                        // 查找设备
                        int device_index = -1;
                        esp_err_t ret    = chain_bus_find_device_index(bus_idx, dev_id, &device_index);
                        if (ret == ESP_OK && device_index >= 0) {
                            chain_device_status_t *dev_status    = &bus_status[bus_idx].device_status[device_index];
                            chain_device_hid_config_t hid_config = {};

                            // 根据设备类型解析配置
                            switch (dev_status->type) {
                                case CHAIN_KEY_TYPE_CODE: {
                                    cJSON *single_click  = cJSON_GetObjectItem(config, "single_click");
                                    cJSON *double_click  = cJSON_GetObjectItem(config, "double_click");
                                    cJSON *long_press    = cJSON_GetObjectItem(config, "long_press");
                                    cJSON *press_down    = cJSON_GetObjectItem(config, "press_down");
                                    cJSON *press_release = cJSON_GetObjectItem(config, "press_release");

                                    if (single_click && cJSON_IsNumber(single_click))
                                        hid_config.key_config.single_click = (hid_func_type_t)single_click->valueint;
                                    if (double_click && cJSON_IsNumber(double_click))
                                        hid_config.key_config.double_click = (hid_func_type_t)double_click->valueint;
                                    if (long_press && cJSON_IsNumber(long_press))
                                        hid_config.key_config.long_press = (hid_func_type_t)long_press->valueint;
                                    if (press_down && cJSON_IsNumber(press_down))
                                        hid_config.key_config.press_down = (hid_func_type_t)press_down->valueint;
                                    if (press_release && cJSON_IsNumber(press_release))
                                        hid_config.key_config.press_release = (hid_func_type_t)press_release->valueint;

                                    chain_bus_set_device_hid_config(bus_idx, dev_id, &hid_config);
                                    break;
                                }
                                case CHAIN_JOYSTICK_TYPE_CODE: {
                                    cJSON *single_click    = cJSON_GetObjectItem(config, "single_click");
                                    cJSON *double_click    = cJSON_GetObjectItem(config, "double_click");
                                    cJSON *long_press      = cJSON_GetObjectItem(config, "long_press");
                                    cJSON *press_down      = cJSON_GetObjectItem(config, "press_down");
                                    cJSON *press_release   = cJSON_GetObjectItem(config, "press_release");
                                    cJSON *xy_move_func    = cJSON_GetObjectItem(config, "xy_move_func");
                                    cJSON *xy_move_reverse = cJSON_GetObjectItem(config, "xy_move_reverse");

                                    if (single_click && cJSON_IsNumber(single_click))
                                        hid_config.joystick_config.single_click =
                                            (hid_func_type_t)single_click->valueint;
                                    if (double_click && cJSON_IsNumber(double_click))
                                        hid_config.joystick_config.double_click =
                                            (hid_func_type_t)double_click->valueint;
                                    if (long_press && cJSON_IsNumber(long_press))
                                        hid_config.joystick_config.long_press = (hid_func_type_t)long_press->valueint;
                                    if (press_down && cJSON_IsNumber(press_down))
                                        hid_config.joystick_config.press_down = (hid_func_type_t)press_down->valueint;
                                    if (xy_move_reverse && cJSON_IsBool(xy_move_reverse))
                                        hid_config.joystick_config.xy_move_reverse = cJSON_IsTrue(xy_move_reverse);
                                    if (press_release && cJSON_IsNumber(press_release))
                                        hid_config.joystick_config.press_release =
                                            (hid_func_type_t)press_release->valueint;
                                    if (xy_move_func && cJSON_IsNumber(xy_move_func))
                                        hid_config.joystick_config.xy_move_func =
                                            (hid_func_type_t)xy_move_func->valueint;

                                    chain_bus_set_device_hid_config(bus_idx, dev_id, &hid_config);
                                    break;
                                }
                                case CHAIN_ENCODER_TYPE_CODE: {
                                    cJSON *single_click    = cJSON_GetObjectItem(config, "single_click");
                                    cJSON *double_click    = cJSON_GetObjectItem(config, "double_click");
                                    cJSON *long_press      = cJSON_GetObjectItem(config, "long_press");
                                    cJSON *press_down      = cJSON_GetObjectItem(config, "press_down");
                                    cJSON *press_release   = cJSON_GetObjectItem(config, "press_release");
                                    cJSON *rotate_cw_func  = cJSON_GetObjectItem(config, "rotate_cw_func");
                                    cJSON *rotate_ccw_func = cJSON_GetObjectItem(config, "rotate_ccw_func");

                                    if (single_click && cJSON_IsNumber(single_click))
                                        hid_config.encoder_config.single_click =
                                            (hid_func_type_t)single_click->valueint;
                                    if (double_click && cJSON_IsNumber(double_click))
                                        hid_config.encoder_config.double_click =
                                            (hid_func_type_t)double_click->valueint;
                                    if (long_press && cJSON_IsNumber(long_press))
                                        hid_config.encoder_config.long_press = (hid_func_type_t)long_press->valueint;
                                    if (press_down && cJSON_IsNumber(press_down))
                                        hid_config.encoder_config.press_down = (hid_func_type_t)press_down->valueint;
                                    if (press_release && cJSON_IsNumber(press_release))
                                        hid_config.encoder_config.press_release =
                                            (hid_func_type_t)press_release->valueint;
                                    if (rotate_cw_func && cJSON_IsNumber(rotate_cw_func))
                                        hid_config.encoder_config.rotate_cw_func =
                                            (hid_func_type_t)rotate_cw_func->valueint;
                                    if (rotate_ccw_func && cJSON_IsNumber(rotate_ccw_func))
                                        hid_config.encoder_config.rotate_ccw_func =
                                            (hid_func_type_t)rotate_ccw_func->valueint;

                                    chain_bus_set_device_hid_config(bus_idx, dev_id, &hid_config);
                                    break;
                                }
                                case CHAIN_ANGLE_TYPE_CODE: {
                                    cJSON *angle_func = cJSON_GetObjectItem(config, "angle_func");

                                    if (angle_func && cJSON_IsNumber(angle_func))
                                        hid_config.angle_config.angle_func = (hid_func_type_t)angle_func->valueint;

                                    chain_bus_set_device_hid_config(bus_idx, dev_id, &hid_config);
                                    break;
                                }
                                default:
                                    ESP_LOGW(TAG, "设备类型 %s 不支持HID配置",
                                             chain_device_type_name(dev_status->type));
                                    break;
                            }

                            // 保存设备配置到NVS
                            esp_err_t save_ret = chain_bus_save_device_config(bus_idx, dev_id);
                            if (save_ret == ESP_OK) {
                                ESP_LOGI(TAG, "设备 %s Bus ID:%d 配置已保存", bus->valuestring, dev_id);
                            } else {
                                ESP_LOGE(TAG, "设备 %s Bus ID:%d 配置保存失败: %s", bus->valuestring, dev_id,
                                         esp_err_to_name(save_ret));
                            }

                            // 标记状态更新，使配置变更立即发送到前端
                            dev_status->status_updated = true;

                            // 立即触发状态刷新，确保配置更新后前端能收到最新状态
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            if (status_refresh_queue != NULL) {
                                xQueueSend(status_refresh_queue, &refresh_msg, 0);
                            }
                        } else {
                            ESP_LOGE(TAG, "找不到设备 %s Bus ID: %d", bus->valuestring, dev_id);
                        }
                    }
                } else if (strcmp(type->valuestring, "reset_device_hid_config") == 0) {
                    // 恢复Chain设备HID配置为默认值
                    cJSON *bus       = cJSON_GetObjectItem(json, "bus");
                    cJSON *device_id = cJSON_GetObjectItem(json, "device_id");

                    if (bus && device_id && cJSON_IsString(bus) && cJSON_IsNumber(device_id)) {
                        int bus_idx    = (strcmp(bus->valuestring, "left") == 0) ? 0 : 1;
                        uint8_t dev_id = (uint8_t)device_id->valueint;

                        ESP_LOGI(TAG, "恢复 %s Bus 设备 %d HID配置为默认值", bus->valuestring, dev_id);

                        // 查找设备
                        int device_index = -1;
                        esp_err_t ret    = chain_bus_find_device_index(bus_idx, dev_id, &device_index);
                        if (ret == ESP_OK && device_index >= 0) {
                            chain_device_status_t *dev_status = &bus_status[bus_idx].device_status[device_index];

                            // 删除保存的配置
                            esp_err_t delete_ret = chain_bus_config_delete(dev_status->uid);
                            if (delete_ret == ESP_OK) {
                                ESP_LOGI(TAG, "设备 %s Bus ID:%d 保存的配置已删除", bus->valuestring, dev_id);
                            } else {
                                ESP_LOGW(TAG, "设备 %s Bus ID:%d 配置删除失败: %s", bus->valuestring, dev_id,
                                         esp_err_to_name(delete_ret));
                            }

                            // 重新初始化设备HID配置为默认值
                            chain_bus_init_device_hid_config(dev_status, bus_idx);

                            // 标记状态更新，使配置变更立即发送到前端
                            dev_status->status_updated = true;

                            // 立即触发状态刷新，确保配置更新后前端能收到最新状态
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            if (status_refresh_queue != NULL) {
                                xQueueSend(status_refresh_queue, &refresh_msg, 0);
                            }

                            ESP_LOGI(TAG, "设备 %s Bus ID:%d HID配置已恢复为默认值", bus->valuestring, dev_id);
                        } else {
                            ESP_LOGE(TAG, "找不到设备 %s Bus ID: %d", bus->valuestring, dev_id);
                        }
                    }
                } else if (strcmp(type->valuestring, "bluetooth_start_adv") == 0) {
                    // 开始蓝牙广播
                    ESP_LOGI(TAG, "启动蓝牙广播");
                    esp_err_t ret = ble_hid_adv_start();
                    if (ret == ESP_OK) {
                        g_ble_adv_status = true;
                        ESP_LOGI(TAG, "蓝牙广播已启动");
                    } else {
                        ESP_LOGE(TAG, "蓝牙广播启动失败: %s", esp_err_to_name(ret));
                    }

                } else if (strcmp(type->valuestring, "bluetooth_disconnect") == 0) {
                    // 断开蓝牙连接
                    ESP_LOGI(TAG, "断开蓝牙连接");
                    esp_err_t ret = ble_hid_adv_stop();
                    if (ret == ESP_OK) {
                        g_ble_adv_status = false;

                        ESP_LOGI(TAG, "蓝牙连接已断开");
                    } else {
                        ESP_LOGE(TAG, "断开蓝牙连接失败: %s", esp_err_to_name(ret));
                    }

                } else if (strcmp(type->valuestring, "bluetooth_start_pairing") == 0) {
                    // 开始蓝牙配对 TODO
                    ESP_LOGI(TAG, "开始蓝牙配对");

                } else if (strcmp(type->valuestring, "save_dualkey_config") == 0) {
                    // 保存DualKey配置
                    ESP_LOGI(TAG, "保存DualKey配置");
                    esp_err_t ret = dualkey_config_save();
                    if (ret == ESP_OK) {
                        ESP_LOGI(TAG, "DualKey配置保存成功");
                    } else {
                        ESP_LOGE(TAG, "DualKey配置保存失败: %s", esp_err_to_name(ret));
                    }

                } else if (strcmp(type->valuestring, "load_dualkey_config") == 0) {
                    // 加载DualKey配置
                    ESP_LOGI(TAG, "加载DualKey配置");
                    esp_err_t ret = dualkey_config_load();
                    if (ret == ESP_OK) {
                        ESP_LOGI(TAG, "DualKey配置加载成功");
                        // 立即发送状态更新
                        websocket_send_status();
                    } else if (ret == ESP_ERR_NOT_FOUND) {
                        ESP_LOGI(TAG, "DualKey配置未找到，使用默认配置");
                    } else {
                        ESP_LOGE(TAG, "DualKey配置加载失败: %s", esp_err_to_name(ret));
                    }

                } else if (strcmp(type->valuestring, "set_wifi_config") == 0) {
                    // 设置WiFi配置
                    cJSON *config = cJSON_GetObjectItem(json, "config");
                    if (config && cJSON_IsObject(config)) {
                        wifi_config_saved_t wifi_cfg;
                        memset(&wifi_cfg, 0, sizeof(wifi_cfg));

                        cJSON *ssid          = cJSON_GetObjectItem(config, "ssid");
                        cJSON *password      = cJSON_GetObjectItem(config, "password");
                        cJSON *use_static_ip = cJSON_GetObjectItem(config, "use_static_ip");

                        if (ssid && password && cJSON_IsString(ssid) && cJSON_IsString(password)) {
                            strncpy(wifi_cfg.ssid, ssid->valuestring, sizeof(wifi_cfg.ssid) - 1);
                            strncpy(wifi_cfg.password, password->valuestring, sizeof(wifi_cfg.password) - 1);
                            wifi_cfg.use_static_ip = (use_static_ip && cJSON_IsTrue(use_static_ip));

                            if (wifi_cfg.use_static_ip) {
                                cJSON *static_ip = cJSON_GetObjectItem(config, "static_ip");
                                cJSON *netmask   = cJSON_GetObjectItem(config, "netmask");
                                cJSON *gateway   = cJSON_GetObjectItem(config, "gateway");

                                if (static_ip && netmask && gateway && cJSON_IsString(static_ip) &&
                                    cJSON_IsString(netmask) && cJSON_IsString(gateway)) {
                                    strncpy(wifi_cfg.static_ip, static_ip->valuestring, sizeof(wifi_cfg.static_ip) - 1);
                                    strncpy(wifi_cfg.netmask, netmask->valuestring, sizeof(wifi_cfg.netmask) - 1);
                                    strncpy(wifi_cfg.gateway, gateway->valuestring, sizeof(wifi_cfg.gateway) - 1);
                                }
                            }

                            // 保存配置
                            esp_err_t save_ret = wifi_config_save(&wifi_cfg);
                            if (save_ret == ESP_OK) {
                                ESP_LOGI(TAG, "WiFi配置已保存，准备重启...");
                                // 延迟2秒后重启
                                vTaskDelay(pdMS_TO_TICKS(2000));
                                esp_restart();
                            } else {
                                ESP_LOGE(TAG, "WiFi配置保存失败: %s", esp_err_to_name(save_ret));
                            }
                        }
                    }
                } else if (strcmp(type->valuestring, "reset_wifi_config") == 0) {
                    // 重置WiFi配置
                    ESP_LOGI(TAG, "重置WiFi配置");
                    esp_err_t reset_ret = wifi_config_reset();
                    if (reset_ret == ESP_OK) {
                        ESP_LOGI(TAG, "WiFi配置已重置，准备重启...");
                        // 延迟2秒后重启
                        vTaskDelay(pdMS_TO_TICKS(2000));
                        esp_restart();
                    } else {
                        ESP_LOGE(TAG, "WiFi配置重置失败: %s", esp_err_to_name(reset_ret));
                    }
                }
            }
            cJSON_Delete(json);
        }

        free(buf);
    }

    websocket_fd = httpd_req_to_sockfd(req);
    return ESP_OK;
}

// 发送状态数据到WebSocket客户端
static void websocket_send_status(void)
{
    if (websocket_fd < 0 || server == NULL) {
        return;
    }

    cJSON *json      = cJSON_CreateObject();
    cJSON *dualkey   = cJSON_CreateObject();
    cJSON *chainbus  = cJSON_CreateObject();
    cJSON *left_bus  = cJSON_CreateObject();
    cJSON *right_bus = cJSON_CreateObject();

    // DualKey状态
    cJSON_AddBoolToObject(dualkey, "left_key_pressed", g_device_status.left_key_pressed);
    cJSON_AddBoolToObject(dualkey, "right_key_pressed", g_device_status.right_key_pressed);
    cJSON_AddNumberToObject(dualkey, "left_key_color", g_device_status.left_key_color);
    cJSON_AddNumberToObject(dualkey, "right_key_color", g_device_status.right_key_color);
    cJSON_AddNumberToObject(dualkey, "usb_voltage", g_device_status.usb_voltage);
    cJSON_AddBoolToObject(dualkey, "usb_connected", g_device_status.usb_connected);
    cJSON_AddNumberToObject(dualkey, "usb_mode", g_device_status.usb_mode);
    cJSON_AddNumberToObject(dualkey, "dip_switch_pos", g_device_status.dip_switch_pos);
    cJSON_AddNumberToObject(dualkey, "battery_voltage", g_device_status.battery_voltage);
    cJSON_AddNumberToObject(dualkey, "charge_status", g_device_status.charge_status);
    cJSON_AddNumberToObject(dualkey, "battery_percentage", g_device_status.battery_percentage);
    cJSON_AddNumberToObject(dualkey, "switch_1_value", g_device_status.switch_1_value);
    cJSON_AddNumberToObject(dualkey, "switch_2_value", g_device_status.switch_2_value);
    // 蓝牙状态
    cJSON_AddBoolToObject(dualkey, "bluetooth_connected", g_device_status.bluetooth_connected);
    cJSON_AddStringToObject(dualkey, "bluetooth_device_name", g_device_status.bluetooth_device_name);
    cJSON_AddNumberToObject(dualkey, "bluetooth_pairing_status", g_device_status.bluetooth_pairing_status);
    cJSON_AddBoolToObject(dualkey, "bluetooth_adv_status", g_device_status.bluetooth_adv_status);

    // HID按键映射状态
    cJSON_AddNumberToObject(dualkey, "current_key_mapping", g_device_status.current_key_mapping);
    cJSON_AddBoolToObject(dualkey, "usb_mapping_enabled", g_usb_mapping_enabled);
    cJSON_AddBoolToObject(dualkey, "ble_mapping_enabled", g_ble_mapping_enabled);

    // WIFI状态
    cJSON_AddStringToObject(dualkey, "wifi_ssid", g_device_status.wifi_ssid);
    cJSON_AddStringToObject(dualkey, "wifi_ip", g_device_status.wifi_ip);
    cJSON_AddNumberToObject(dualkey, "wifi_rssi", g_device_status.wifi_rssi);
    cJSON_AddBoolToObject(dualkey, "wifi_connected", g_device_status.wifi_connected);

    // Chain Bus状态 - 只发送有更新的信息
    bool left_connected  = bus_status[0].initialized && (bus_status[0].device_count > 0);
    bool right_connected = bus_status[1].initialized && (bus_status[1].device_count > 0);

    cJSON_AddBoolToObject(left_bus, "connected", left_connected);
    cJSON_AddNumberToObject(left_bus, "device_count", bus_status[0].device_count);
    cJSON_AddBoolToObject(right_bus, "connected", right_connected);
    cJSON_AddNumberToObject(right_bus, "device_count", bus_status[1].device_count);

    // 添加当前连接设备的ID列表（用于前端判断设备离线）
    cJSON *left_device_ids = cJSON_CreateArray();
    for (int i = 0; i < bus_status[0].device_count; i++) {
        if (bus_status[0].device_status[i].connected) {
            cJSON_AddItemToArray(left_device_ids, cJSON_CreateNumber(bus_status[0].device_status[i].id));
        }
    }
    cJSON_AddItemToObject(left_bus, "connected_device_ids", left_device_ids);

    cJSON *right_device_ids = cJSON_CreateArray();
    for (int i = 0; i < bus_status[1].device_count; i++) {
        if (bus_status[1].device_status[i].connected) {
            cJSON_AddItemToArray(right_device_ids, cJSON_CreateNumber(bus_status[1].device_status[i].id));
        }
    }
    cJSON_AddItemToObject(right_bus, "connected_device_ids", right_device_ids);

    // 添加设备信息 - 只发送有更新的设备
    cJSON *left_devices  = cJSON_CreateArray();
    cJSON *right_devices = cJSON_CreateArray();

    // 处理Bus设备（左Bus和右Bus）
    for (int bus_idx = 0; bus_idx < 2; bus_idx++) {
        cJSON *devices_array = (bus_idx == 0) ? left_devices : right_devices;

        for (int i = 0; i < bus_status[bus_idx].device_count; i++) {
            if (bus_status[bus_idx].device_status[i].status_updated || send_bus_all_data ||
                !bus_status[bus_idx].device_status[i].communication_flag) {
                cJSON *device                     = cJSON_CreateObject();
                chain_device_status_t *dev_status = &bus_status[bus_idx].device_status[i];

                // 基本设备信息
                cJSON_AddNumberToObject(device, "id", dev_status->id);
                cJSON_AddStringToObject(device, "type", chain_device_type_name(dev_status->type));
                cJSON_AddBoolToObject(device, "connected", dev_status->connected);
                if (dev_status->uid_valid) {
                    cJSON *uid_array = cJSON_CreateArray();
                    for (int j = 0; j < 4; j++) {
                        cJSON_AddItemToArray(uid_array, cJSON_CreateNumber(dev_status->uid[j]));
                    }
                    cJSON_AddItemToObject(device, "uid", uid_array);
                }
                cJSON_AddBoolToObject(device, "communication_flag", dev_status->communication_flag);

                // RGB状态
                cJSON_AddNumberToObject(device, "rgb_color", dev_status->rgb_color);
                cJSON_AddBoolToObject(device, "rgb_setting", dev_status->rgb_setting);

                // 设备数据
                cJSON *device_data = cJSON_CreateObject();
                switch (dev_status->type) {
                    case CHAIN_KEY_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "button_status",
                                                dev_status->device_data.key_data.button_status);
                        break;
                    case CHAIN_PIR_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "detect_status",
                                                dev_status->device_data.pir_data.detect_status);
                        cJSON_AddNumberToObject(device_data, "trigger_count",
                                                dev_status->device_data.pir_data.trigger_count);
                        break;
                    case CHAIN_JOYSTICK_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "x_value", dev_status->device_data.joystick_data.x_value);
                        cJSON_AddNumberToObject(device_data, "y_value", dev_status->device_data.joystick_data.y_value);
                        cJSON_AddNumberToObject(device_data, "button_status",
                                                dev_status->device_data.joystick_data.button_status);
                        break;
                    case CHAIN_ENCODER_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "encoder_value",
                                                dev_status->device_data.encoder_data.encoder_value);
                        cJSON_AddNumberToObject(device_data, "last_encoder_value",
                                                dev_status->device_data.encoder_data.last_encoder_value);
                        cJSON_AddNumberToObject(device_data, "button_status",
                                                dev_status->device_data.encoder_data.button_status);
                        break;
                    case CHAIN_TOF_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "distance", dev_status->device_data.tof_data.distance);
                        cJSON_AddNumberToObject(device_data, "last_distance",
                                                dev_status->device_data.tof_data.last_distance);
                        break;
                    case CHAIN_ANGLE_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "angle_value",
                                                dev_status->device_data.angle_data.angle_value);
                        cJSON_AddNumberToObject(device_data, "last_angle_value",
                                                dev_status->device_data.angle_data.last_angle_value);
                        break;
                    case CHAIN_SWITCH_TYPE_CODE:
                        cJSON_AddNumberToObject(device_data, "switch_status",
                                                dev_status->device_data.switch_data.switch_status);
                        cJSON_AddNumberToObject(device_data, "switch_count",
                                                dev_status->device_data.switch_data.switch_count);
                        break;
                    default:
                        break;
                }
                cJSON_AddItemToObject(device, "device_data", device_data);

                // 添加HID配置信息（根据设备类型）
                cJSON *hid_config = cJSON_CreateObject();
                switch (dev_status->type) {
                    case CHAIN_KEY_TYPE_CODE: {
                        cJSON_AddNumberToObject(hid_config, "single_click",
                                                dev_status->hid_config.key_config.single_click);
                        cJSON_AddNumberToObject(hid_config, "double_click",
                                                dev_status->hid_config.key_config.double_click);
                        cJSON_AddNumberToObject(hid_config, "long_press", dev_status->hid_config.key_config.long_press);
                        cJSON_AddNumberToObject(hid_config, "press_down", dev_status->hid_config.key_config.press_down);
                        cJSON_AddNumberToObject(hid_config, "press_release",
                                                dev_status->hid_config.key_config.press_release);
                        break;
                    }
                    case CHAIN_JOYSTICK_TYPE_CODE: {
                        cJSON_AddNumberToObject(hid_config, "single_click",
                                                dev_status->hid_config.joystick_config.single_click);
                        cJSON_AddNumberToObject(hid_config, "double_click",
                                                dev_status->hid_config.joystick_config.double_click);
                        cJSON_AddNumberToObject(hid_config, "long_press",
                                                dev_status->hid_config.joystick_config.long_press);
                        cJSON_AddNumberToObject(hid_config, "press_down",
                                                dev_status->hid_config.joystick_config.press_down);
                        cJSON_AddNumberToObject(hid_config, "press_release",
                                                dev_status->hid_config.joystick_config.press_release);
                        cJSON_AddBoolToObject(hid_config, "xy_move_reverse",
                                              dev_status->hid_config.joystick_config.xy_move_reverse);
                        cJSON_AddNumberToObject(hid_config, "xy_move_func",
                                                dev_status->hid_config.joystick_config.xy_move_func);
                        break;
                    }
                    case CHAIN_ENCODER_TYPE_CODE: {
                        cJSON_AddNumberToObject(hid_config, "single_click",
                                                dev_status->hid_config.encoder_config.single_click);
                        cJSON_AddNumberToObject(hid_config, "double_click",
                                                dev_status->hid_config.encoder_config.double_click);
                        cJSON_AddNumberToObject(hid_config, "long_press",
                                                dev_status->hid_config.encoder_config.long_press);
                        cJSON_AddNumberToObject(hid_config, "press_down",
                                                dev_status->hid_config.encoder_config.press_down);
                        cJSON_AddNumberToObject(hid_config, "press_release",
                                                dev_status->hid_config.encoder_config.press_release);
                        cJSON_AddNumberToObject(hid_config, "rotate_cw_func",
                                                dev_status->hid_config.encoder_config.rotate_cw_func);
                        cJSON_AddNumberToObject(hid_config, "rotate_ccw_func",
                                                dev_status->hid_config.encoder_config.rotate_ccw_func);
                        break;
                    }
                    case CHAIN_ANGLE_TYPE_CODE: {
                        cJSON_AddNumberToObject(hid_config, "angle_func",
                                                dev_status->hid_config.angle_config.angle_func);
                        break;
                    }
                    default:
                        // 其他设备类型不支持HID配置
                        break;
                }
                cJSON_AddItemToObject(device, "hid_config", hid_config);

                // 如果有新事件，添加事件信息
                if (dev_status->event_updated) {
                    cJSON_AddStringToObject(device, "last_event", dev_status->last_event_desc);
                    cJSON_AddNumberToObject(device, "event_time", dev_status->last_event_time);
                    dev_status->event_updated = false;  // 重置事件更新标记
                }

                cJSON_AddItemToArray(devices_array, device);
                dev_status->status_updated = false;  // 重置状态更新标记
                dev_status->rgb_updated    = false;  // 重置RGB更新标记
            }
        }
    }

    send_bus_all_data = false;
    cJSON_AddItemToObject(left_bus, "devices", left_devices);
    cJSON_AddItemToObject(right_bus, "devices", right_devices);

    cJSON_AddItemToObject(chainbus, "left_bus", left_bus);
    cJSON_AddItemToObject(chainbus, "right_bus", right_bus);

    cJSON_AddStringToObject(json, "type", "status_update");
    cJSON_AddItemToObject(json, "dualkey", dualkey);
    cJSON_AddItemToObject(json, "chainbus", chainbus);

    char *json_str = cJSON_PrintUnformatted(json);
    if (json_str) {
        httpd_ws_frame_t ws_pkt;
        memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t));
        ws_pkt.type    = HTTPD_WS_TYPE_TEXT;
        ws_pkt.payload = (uint8_t *)json_str;
        ws_pkt.len     = strlen(json_str);

        esp_err_t ret = httpd_ws_send_frame_async(server, websocket_fd, &ws_pkt);
        if (ret != ESP_OK) {
            ESP_LOGW(TAG, "WebSocket发送失败: %s", esp_err_to_name(ret));
        }

        free(json_str);
    }

    cJSON_Delete(json);
}

// 更新设备状态
static void update_device_status(void)
{
    // 更新拨码开关位置
    if (sys_param) {
        switch (switch_pos) {
            case 0:
                // g_device_status.usb_mode = 0;
                g_device_status.dip_switch_pos = 0;  // center
                break;
            case 1:
                // g_device_status.usb_mode = 1;
                g_device_status.dip_switch_pos = 1;  // left
                break;
            case 2:
                // g_device_status.usb_mode = 2;
                g_device_status.dip_switch_pos = 2;  // right
                break;
            default:
                break;
        }
    }

    // 更新电池状态 (使用test_case.c中的真实数据)
    g_device_status.battery_voltage    = g_battery_voltage;
    g_device_status.charge_status      = g_charging_status;
    g_device_status.battery_percentage = g_battery_percentage;
    g_device_status.usb_connected      = g_usb_connected;
    g_device_status.usb_voltage        = g_usb_voltage;

    // 更新蓝牙状态
    g_device_status.bluetooth_connected = (g_connect_status == 2);
    const char *device_name             = ble_hid_get_device_name();
    if (device_name != NULL) {
        strncpy(g_device_status.bluetooth_device_name, device_name, sizeof(g_device_status.bluetooth_device_name) - 1);
        g_device_status.bluetooth_device_name[sizeof(g_device_status.bluetooth_device_name) - 1] = '\0';
    }

    // 更新配对状态 (基于连接状态推断)
    g_device_status.bluetooth_pairing_status = g_connect_status;

    // 更新广播状态
    g_ble_adv_status                     = ble_hid_is_advertising();
    g_device_status.bluetooth_adv_status = g_ble_adv_status;

    // 同步按键映射状态
    g_device_status.current_key_mapping = btn_progress_get_key_mapping();

    // 更新WIFI状态
    if (wifi_ap_mode) {
        // AP模式：显示AP的信息
        g_device_status.wifi_connected = true;

        // 获取AP配置
        wifi_config_t wifi_config;
        esp_err_t ret = esp_wifi_get_config(WIFI_IF_AP, &wifi_config);
        if (ret == ESP_OK) {
            strncpy(g_device_status.wifi_ssid, (char *)wifi_config.ap.ssid, sizeof(g_device_status.wifi_ssid) - 1);
            g_device_status.wifi_ssid[sizeof(g_device_status.wifi_ssid) - 1] = '\0';
        } else {
            strcpy(g_device_status.wifi_ssid, "");
        }

        // 获取AP IP地址
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
        if (netif != NULL) {
            esp_netif_ip_info_t ip_info;
            ret = esp_netif_get_ip_info(netif, &ip_info);
            if (ret == ESP_OK) {
                inet_ntoa_r(ip_info.ip.addr, g_device_status.wifi_ip, sizeof(g_device_status.wifi_ip));
            } else {
                strcpy(g_device_status.wifi_ip, "0.0.0.0");
            }
        } else {
            strcpy(g_device_status.wifi_ip, "0.0.0.0");
        }

        g_device_status.wifi_rssi = 0;
    } else if (wifi_connected) {
        // STA模式已连接：显示连接的AP信息
        g_device_status.wifi_connected = true;

        // 获取SSID
        wifi_ap_record_t ap_info;
        esp_err_t ret = esp_wifi_sta_get_ap_info(&ap_info);
        if (ret == ESP_OK) {
            strncpy(g_device_status.wifi_ssid, (char *)ap_info.ssid, sizeof(g_device_status.wifi_ssid) - 1);
            g_device_status.wifi_ssid[sizeof(g_device_status.wifi_ssid) - 1] = '\0';
            g_device_status.wifi_rssi                                        = ap_info.rssi;
        } else {
            strcpy(g_device_status.wifi_ssid, "");
            g_device_status.wifi_rssi = 0;
        }

        // 获取IP地址
        esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
        if (netif != NULL) {
            esp_netif_ip_info_t ip_info;
            ret = esp_netif_get_ip_info(netif, &ip_info);
            if (ret == ESP_OK) {
                inet_ntoa_r(ip_info.ip.addr, g_device_status.wifi_ip, sizeof(g_device_status.wifi_ip));
            } else {
                strcpy(g_device_status.wifi_ip, "0.0.0.0");
            }
        } else {
            strcpy(g_device_status.wifi_ip, "0.0.0.0");
        }
    } else {
        // WiFi未连接
        g_device_status.wifi_connected = false;
        strcpy(g_device_status.wifi_ssid, "");
        strcpy(g_device_status.wifi_ip, "0.0.0.0");
        g_device_status.wifi_rssi = 0;
    }
}

static void set_key_rgb_color(int key_index, uint32_t rgb_color)
{
    uint8_t R = (rgb_color >> 16) & 0xFF;
    uint8_t G = (rgb_color >> 8) & 0xFF;
    uint8_t B = rgb_color & 0xFF;
    led_strip_set_pixel(led_strip, key_index, R, G, B);
    led_strip_refresh(led_strip);
}

// WebSocket任务
static void websocket_task(void *pvParameters)
{
    status_refresh_type_t refresh_msg;
    TickType_t timeout = pdMS_TO_TICKS(500);  // 500ms超时

    while (1) {
        // 等待状态刷新指令，超时时间为500ms
        if (xQueueReceive(status_refresh_queue, &refresh_msg, timeout) == pdTRUE) {
            // 收到刷新指令，立即刷新状态
            // ESP_LOGI(TAG, "Status refresh requested: %s", refresh_msg == STATUS_REFRESH_IMMEDIATE ? "immediate" :
            // "normal");
        } else {
            // 超时，执行正常的状态更新
            // ESP_LOGD(TAG, "Status update timeout, performing normal refresh");
        }

        // 更新设备状态
        update_device_status();
        websocket_send_status();

        // 更新电源状态
        update_power_status(NULL);
    }
}

// 启动web服务器
static httpd_handle_t start_webserver(void)
{
    if (server != NULL) {
        return server;
    }

    httpd_config_t config   = HTTPD_DEFAULT_CONFIG();
    config.max_open_sockets = 6;
    config.max_uri_handlers = 20;
    config.lru_purge_enable = true;

    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Registering URI handlers");

        // 注册URI处理器
        httpd_uri_t index_uri = {.uri = "/", .method = HTTP_GET, .handler = index_get_handler};
        httpd_register_uri_handler(server, &index_uri);

        httpd_uri_t styles_uri = {.uri = "/styles.css", .method = HTTP_GET, .handler = styles_get_handler};
        httpd_register_uri_handler(server, &styles_uri);

        httpd_uri_t script_uri = {.uri = "/script.js", .method = HTTP_GET, .handler = script_get_handler};
        httpd_register_uri_handler(server, &script_uri);

        // 注册WebSocket处理器
        httpd_uri_t websocket_uri = {
            .uri = "/ws", .method = HTTP_GET, .handler = websocket_handler, .user_ctx = NULL, .is_websocket = true};
        httpd_register_uri_handler(server, &websocket_uri);

        httpd_uri_t favicon_uri = {.uri = "/favicon.ico", .method = HTTP_GET, .handler = favicon_get_handler};
        httpd_register_uri_handler(server, &favicon_uri);

        httpd_uri_t chain_angle_uri = {
            .uri = "/Chain_Angle.jpg", .method = HTTP_GET, .handler = chain_angle_get_handler};
        httpd_register_uri_handler(server, &chain_angle_uri);

        httpd_uri_t chain_blank_uri = {
            .uri = "/Chain_Blank.jpg", .method = HTTP_GET, .handler = chain_blank_get_handler};
        httpd_register_uri_handler(server, &chain_blank_uri);

        httpd_uri_t chain_dualKey_uri = {
            .uri = "/Chain_DualKey.png", .method = HTTP_GET, .handler = chain_dualKey_get_handler};
        httpd_register_uri_handler(server, &chain_dualKey_uri);

        httpd_uri_t chain_encoder_uri = {
            .uri = "/Chain_Encoder.jpg", .method = HTTP_GET, .handler = chain_encoder_get_handler};
        httpd_register_uri_handler(server, &chain_encoder_uri);

        httpd_uri_t chain_joystick_uri = {
            .uri = "/Chain_Joystick.jpg", .method = HTTP_GET, .handler = chain_joystick_get_handler};
        httpd_register_uri_handler(server, &chain_joystick_uri);

        httpd_uri_t chain_key_uri = {.uri = "/Chain_Key.jpg", .method = HTTP_GET, .handler = chain_key_get_handler};
        httpd_register_uri_handler(server, &chain_key_uri);

        httpd_uri_t chain_mount_uri = {
            .uri = "/Chain_Mount.jpg", .method = HTTP_GET, .handler = chain_mount_get_handler};
        httpd_register_uri_handler(server, &chain_mount_uri);

        httpd_uri_t chain_tof_uri = {.uri = "/Chain_ToF.jpg", .method = HTTP_GET, .handler = chain_tof_get_handler};
        httpd_register_uri_handler(server, &chain_tof_uri);

        // 注册404错误处理器
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    }
    return server;
}

static void keyboard_cb(keyboard_btn_handle_t kbd_handle, keyboard_btn_report_t kbd_report, void *user_data)
{
    ESP_LOGI(TAG, "keyboard_cb: pressed=%ld, released=%ld, changed=%d", kbd_report.key_pressed_num,
             kbd_report.key_release_num, kbd_report.key_change_num);

    if (rgb_matrix_get_suspend_state() == true && g_device_status.left_key_color == 0x000000 &&
        g_device_status.right_key_color == 0x000000) {
        rgb_matrix_set_suspend_state(false);
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }

    // 更新按键状态
    g_device_status.left_key_pressed  = false;
    g_device_status.right_key_pressed = false;

    // 检查当前按下的按键
    for (int i = 0; i < kbd_report.key_pressed_num; i++) {
        uint8_t input_index = kbd_report.key_data[i].input_index;
        // 假设output_index 0对应左键，1对应右键
        if (input_index == 0) {
            g_device_status.left_key_pressed = true;
        } else if (input_index == 1) {
            g_device_status.right_key_pressed = true;
        }
    }

    btn_progress(kbd_report);

    /*!< Lighting with key pressed */
    if (kbd_report.key_change_num > 0) {
        for (int i = 1; i <= kbd_report.key_change_num; i++) {
            process_rgb_matrix(kbd_report.key_data[kbd_report.key_pressed_num - i].output_index,
                               kbd_report.key_data[kbd_report.key_pressed_num - i].input_index, true);
        }
    }

    // 按键状态变更时，发送立即刷新指令
    if (status_refresh_queue && kbd_report.key_change_num != 0) {
        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
        xQueueSend(status_refresh_queue, &refresh_msg, 0);
        // ESP_LOGI(TAG, "Sent immediate status refresh request due to key state change");
    }

    // iot_button is enabled for dual-button long press detection only when a button is pressed.
    if (kbd_report.key_pressed_num) {
        iot_button_resume();
    }
}

static void light_progress_task(void *pvParameters)
{
    while (1) {
        if (bsp_ws2812_is_enable()) {
            light_progress();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void adc_switch_task(void *pvParameters)
{
    adc_oneshot_unit_handle_t handle = (adc_oneshot_unit_handle_t)pvParameters;
    int adc_value[2]                 = {0};
    sys_param_t *sys_param           = settings_get_parameter();
    btn_report_type_t report_type    = sys_param->report_type;
    report_type                      = ALL_REPORT;  // 默认打印按键 不report kb功能
    while (1) {
        adc_oneshot_read(handle, KBD_ADC_SWITCH_BLE_CHAN, &adc_value[0]);
        adc_oneshot_read(handle, KBD_ADC_SWITCH_RAINMAKER_CHAN, &adc_value[1]);
        g_device_status.switch_1_value = adc_value[0];
        g_device_status.switch_2_value = adc_value[1];
        if (adc_value[0] > 2000) {
            // report_type = BLE_HID_REPORT;
            switch_pos = 2;
        } else if (adc_value[1] > 2000) {
            // report_type = USB_CDC_REPORT;
            switch_pos = 1;
        } else {
            // report_type = TINYUSB_HID_REPORT;
            switch_pos = 0;
        }

        // ESP_LOGI(TAG, "adc_value[0]: %d, adc_value[1]: %d", g_device_status.switch_1_value,
        // g_device_status.switch_2_value);

        if (report_type != sys_param->report_type) {
            sys_param->report_type = report_type;
            settings_write_parameter_to_nvs();

            // 拨码开关位置改变时，发送立即刷新指令
            if (status_refresh_queue) {
                status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                xQueueSend(status_refresh_queue, &refresh_msg, 0);
                ESP_LOGI(TAG, "Sent immediate status refresh request due to DIP switch change");
            }

            // esp_restart();
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

/**
 * @brief Press and hold for 4 seconds to start blinking.
 *        Release between 4s-7s to enter pairing/reset WiFi mode.
 *        If held for more than 7 seconds, ignore this long press.
 */
static void button_event_cb(void *button_handle, void *usr_data)
{
    button_handle_t btn  = (button_handle_t)button_handle;
    button_event_t event = iot_button_get_event(btn);
    uint32_t ticks       = iot_button_get_ticks_time(btn);
    if (event == BUTTON_LONG_PRESS_HOLD) {
        if (ticks >= 4000 && ticks <= 7000) {
            ESP_LOGI(TAG, "button_long_press_hold");
            update_power_status(led_strip);

            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

    } else if (event == BUTTON_LONG_PRESS_UP) {
        if (ticks >= 4000 && ticks <= 7000) {
            // 重置WiFi配置并重启
            ESP_LOGI(TAG, "重置WiFi配置");
            esp_err_t ret = wifi_config_reset();
            if (ret == ESP_OK) {
                ESP_LOGI(TAG, "WiFi配置已重置，即将重启...");
                // 闪烁LED指示成功
                for (int i = 0; i < 3; i++) {
                    led_strip_set_pixel(led_strip, 0, 255, 0, 0);
                    led_strip_set_pixel(led_strip, 1, 255, 0, 0);
                    led_strip_refresh(led_strip);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                    led_strip_clear(led_strip);
                    vTaskDelay(200 / portTICK_PERIOD_MS);
                }
                // 重启设备
                esp_restart();
            } else {
                ESP_LOGE(TAG, "WiFi配置重置失败");
            }
            rgb_matrix_set_suspend_state(false);
            iot_button_stop();
        }
        ESP_LOGI(TAG, "button_long_press_up");
    } else if (event == BUTTON_LONG_PRESS_START) {
        if (ticks >= 6900) {
            rgb_matrix_set_suspend_state(false);
            iot_button_stop();
        } else {
            rgb_matrix_set_suspend_state(true);
        }
        ESP_LOGI(TAG, "button_press_start");
    }
}

void app_dual_button_init(void)
{
    /*!< Init reset/reconfig event */
    const button_config_t btn_cfg = {
        .long_press_time  = 4000,
        .short_press_time = 0,
    };
    button_dual_config_t dual_cfg = {
        .active_level   = KBD_ATTIVE_LEVEL,
        .gpio_num       = KBD_INPUT_IOS,
        .disable_pull   = 0,
        .skip_gpio_init = true,
    };
    button_handle_t btn = NULL;
    iot_button_new_dual_button_device(&btn_cfg, &dual_cfg, &btn);
    iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, NULL, button_event_cb, NULL);
    iot_button_register_cb(btn, BUTTON_LONG_PRESS_HOLD, NULL, button_event_cb, NULL);
    iot_button_register_cb(btn, BUTTON_LONG_PRESS_UP, NULL, button_event_cb, NULL);
    button_event_args_t btn_event_cfg{};
    btn_event_cfg.long_press.press_time = 7000;
    iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, &btn_event_cfg, button_event_cb, NULL);
}

// 计算DualKey配置的CRC32
static uint32_t dualkey_config_calculate_crc(const dualkey_saved_config_t *config)
{
    return esp_crc32_le(0, (const uint8_t *)config, sizeof(dualkey_saved_config_t) - sizeof(uint32_t));
}

// 保存DualKey配置到NVS
static esp_err_t dualkey_config_save(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("dualkey_cfg", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "打开DualKey配置NVS失败: %s", esp_err_to_name(ret));
        return ret;
    }

    dualkey_saved_config_t config;
    memset(&config, 0, sizeof(config));

    // 填充配置数据
    config.left_key_color      = g_device_status.left_key_color;
    config.right_key_color     = g_device_status.right_key_color;
    config.current_key_mapping = g_device_status.current_key_mapping;
    config.usb_mapping_enabled = g_usb_mapping_enabled;
    config.ble_mapping_enabled = g_ble_mapping_enabled;
    config.crc32               = dualkey_config_calculate_crc(&config);

    // 保存到NVS
    ret = nvs_set_blob(nvs_handle, "config", &config, sizeof(config));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "保存DualKey配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    ret = nvs_commit(nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交DualKey配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "DualKey配置已保存: 左键颜色=0x%06lX, 右键颜色=0x%06lX, 映射=%d", config.left_key_color,
             config.right_key_color, config.current_key_mapping);

    return ESP_OK;
}

// 从NVS加载DualKey配置
static esp_err_t dualkey_config_load(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("dualkey_cfg", NVS_READONLY, &nvs_handle);
    if (ret != ESP_OK) {
        if (ret == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGI(TAG, "DualKey配置未找到，使用默认配置");
            return ESP_ERR_NOT_FOUND;
        }
        ESP_LOGE(TAG, "打开DualKey配置NVS失败: %s", esp_err_to_name(ret));
        return ret;
    }

    dualkey_saved_config_t config;
    size_t required_size = sizeof(config);

    ret = nvs_get_blob(nvs_handle, "config", &config, &required_size);
    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "DualKey配置未找到，使用默认配置");
        nvs_close(nvs_handle);
        return ESP_ERR_NOT_FOUND;
    }

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "读取DualKey配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    // 验证CRC
    uint32_t calculated_crc = dualkey_config_calculate_crc(&config);
    if (calculated_crc != config.crc32) {
        ESP_LOGE(TAG, "DualKey配置CRC校验失败");
        nvs_close(nvs_handle);
        return ESP_ERR_INVALID_CRC;
    }

    // 应用配置
    g_device_status.left_key_color      = config.left_key_color;
    g_device_status.right_key_color     = config.right_key_color;
    g_device_status.current_key_mapping = config.current_key_mapping;
    g_usb_mapping_enabled               = config.usb_mapping_enabled;
    g_ble_mapping_enabled               = config.ble_mapping_enabled;

    // 应用按键映射
    btn_progress_set_key_mapping(config.current_key_mapping);

    // 应用RGB颜色
    vTaskDelay(10 / portTICK_PERIOD_MS);
    if (config.left_key_color != 0x000000 || config.right_key_color != 0x000000) {
        if (rgb_matrix_get_suspend_state() == false) {
            rgb_matrix_set_suspend_state(true);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        for (int i = 0; i < 2; i++) {
            set_key_rgb_color(1, config.left_key_color);   // 左键 LED index 1
            set_key_rgb_color(0, config.right_key_color);  // 右键 LED index 0
            vTaskDelay(10 / portTICK_PERIOD_MS);
        }
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "DualKey配置已加载: 左键颜色=0x%06lX, 右键颜色=0x%06lX, 映射=%d", config.left_key_color,
             config.right_key_color, config.current_key_mapping);

    return ESP_OK;
}

// 计算WiFi配置的CRC32
static uint32_t wifi_config_calculate_crc(const wifi_config_saved_t *config)
{
    return esp_crc32_le(0, (const uint8_t *)config, sizeof(wifi_config_saved_t) - sizeof(uint32_t));
}

// 保存WiFi配置到NVS
static esp_err_t wifi_config_save(const wifi_config_saved_t *config)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("wifi_cfg", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "打开WiFi配置NVS失败: %s", esp_err_to_name(ret));
        return ret;
    }

    wifi_config_saved_t cfg_to_save;
    memcpy(&cfg_to_save, config, sizeof(wifi_config_saved_t));
    cfg_to_save.crc32 = wifi_config_calculate_crc(&cfg_to_save);

    // 保存到NVS
    ret = nvs_set_blob(nvs_handle, "config", &cfg_to_save, sizeof(cfg_to_save));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "保存WiFi配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    ret = nvs_commit(nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交WiFi配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "WiFi配置已保存: SSID=%s, 静态IP=%s", cfg_to_save.ssid, cfg_to_save.use_static_ip ? "启用" : "禁用");

    return ESP_OK;
}

// 从NVS加载WiFi配置
static esp_err_t wifi_config_load(wifi_config_saved_t *config)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("wifi_cfg", NVS_READONLY, &nvs_handle);
    if (ret != ESP_OK) {
        if (ret == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGI(TAG, "WiFi配置未找到");
            return ESP_ERR_NOT_FOUND;
        }
        ESP_LOGE(TAG, "打开WiFi配置NVS失败: %s", esp_err_to_name(ret));
        return ret;
    }

    size_t required_size = sizeof(wifi_config_saved_t);

    ret = nvs_get_blob(nvs_handle, "config", config, &required_size);
    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGI(TAG, "WiFi配置未找到");
        nvs_close(nvs_handle);
        return ESP_ERR_NOT_FOUND;
    }

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "读取WiFi配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    // 验证CRC
    uint32_t calculated_crc = wifi_config_calculate_crc(config);
    if (calculated_crc != config->crc32) {
        ESP_LOGE(TAG, "WiFi配置CRC校验失败");
        nvs_close(nvs_handle);
        return ESP_ERR_INVALID_CRC;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "WiFi配置已加载: SSID=%s", config->ssid);

    return ESP_OK;
}

// 重置WiFi配置
static esp_err_t wifi_config_reset(void)
{
    nvs_handle_t nvs_handle;
    esp_err_t ret = nvs_open("wifi_cfg", NVS_READWRITE, &nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "打开WiFi配置NVS失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 删除配置
    ret = nvs_erase_key(nvs_handle, "config");
    if (ret != ESP_OK && ret != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE(TAG, "删除WiFi配置失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    ret = nvs_commit(nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交WiFi配置删除失败: %s", esp_err_to_name(ret));
        nvs_close(nvs_handle);
        return ret;
    }

    nvs_close(nvs_handle);
    ESP_LOGI(TAG, "WiFi配置已重置");

    return ESP_OK;
}

void app_main(void)
{
    // 创建状态刷新消息队列
    status_refresh_queue = xQueueCreate(10, sizeof(status_refresh_type_t));
    if (status_refresh_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create status refresh queue");
    } else {
        ESP_LOGI(TAG, "Status refresh queue created successfully");
    }

    // Initialize NVS.
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /*!< Read System config */
    settings_read_parameter_from_nvs();
    sys_param = settings_get_parameter();

    /*!< Monitor adc switch */
    bsp_adc_switch_init();
    bsp_adc_charge_monitor_init();
    adc_oneshot_unit_handle_t handle = NULL;
    ret                              = bsp_get_adc_handle(&handle);
    assert(ret == ESP_OK);
    xTaskCreate(adc_switch_task, "adc_switch_task", 4096, handle, 5, NULL);

    /*!< Init LED and clear WS2812's status */
    bsp_ws2812_init(&led_strip);
    if (led_strip) {
        led_strip_clear(led_strip);
    }

    chain_bus_init();

    /* ADC detection test */
    test_adc_detection(led_strip, handle);

    bsp_rgb_matrix_init();
    bsp_ws2812_enable(true);
    xTaskCreate(light_progress_task, "light_progress_task", 4096, NULL, 5, &light_progress_task_handle);

    /*!< Init keyboard key monitor */
    bsp_keyboard_init(&kbd_handle, NULL);
    keyboard_btn_cb_config_t cb_cfg = {
        .event    = KBD_EVENT_PRESSED,
        .callback = keyboard_cb,
    };
    keyboard_button_register_cb(kbd_handle, cb_cfg, NULL);
    btn_progress_set_report_type(sys_param->report_type);

    app_dual_button_init();
    /*!< Init report */
    // switch (sys_param->report_type) {
    //     case TINYUSB_HID_REPORT:
    //         tinyusb_hid_init();
    //         break;
    //     case BLE_HID_REPORT:
    //         ble_hid_init();
    //         break;
    //     case USB_CDC_REPORT:
    //         tinyusb_cdc_init(NULL, NULL);
    //         ESP_LOGI(TAG, "USB CDC mode initialized");
    //         break;
    //     default:
    //         break;
    // }

    // 初始化WiFi
    wifi_init_sta();

    // 启动web服务器
    start_webserver();

    // 启动WebSocket状态更新任务
    xTaskCreate(websocket_task, "websocket_task", 1024 * 10, NULL, 5, &websocket_task_handle);

    tinyusb_hid_init();
    // tinyusb_cdc_init(NULL, NULL);
    ble_hid_init();

    rgb_matrix_mode(2);

    /*!< Load DualKey config */
    esp_err_t dualkey_ret = dualkey_config_load();
    if (dualkey_ret == ESP_OK) {
        ESP_LOGI(TAG, "DualKey配置加载成功");
    } else if (dualkey_ret == ESP_ERR_NOT_FOUND) {
        ESP_LOGI(TAG, "DualKey配置未找到，使用默认配置");
    } else {
        ESP_LOGW(TAG, "DualKey配置加载失败: %s，使用默认配置", esp_err_to_name(dualkey_ret));
    }

    static uint8_t last_battery_percentage = 255;
    while (1) {
        if (g_device_status.bluetooth_connected) {
            // 只在电量变化时发送，减少蓝牙通信负担
            if (last_battery_percentage != g_battery_percentage) {
                esp_err_t ret = ble_hid_set_battery(g_battery_percentage);
                if (ret == ESP_OK) {
                    last_battery_percentage = g_battery_percentage;
                    ESP_LOGI(TAG, "BLE battery updated: %d%%", g_battery_percentage);
                }
            }
        } else {
            // 连接断开时重置，确保下次连接会发送电量
            last_battery_percentage = 255;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
}