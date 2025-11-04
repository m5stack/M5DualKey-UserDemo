/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*!< Keyboard active level */
#define KBD_ATTIVE_LEVEL      0
#define KBD_TICKS_INTERVAL_US 1000

/*!< Keyboard matrix GPIO */
#define KBD_COL_NUM  2

#define KBD_COL_IO_0 17
#define KBD_COL_IO_1 0

#define KBD_INPUT_IOS  {KBD_COL_IO_1, KBD_COL_IO_0}

/*!< WS2812 power control GPIO */
#define KBD_WS2812_POWER_IO    (40)
#define KBD_WS2812_GPIO        (21)

/*!< Mode switch monitor GPIO */
#define KBD_ADC_SWITCH_BLE_IO           (8)
#define KBD_ADC_SWITCH_BLE_CHAN         (ADC_CHANNEL_7)
#define KBD_ADC_SWITCH_RAINMAKER_IO     (7)
#define KBD_ADC_SWITCH_RAINMAKER_CHAN   (ADC_CHANNEL_6)

/*!< Battery monitor GPIO */
#define KBD_ADC_BATTERY_MONITOR_IO      (10)
#define KBD_ADC_BATTERY_MONITOR_CHAN    (ADC_CHANNEL_9)

/*  NO Charge: 3.3v.
 *  Charging: 1.65v.
 *  Full: 2.2v.
 */
#define KBD_ADC_CHARGE_MONITOR_IO       (9)
#define KBD_ADC_CHARGE_MONITOR_CHAN     (ADC_CHANNEL_8)

/*!<  */

#ifdef __cplusplus
}
#endif
