/* SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "btn_progress.h"

/**
 * @brief Initialize BLE HID device.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_ERR_NO_MEM: No memory
 */
esp_err_t ble_hid_init(void);

/**
 * @brief Deinitialize BLE HID device.
 *
 * @return
 *    - ESP_OK: Success
 *    - ESP_ERR_NO_MEM: No memory
 */
esp_err_t ble_hid_deinit(void);
/**
 * @brief Send ble hid keyboard report
 *
 * @param report hid report
 */
void ble_hid_keyboard_report(hid_report_t report);

/**
 * @brief Get BLE connection status
 *
 * @return true if connected, false otherwise
 */
bool ble_hid_is_connected(void);

/**
 * @brief Set BLE battery level
 *
 * @param level battery level (0-100)
 * @return esp_err_t
 *     - ESP_OK: success
 */
esp_err_t ble_hid_set_battery(uint8_t level);

/**
 * @brief Get BLE device name
 *
 * @return pointer to device name string
 */
const char* ble_hid_get_device_name(void);

/**
 * @brief Start BLE advertising
 *
 * @return esp_err_t
 *     - ESP_OK: success
 */
esp_err_t ble_hid_adv_start(void);

/**
 * @brief Stop BLE advertising
 *
 * @return esp_err_t
 *     - ESP_OK: success
 */
esp_err_t ble_hid_adv_stop(void);

/**
 * @brief Get BLE advertising status
 *
 * @return true if advertising, false otherwise
 */
bool ble_hid_is_advertising(void);

#ifdef __cplusplus
}
#endif
