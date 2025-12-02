/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "adc_detect.h"
#include "memory.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "esp_timer.h"
#include "esp_pm.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "bsp/esp-bsp.h"
#include "rgb_matrix.h"
#include "settings.h"
#include "bsp/esp_duo.h"
#include "driver/uart.h"
#include "esp_crc.h"
#include "string.h"
#include "driver/rtc_io.h"
#include "driver/adc.h"

static const char *TAG = "test_case";

// LED互斥锁
static SemaphoreHandle_t led_mutex = NULL;

// ADC检测配置
#define ADC_BAT_GPIO  10  // 电池电压检测GPIO
#define ADC_CHRG_GPIO 9   // 充电状态检测GPIO
#define ADC_VBUS_GPIO 2   // USB电压检测GPIO

#define ADC_BAT_CHANNEL  ADC_CHANNEL_9  // GPIO10对应ADC1_CH9
#define ADC_CHRG_CHANNEL ADC_CHANNEL_8  // GPIO9对应ADC1_CH8
#define ADC_VBUS_CHANNEL ADC_CHANNEL_1  // GPIO2对应ADC1_CH1

#define ADC_ATTEN        ADC_ATTEN_DB_11  // 衰减11dB, 测量范围0-3.3V
#define ADC_BITWIDTH     ADC_BITWIDTH_12  // 12位精度
#define ADC_SAMPLE_COUNT 64               // 采样次数，用于平均滤波

// 分压比例定义
#define VOLTAGE_DIVIDER_RATIO_BAT  1.51f  // 电池检测: (51+100)/100 = 1.51
#define VOLTAGE_DIVIDER_RATIO_VBUS 1.51f  // VBUS检测: (51+100)/100 = 1.51
#define VREF_VOLTAGE               3333   // 参考电压3.3V (mV) Vbat*0.662
#define ADC_MAX_VALUE              4095   // 12位ADC最大值

// 充电状态电压阈值 (mV)
#define CHRG_NOT_CHARGING 3000  // 没充电：>3.0V
#define CHRG_CHARGING     1400  // 充电中：1.4V-1.9V
#define CHRG_CHARGED      1900  // 充满电：1.9V-3.0V

// ADC检测结果结构
typedef struct {
    float battery_voltage;      // 电池电压 (V)
    float vbus_voltage;         // USB电压 (V)
    float chrg_voltage;         // 充电检测电压 (V)
    const char *charge_status;  // 充电状态字符串
    bool usb_connected;         // USB连接状态
    bool battery_low;           // 电池低电量标志
} adc_result_t;

// ADC句柄
static adc_oneshot_unit_handle_t adc_handle = NULL;

// 全局变量，供其他模块访问
float g_battery_voltage  = 3.7f;
int g_charging_status    = 0;
int g_battery_percentage = 75;
float g_usb_voltage      = 0.0f;
bool g_usb_connected     = false;

// 安全的LED操作函数
static void safe_led_set_and_refresh(led_strip_handle_t led_strip, int index, uint8_t r, uint8_t g, uint8_t b,
                                     int delay_ms)
{
    if (led_mutex == NULL || led_strip == NULL) {
        return;
    }

    if (xSemaphoreTake(led_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        led_strip_set_pixel(led_strip, index, r, g, b);
        led_strip_refresh(led_strip);
        if (delay_ms > 0) {
            vTaskDelay(pdMS_TO_TICKS(delay_ms));
            led_strip_set_pixel(led_strip, index, 0, 0, 0);
            led_strip_refresh(led_strip);
        }
        xSemaphoreGive(led_mutex);
    }
}

// 初始化ADC
static esp_err_t init_adc(adc_oneshot_unit_handle_t handle)
{
    adc_handle = handle;

    // 配置ADC通道
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH,
        .atten    = ADC_ATTEN,
    };

    // 配置电池电压检测通道
    esp_err_t ret = adc_oneshot_config_channel(adc_handle, ADC_BAT_CHANNEL, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to config battery ADC channel: %s", esp_err_to_name(ret));
        return ret;
    }

    // 配置充电状态检测通道
    ret = adc_oneshot_config_channel(adc_handle, ADC_CHRG_CHANNEL, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to config charge ADC channel: %s", esp_err_to_name(ret));
        return ret;
    }

    // 配置USB电压检测通道
    ret = adc_oneshot_config_channel(adc_handle, ADC_VBUS_CHANNEL, &config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to config VBUS ADC channel: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "ADC initialized successfully");
    return ESP_OK;
}

