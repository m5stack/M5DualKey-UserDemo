/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "btn_progress.h"
#include "tinyusb_hid.h"
#include "ble_hid.h"
#include "tinyusb_cdc.h"
#include "bsp/keycodes.h"
#include "bsp/keymap.h"
#include "usb_descriptors.h"
#include "rgb_matrix.h"
#include "settings.h"
#include "esp_system.h"
#include "esp_pm.h"
#include "bsp/esp-bsp.h"
#include "esp_log.h"

// 声明外部变量来获取设备状态
extern bool g_usb_mapping_enabled;
extern bool g_ble_mapping_enabled;

static btn_report_type_t report_type = USB_CDC_REPORT;

// 当前按键映射索引，默认为1（翻页模式）
static int current_key_mapping_index = 1;

#define HSV_MAX 18
static uint8_t hsv_index             = 0;
static uint8_t hsv_color[HSV_MAX][3] = {
    {132, 102, 255},  // HSV_AZURE
    {170, 255, 255},  // HSV_BLUE
    {64, 255, 255},   // HSV_CHARTREUSE
    {11, 176, 255},   // HSV_CORAL
    {128, 255, 255},  // HSV_CYAN
    {36, 255, 255},   // HSV_GOLD
    {30, 218, 218},   // HSV_GOLDENROD
    {85, 255, 255},   // HSV_GREEN
    {213, 255, 255},  // HSV_MAGENTA
    {21, 255, 255},   // HSV_ORANGE
    {234, 128, 255},  // HSV_PINK
    {191, 255, 255},  // HSV_PURPLE
    {0, 255, 255},    // HSV_RED
    {106, 255, 255},  // HSV_SPRINGGREEN
    {128, 255, 128},  // HSV_TEAL
    {123, 90, 112},   // HSV_TURQUOISE
    {0, 0, 255},      // HSV_WHITE
    {43, 255, 255},   // HSV_YELLOW
};

static void _report(hid_report_t report)
{
    switch (report_type) {
        case TINYUSB_HID_REPORT:
            tinyusb_hid_keyboard_report(report);
            break;
        case BLE_HID_REPORT:
            ble_hid_keyboard_report(report);
            break;
        case USB_CDC_REPORT:
            // CDC 模式不发送 HID 报告，在 btn_progress 中直接处理
            break;
        case ALL_REPORT:
            if (g_usb_mapping_enabled) {
                tinyusb_hid_keyboard_report(report);
            }
            if (g_ble_mapping_enabled) {
                ble_hid_keyboard_report(report);
            }
            break;
        default:
            break;
    }
}

