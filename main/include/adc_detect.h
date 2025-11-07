/* SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_err.h"
#include "esp_sleep.h"
#include "esp_pm.h"
#include "led_strip.h"
#include "esp_adc/adc_oneshot.h"
#ifdef __cplusplus
extern "C" {
#endif

void test_light_sleep_button_wakeup(led_strip_handle_t led_strip);
void test_deep_sleep_button_wakeup(led_strip_handle_t led_strip);
void test_uart_wakeup(led_strip_handle_t led_strip);

// UART通信测试函数
void test_uart_communication(led_strip_handle_t led_strip);

// ADC检测测试函数
void test_adc_detection(led_strip_handle_t led_strip, adc_oneshot_unit_handle_t adc_handle);

// 低功耗测试函数
void low_power_test(void);

// 更新电源状态函数
void update_power_status(led_strip_handle_t led_strip);

// 全局电池状态变量
extern float g_battery_voltage;
extern int g_charging_status;
extern int g_battery_percentage;
extern bool g_usb_connected;

#ifdef __cplusplus
}
#endif