// 多次采样并平均
static int adc_read_average(adc_channel_t channel, int sample_count)
{
    int sum           = 0;
    int valid_samples = 0;

    for (int i = 0; i < sample_count; i++) {
        int adc_raw   = 0;
        esp_err_t ret = adc_oneshot_read(adc_handle, channel, &adc_raw);
        if (ret == ESP_OK) {
            sum += adc_raw;
            valid_samples++;
        }
        vTaskDelay(pdMS_TO_TICKS(1));  // 1ms延时
    }

    if (valid_samples > 0) {
        return sum / valid_samples;
    }
    return 0;
}

// 将ADC原始值转换为电压 (mV)
static float adc_raw_to_voltage(int adc_raw)
{
    return (float)adc_raw * VREF_VOLTAGE / ADC_MAX_VALUE;
}

// 获取充电状态字符串
static const char *get_charge_status_string(float chrg_voltage_mv)
{
    if (chrg_voltage_mv > CHRG_NOT_CHARGING) {
        return "Not Charging";
    } else if (chrg_voltage_mv > CHRG_CHARGED) {
        return "Fully Charged";
    } else if (chrg_voltage_mv > CHRG_CHARGING) {
        return "Charging";
    } else {
        return "Unknown";
    }
}

// 执行ADC检测
static adc_result_t perform_adc_detection(void)
{
    adc_result_t result = {0};

    // 读取电池电压
    int bat_raw            = adc_read_average(ADC_BAT_CHANNEL, ADC_SAMPLE_COUNT);
    float bat_voltage_mv   = adc_raw_to_voltage(bat_raw);
    result.battery_voltage = bat_voltage_mv * VOLTAGE_DIVIDER_RATIO_BAT / 1000.0f;  // 转换为V
    result.battery_low     = (result.battery_voltage < 3.2f);                       // 低于3.2V认为低电量

    // 读取USB电压
    int vbus_raw          = adc_read_average(ADC_VBUS_CHANNEL, ADC_SAMPLE_COUNT);
    float vbus_voltage_mv = adc_raw_to_voltage(vbus_raw);
    result.vbus_voltage   = vbus_voltage_mv * VOLTAGE_DIVIDER_RATIO_VBUS / 1000.0f;  // 转换为V
    result.usb_connected  = (result.vbus_voltage > 4.0f);                            // 高于4.0V认为USB连接

    // 读取充电状态
    int chrg_raw         = adc_read_average(ADC_CHRG_CHANNEL, ADC_SAMPLE_COUNT);
    result.chrg_voltage  = adc_raw_to_voltage(chrg_raw) / 1000.0f;  // 转换为V
    result.charge_status = get_charge_status_string(result.chrg_voltage * 1000);

    return result;
}

// LED状态指示
static void update_status_leds(led_strip_handle_t led_strip, const adc_result_t *result)
{
    if (led_mutex == NULL || led_strip == NULL) {
        return;
    }

    if (xSemaphoreTake(led_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
        // LED0: 电池状态指示
        if (result->battery_low) {
            led_strip_set_pixel(led_strip, 0, 255, 0, 0);  // 红色 - 低电量
        } else if (result->battery_voltage > 4.0f) {
            led_strip_set_pixel(led_strip, 0, 0, 255, 0);  // 绿色 - 电量充足
        } else {
            led_strip_set_pixel(led_strip, 0, 255, 255, 0);  // 黄色 - 电量中等
        }

        // LED1: 充电状态指示
        if (strcmp(result->charge_status, "Charging") == 0) {
            led_strip_set_pixel(led_strip, 1, 0, 0, 255);  // 蓝色 - 充电中
        } else if (strcmp(result->charge_status, "Fully Charged") == 0) {
            led_strip_set_pixel(led_strip, 1, 0, 255, 0);  // 绿色 - 充满电
        } else if (result->usb_connected) {
            led_strip_set_pixel(led_strip, 1, 255, 0, 255);  // 紫色 - USB连接但不充电
        } else {
            led_strip_set_pixel(led_strip, 1, 0, 0, 0);  // 关闭 - 无USB连接
        }

        led_strip_refresh(led_strip);
        xSemaphoreGive(led_mutex);
    }
}

// ADC检测任务
static void adc_detection_task(void *arg)
{
    led_strip_handle_t led_strip = (led_strip_handle_t)arg;
    TickType_t last_wake_time    = xTaskGetTickCount();

    while (1) {
        // 执行ADC检测
        adc_result_t result = perform_adc_detection();

        // 打印检测结果
        ESP_LOGI(TAG, "=== ADC Detection Results ===");
        ESP_LOGI(TAG, "Battery Voltage: %.2fV %s", result.battery_voltage, result.battery_low ? "(LOW!)" : "(OK)");
        ESP_LOGI(TAG, "USB Voltage: %.2fV %s", result.vbus_voltage,
                 result.usb_connected ? "(Connected)" : "(Disconnected)");
        ESP_LOGI(TAG, "Charge Status: %s (%.2fV)", result.charge_status, result.chrg_voltage);
        ESP_LOGI(TAG, "=============================");

        // 更新LED状态
        update_status_leds(led_strip, &result);

        // 每2秒检测一次
        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(2000));
    }
}