void btn_progress(keyboard_btn_report_t kbd_report)
{
    static uint8_t layer         = 1;
    uint8_t mo_action_layer      = layer;
    uint8_t keycode[120]         = {0};
    uint8_t keynum               = 0;
    uint8_t modify               = 0;
    hid_report_t kbd_hid_report  = {0};
    hid_report_t consumer_report = {0};
    bool if_consumer_report      = false;
    bool release_consumer_report = false;
    sys_param_t *sys_param       = settings_get_parameter();

    if (sys_param->report_type == USB_CDC_REPORT) {
        // USB CDC 模式：发送原始键盘数据
        // tinyusb_cdc_send_keyboard_report(kbd_report);
        return;
    }

    /*!< Report with key pressed */
    for (int i = 0; i < kbd_report.key_pressed_num; i++) {
        uint8_t row = kbd_report.key_data[i].output_index;
        uint8_t col = kbd_report.key_data[i].input_index;
        // 使用当前按键映射索引而不是固定的层
        uint16_t kc = keymaps[current_key_mapping_index][row][col];

        switch (kc) {
            case QK_MOMENTARY ... QK_MOMENTARY_MAX:
                // Momentary action_layer
                mo_action_layer = QK_MOMENTARY_GET_LAYER(kc);
                printf("Momentary action_layer: %d\n", mo_action_layer);
                break;

            case QK_LCTL ... QK_LSFT:
                // add HID_KEY_CONTROL_LEFT
                modify |= 1;
                keycode[keynum++] = QK_MODS_GET_BASIC_KEYCODE(kc);
                break;

            case HID_KEY_CONTROL_LEFT ... HID_KEY_GUI_RIGHT:
                // Modifier key
                modify |= 1 << (kc - HID_KEY_CONTROL_LEFT);
                break;
                ;

            case KC_KB_MUTE:
                consumer_report.consumer_report.keycode = HID_USAGE_CONSUMER_MUTE;
                if_consumer_report                      = true;
                break;
            case KC_KB_VOLUME_UP:
                consumer_report.consumer_report.keycode = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
                if_consumer_report                      = true;
                break;
            case KC_KB_VOLUME_DOWN:
                consumer_report.consumer_report.keycode = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
                if_consumer_report                      = true;
                break;

            case KC_MEDIA_PREV_TRACK:
                consumer_report.consumer_report.keycode = HID_USAGE_CONSUMER_SCAN_PREVIOUS;
                if_consumer_report                      = true;
                break;

            case KC_MEDIA_NEXT_TRACK:
                consumer_report.consumer_report.keycode = HID_USAGE_CONSUMER_SCAN_NEXT;
                if_consumer_report                      = true;
                break;

            case QK_BACKLIGHT_UP:
                rgb_matrix_increase_val();
                break;

            case QK_BACKLIGHT_DOWN:
                rgb_matrix_decrease_val();
                break;

            case RGB_SPD:
                rgb_matrix_decrease_speed();
                break;

            case RGB_SPI:
                rgb_matrix_increase_speed();
                break;

            case QK_BACKLIGHT_TOGGLE:
                rgb_matrix_toggle();
                if (!rgb_matrix_is_enabled()) {
                    bsp_ws2812_clear();
                }
                bsp_ws2812_enable(rgb_matrix_is_enabled());
                break;

            case RGB_MODE_FORWARD: {
                uint16_t index = (rgb_matrix_get_mode() + 1) % RGB_MATRIX_EFFECT_MAX;
                rgb_matrix_mode(index);
                break;
            }

            case RGB_MODE_REVERSE: {
                uint16_t index = rgb_matrix_get_mode() - 1;
                if (index < 1) {
                    index = RGB_MATRIX_EFFECT_MAX - 1;
                }
                rgb_matrix_mode(index);
                break;
            }

            case RGB_TOG:
                rgb_matrix_sethsv(hsv_color[hsv_index][0], hsv_color[hsv_index][1], hsv_color[hsv_index][2]);
                hsv_index = (hsv_index + 1) % HSV_MAX;
                break;

            default:
                if (kc != HID_KEY_NONE) {
                    keycode[keynum++] = kc;
                }
                break;
        }
    }

    /*!< Find if consumer key release */
    for (int i = 0; i < kbd_report.key_release_num; i++) {
        uint8_t row = kbd_report.key_release_data[i].output_index;
        uint8_t col = kbd_report.key_release_data[i].input_index;
        // 使用当前按键映射索引
        uint16_t kc = keymaps[current_key_mapping_index][row][col];
        switch (kc) {
            case KC_KB_MUTE:
                release_consumer_report = true;
                break;

            case KC_KB_VOLUME_UP:
                release_consumer_report = true;
                break;

            case KC_KB_VOLUME_DOWN:
                release_consumer_report = true;
                break;
        }
    }

    if (keynum <= 6) {
        kbd_hid_report.report_id                = REPORT_ID_KEYBOARD;
        kbd_hid_report.keyboard_report.modifier = modify;
        for (int i = 0; i < keynum; i++) {
            kbd_hid_report.keyboard_report.keycode[i] = keycode[i];
        }
    } else {
        kbd_hid_report.report_id                         = REPORT_ID_FULL_KEY_KEYBOARD;
        kbd_hid_report.keyboard_full_key_report.modifier = modify;
        for (int i = 0; i < keynum; i++) {
            // USAGE ID for keyboard starts from 4
            uint8_t key       = keycode[i] - 3;
            uint8_t byteIndex = (key - 1) / 8;
            uint8_t bitIndex  = (key - 1) % 8;
            kbd_hid_report.keyboard_full_key_report.keycode[byteIndex] |= (1 << bitIndex);
        }
    }

    _report(kbd_hid_report);

    if (if_consumer_report && !release_consumer_report) {
        consumer_report.report_id = REPORT_ID_CONSUMER;
        _report(consumer_report);
    } else if (release_consumer_report) {
        consumer_report.report_id               = REPORT_ID_CONSUMER;
        consumer_report.consumer_report.keycode = 0;
        _report(consumer_report);
    }
}

void light_progress(void)
{
    rgb_matrix_task();

    // TODO: for Rainmaker light
    // rgb_matrix_set_suspend_state(false);
}

void btn_progress_set_report_type(btn_report_type_t type)
{
    report_type = type;
}

void btn_progress_set_key_mapping(int mapping_index)
{
    if (mapping_index >= 0 && mapping_index < 4) {
        current_key_mapping_index = mapping_index;
        ESP_LOGI("btn_progress", "按键映射已设置为: %d", mapping_index);
    } else {
        ESP_LOGW("btn_progress", "无效的按键映射索引: %d", mapping_index);
    }
}

int btn_progress_get_key_mapping(void)
{
    return current_key_mapping_index;
}
