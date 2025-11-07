/* SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include "keyboard_button.h"

// CDC 数据传输结构
typedef struct {
    uint8_t data[64];
    size_t len;
} cdc_tx_data_t;

// CDC 接收回调函数类型
typedef void (*tinyusb_cdc_rx_callback_t)(const uint8_t *data, size_t len, void *arg);

/**
 * @brief Initialize USB CDC device
 *
 * @param rx_cb Receive callback function
 * @param rx_cb_arg Callback argument
 * @return
 *    - ESP_OK: Success
 *    - ESP_ERR_NO_MEM: No memory
 */
esp_err_t tinyusb_cdc_init(tinyusb_cdc_rx_callback_t rx_cb, void *rx_cb_arg);

/**
 * @brief Deinitialize USB CDC device
 *
 * @return
 *    - ESP_OK: Success
 */
esp_err_t tinyusb_cdc_deinit(void);

/**
 * @brief Write data to USB CDC
 *
 * @param data Data to write
 * @param len Data length
 * @return
 *    - ESP_OK: Success
 *    - ESP_ERR_INVALID_STATE: CDC not initialized
 *    - ESP_ERR_INVALID_ARG: Invalid arguments
 *    - ESP_ERR_TIMEOUT: Queue full
 */
esp_err_t tinyusb_cdc_write(const uint8_t *data, size_t len);

/**
 * @brief Send string through USB CDC
 *
 * @param str Null-terminated string to send
 */
void tinyusb_cdc_send_string(const char *str);

/**
 * @brief Send keyboard report through USB CDC
 *
 * @param report Keyboard button report
 */
void tinyusb_cdc_send_keyboard_report(keyboard_btn_report_t report);

#ifdef __cplusplus
}
#endif