// ADC检测测试主函数
void test_adc_detection(led_strip_handle_t led_strip, adc_oneshot_unit_handle_t adc_handle)
{
    ESP_LOGI(TAG, "=== ADC Detection Test ===");

    // 创建LED互斥锁（如果还没有创建）
    if (led_mutex == NULL) {
        led_mutex = xSemaphoreCreateMutex();
        if (led_mutex == NULL) {
            ESP_LOGE(TAG, "Failed to create LED mutex");
            return;
        }
    }

    // 显示测试开始状态 - 白色闪烁
    for (int i = 0; i < 3; i++) {
        safe_led_set_and_refresh(led_strip, 0, 255, 255, 255, 0);
        safe_led_set_and_refresh(led_strip, 1, 255, 255, 255, 0);
        vTaskDelay(pdMS_TO_TICKS(200));
        safe_led_set_and_refresh(led_strip, 0, 0, 0, 0, 0);
        safe_led_set_and_refresh(led_strip, 1, 0, 0, 0, 0);
        vTaskDelay(pdMS_TO_TICKS(200));
    }

    // 初始化ADC
    esp_err_t ret = init_adc(adc_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "ADC initialization failed");
        // 错误指示 - 红色闪烁
        for (int i = 0; i < 5; i++) {
            safe_led_set_and_refresh(led_strip, 0, 255, 0, 0, 100);
            safe_led_set_and_refresh(led_strip, 1, 255, 0, 0, 100);
        }
        return;
    }

    ESP_LOGI(TAG, "ADC Detection Configuration:");
    ESP_LOGI(TAG, "- GPIO%d: Battery voltage (VBAT)", ADC_BAT_GPIO);
    ESP_LOGI(TAG, "- GPIO%d: Charge status (TP4057)", ADC_CHRG_GPIO);
    ESP_LOGI(TAG, "- GPIO%d: USB voltage (VBUS)", ADC_VBUS_GPIO);
    ESP_LOGI(TAG, "- Voltage divider ratio: %.2f", VOLTAGE_DIVIDER_RATIO_BAT);
    ESP_LOGI(TAG, "- Sample count: %d", ADC_SAMPLE_COUNT);

    ESP_LOGI(TAG, "Charge Status Thresholds:");
    ESP_LOGI(TAG, "- Not Charging: >%.1fV", CHRG_NOT_CHARGING / 1000.0f);
    ESP_LOGI(TAG, "- Fully Charged: %.1fV-%.1fV", CHRG_CHARGED / 1000.0f, CHRG_NOT_CHARGING / 1000.0f);
    ESP_LOGI(TAG, "- Charging: %.1fV-%.1fV", CHRG_CHARGING / 1000.0f, CHRG_CHARGED / 1000.0f);

    ESP_LOGI(TAG, "LED Status Indicators:");
    ESP_LOGI(TAG, "  LED0 - Battery Status:");
    ESP_LOGI(TAG, "    Red: Low battery (<3.2V)");
    ESP_LOGI(TAG, "    Yellow: Medium battery (3.2V-4.0V)");
    ESP_LOGI(TAG, "    Green: High battery (>4.0V)");
    ESP_LOGI(TAG, "  LED1 - Charge Status:");
    ESP_LOGI(TAG, "    Blue: Charging");
    ESP_LOGI(TAG, "    Green: Fully charged");
    ESP_LOGI(TAG, "    Purple: USB connected, not charging");
    ESP_LOGI(TAG, "    Off: USB disconnected");

    // 创建ADC检测任务
    // xTaskCreate(adc_detection_task, "adc_detection_task", 4096, led_strip, 5, NULL);
    update_power_status(led_strip);
}

