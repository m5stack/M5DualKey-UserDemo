/* SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_private/usb_phy.h"
#include "tinyusb_cdc.h"
#include "device/usbd.h"
#include "class/cdc/cdc_device.h"

static const char *TAG = "tinyusb_cdc";

typedef struct {
    TaskHandle_t task_handle;
    QueueHandle_t tx_queue;
    tinyusb_cdc_rx_callback_t rx_callback;
    void *rx_callback_arg;
} tinyusb_cdc_t;

static tinyusb_cdc_t *s_tinyusb_cdc = NULL;

//--------------------------------------------------------------------+
// USB PHY config
//--------------------------------------------------------------------+
static bool usb_phy_initialized = false;

static void usb_phy_init(void)
{
    if (usb_phy_initialized) {
        return;
    }
    
    usb_phy_handle_t phy_hdl;
    // Configure USB PHY
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_OTG,
        .otg_mode = USB_OTG_MODE_DEVICE,
    };
    phy_conf.target = USB_PHY_TARGET_INT;
    usb_new_phy(&phy_conf, &phy_hdl);
    usb_phy_initialized = true;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

static void tusb_device_task(void *arg)
{
    while (1) {
        tud_task();
    }
}

static void tinyusb_cdc_task(void *arg)
{
    uint8_t rx_buf[CFG_TUD_CDC_RX_BUFSIZE];
    
    while (1) {
        // 处理接收的数据
        if (tud_cdc_available()) {
            uint32_t count = tud_cdc_read(rx_buf, sizeof(rx_buf));
            if (count > 0 && s_tinyusb_cdc->rx_callback) {
                s_tinyusb_cdc->rx_callback(rx_buf, count, s_tinyusb_cdc->rx_callback_arg);
            }
        }
        
        // 处理发送队列
        uint8_t tx_buf[CFG_TUD_CDC_TX_BUFSIZE];
        cdc_tx_data_t tx_data;
        if (xQueueReceive(s_tinyusb_cdc->tx_queue, &tx_data, 10 / portTICK_PERIOD_MS) == pdTRUE) {
            if (tx_data.len > 0 && tx_data.len <= sizeof(tx_buf)) {
                memcpy(tx_buf, tx_data.data, tx_data.len);
                tud_cdc_write(tx_buf, tx_data.len);
                tud_cdc_write_flush();
            }
        }
        
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}

esp_err_t tinyusb_cdc_init(tinyusb_cdc_rx_callback_t rx_cb, void *rx_cb_arg)
{
    if (s_tinyusb_cdc) {
        ESP_LOGW(TAG, "tinyusb_cdc already initialized");
        return ESP_OK;
    }

    esp_err_t ret = ESP_OK;
    s_tinyusb_cdc = calloc(1, sizeof(tinyusb_cdc_t));
    ESP_RETURN_ON_FALSE(s_tinyusb_cdc, ESP_ERR_NO_MEM, TAG, "calloc failed");
    
    set_string_desc_product(USB_CDC_PRODUCT);

    s_tinyusb_cdc->rx_callback = rx_cb;
    s_tinyusb_cdc->rx_callback_arg = rx_cb_arg;
    
    // 只在 USB 未初始化时进行初始化
    if (!tud_inited()) {
        usb_phy_init();
        tud_init(BOARD_TUD_RHPORT);
        xTaskCreate(tusb_device_task, "TinyUSB", 4096, NULL, 5, NULL);
    }
    
    s_tinyusb_cdc->tx_queue = xQueueCreate(10, sizeof(cdc_tx_data_t));
    ESP_GOTO_ON_FALSE(s_tinyusb_cdc->tx_queue, ESP_ERR_NO_MEM, fail, TAG, "xQueueCreate failed");
    
    xTaskCreate(tinyusb_cdc_task, "tinyusb_cdc_task", 4096, NULL, 5, &s_tinyusb_cdc->task_handle);
    
    return ret;
    
fail:
    free(s_tinyusb_cdc);
    s_tinyusb_cdc = NULL;
    return ret;
}

esp_err_t tinyusb_cdc_deinit(void)
{
    if (!s_tinyusb_cdc) {
        return ESP_OK;
    }
    
    if (s_tinyusb_cdc->task_handle) {
        vTaskDelete(s_tinyusb_cdc->task_handle);
    }
    
    if (s_tinyusb_cdc->tx_queue) {
        vQueueDelete(s_tinyusb_cdc->tx_queue);
    }
    
    free(s_tinyusb_cdc);
    s_tinyusb_cdc = NULL;
    
    return ESP_OK;
}

esp_err_t tinyusb_cdc_write(const uint8_t *data, size_t len)
{
    ESP_RETURN_ON_FALSE(s_tinyusb_cdc, ESP_ERR_INVALID_STATE, TAG, "CDC not initialized");
    ESP_RETURN_ON_FALSE(data && len > 0, ESP_ERR_INVALID_ARG, TAG, "Invalid data or length");
    
    if (len > CFG_TUD_CDC_TX_BUFSIZE) {
        len = CFG_TUD_CDC_TX_BUFSIZE;
    }
    
    cdc_tx_data_t tx_data;
    memcpy(tx_data.data, data, len);
    tx_data.len = len;
    
    if (xQueueSend(s_tinyusb_cdc->tx_queue, &tx_data, pdMS_TO_TICKS(100)) != pdTRUE) {
        ESP_LOGW(TAG, "TX queue full");
        return ESP_ERR_TIMEOUT;
    }
    
    return ESP_OK;
}

void tinyusb_cdc_send_string(const char *str)
{
    if (str) {
        tinyusb_cdc_write((const uint8_t *)str, strlen(str));
    }
}

void tinyusb_cdc_send_keyboard_report(keyboard_btn_report_t report)
{
    char buffer[256];
    int len = 0;
    
    // 格式化键盘报告为可读字符串
    len += snprintf(buffer + len, sizeof(buffer) - len, 
                    "Keys pressed: %ld, released: %ld, changed: %d\r\n",
                    report.key_pressed_num, report.key_release_num, report.key_change_num);
    
    // 打印按下的键
    if (report.key_pressed_num > 0) {
        len += snprintf(buffer + len, sizeof(buffer) - len, "Pressed: ");
        for (int i = 0; i < report.key_pressed_num; i++) {
            len += snprintf(buffer + len, sizeof(buffer) - len, "(%d,%d) ", 
                          report.key_data[i].output_index, 
                          report.key_data[i].input_index);
        }
        len += snprintf(buffer + len, sizeof(buffer) - len, "\r\n");
    }
    
    // 打印释放的键
    if (report.key_release_num > 0) {
        len += snprintf(buffer + len, sizeof(buffer) - len, "Released: ");
        for (int i = 0; i < report.key_release_num; i++) {
            len += snprintf(buffer + len, sizeof(buffer) - len, "(%d,%d) ", 
                          report.key_release_data[i].output_index, 
                          report.key_release_data[i].input_index);
        }
        len += snprintf(buffer + len, sizeof(buffer) - len, "\r\n");
    }
    
    tinyusb_cdc_send_string(buffer);
}

// TinyUSB CDC callbacks
void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    ESP_LOGI(TAG, "CDC line state: DTR=%d, RTS=%d", dtr, rts);
}

void tud_cdc_rx_cb(uint8_t itf)
{
    // 数据接收在任务中处理
} 