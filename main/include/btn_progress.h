/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "esp_err.h"
#include "keyboard_button.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TINYUSB_HID_REPORT = 0,
    BLE_HID_REPORT,
    USB_CDC_REPORT,
    ALL_REPORT,
    BTN_REPORT_TYPE_MAX,
} btn_report_type_t;

typedef struct {
    uint32_t report_id;    // Report identifier
    union {
        struct {
            uint8_t modifier;     // Modifier keys
            uint8_t reserved;     // Reserved byte
            uint8_t keycode[15];  // Keycode
        } keyboard_full_key_report;  // Keyboard full key report
        struct {
            uint8_t modifier;   // Modifier keys
            uint8_t reserved;   // Reserved byte
            uint8_t keycode[6]; // Keycodes
        } keyboard_report;  // Keyboard report
        struct {
            uint16_t keycode;    // Keycode
        } consumer_report;
        struct {
            uint8_t buttons; /**< buttons mask for currently pressed buttons in the mouse. */
            int8_t  x;       /**< Current delta x movement of the mouse. */
            int8_t  y;       /**< Current delta y movement on the mouse. */
            int8_t  wheel;   /**< Current delta wheel movement on the mouse. */
            int8_t  pan;     // using AC Pan
        } mouse_report;
    };
} hid_report_t;

/**
 * @brief Button progress function.
 *
 * @param kbd_report Keyboard button report.
 */
void btn_progress(keyboard_btn_report_t kbd_report);

/**
 * @brief Set the report type for button progress.
 *
 * @param type Button report type.
 */
void btn_progress_set_report_type(btn_report_type_t type);

/**
 * @brief Function to handle light progress.
 */
void light_progress(void);

/**
 * @brief Set the key mapping index for button progress.
 *
 * @param mapping_index Key mapping index (0-3)
 */
void btn_progress_set_key_mapping(int mapping_index);

/**
 * @brief Get the current key mapping index.
 *
 * @return Current key mapping index
 */
int btn_progress_get_key_mapping(void);

#ifdef __cplusplus
}
#endif