static int calculate_smooth_percentage(float voltage, bool is_charging)
{
    // 定义查找表结构
    typedef struct { float v; int p; } point_t;
    
    // 充电曲线表 (电压从小到大)
    const point_t chrg_table[] = {
        {3.40f, 0}, {3.61f, 25}, {3.88f, 50}, {4.12f, 75}, {4.20f, 100}
    };
    // 放电曲线表 (电压从小到大)
    const point_t dischrg_table[] = {
        {3.33f, 0}, {3.55f, 25}, {3.81f, 50}, {4.07f, 75}, {4.20f, 100}
    };

    const point_t *table = is_charging ? chrg_table : dischrg_table;
    int count = 5;

    if (voltage <= table[0].v) return 0;
    if (voltage >= table[count-1].v) return 100;

    // 线性插值
    for (int i = 0; i < count - 1; i++) {
        if (voltage >= table[i].v && voltage < table[i+1].v) {
            float range_v = table[i+1].v - table[i].v;
            float range_p = table[i+1].p - table[i].p;
            float offset = voltage - table[i].v;
            return (int)(table[i].p + (offset / range_v) * range_p);
        }
    }
    return 100;
}

void update_power_status(led_strip_handle_t led_strip)
{
    adc_result_t result = perform_adc_detection();
    
    // 更新全局变量
    g_battery_voltage = result.battery_voltage;
    
    // 判定充电状态: 0=未充电, 1=充电中, 2=充满
    g_charging_status = (strcmp(result.charge_status, "Charging") == 0) ? 1
                        : (strcmp(result.charge_status, "Fully Charged") == 0) ? 2
                        : 0;
    g_usb_voltage = result.vbus_voltage;
    
    g_usb_connected = result.usb_connected;

    // --- 优化后的电量计算逻辑开始 ---
    if (g_charging_status == 2) {
        // 硬件指示已充满，强制100%
        g_battery_percentage = 100;
    } else {
        // 根据充电/放电状态选择不同的曲线
        bool is_charging_curve = (g_charging_status == 1); 
        g_battery_percentage = calculate_smooth_percentage(g_battery_voltage, is_charging_curve);
    }
    // --- 优化后的电量计算逻辑结束 ---

    // 边界保护
    if (g_battery_percentage > 100) g_battery_percentage = 100;
    if (g_battery_percentage < 0) g_battery_percentage = 0;

    // 更新LED状态
    if (led_strip != NULL) {
        // 打印检测结果，增加百分比显示
        ESP_LOGI(TAG, "=== ADC Detection Results ===");
        ESP_LOGI(TAG, "Battery: %.2fV [%d%%] %s", 
                 result.battery_voltage, 
                 g_battery_percentage,
                 result.battery_low ? "(LOW!)" : "(OK)");
        ESP_LOGI(TAG, "USB Voltage: %.2fV %s", result.vbus_voltage,
                 result.usb_connected ? "(Connected)" : "(Disconnected)");
        ESP_LOGI(TAG, "Charge Status: %s (%.2fV)", result.charge_status, result.chrg_voltage);
        ESP_LOGI(TAG, "=============================");
        update_status_leds(led_strip, &result);
    }
}

void low_power_test(void)
{
    // gpio_reset_pin((gpio_num_t)21);
    // gpio_set_direction((gpio_num_t)21, GPIO_MODE_INPUT);
    // // gpio_set_level((gpio_num_t)21, 1); //拉高
    // gpio_set_pull_mode((gpio_num_t)21, GPIO_PULLDOWN_ONLY);

    gpio_deep_sleep_hold_en();
    esp_deep_sleep_start();
}