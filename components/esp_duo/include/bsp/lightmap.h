/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include "stdint.h"
#include "rgb_matrix_types.h"
#include "bsp/keyboard.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LIGHTMAP_GPIO            KBD_WS2812_GPIO
#define LIGHTMAP_NUM             2

led_config_t g_led_config = {{
        // Key Matrix to LED Index
        {1, 0}
    }, {
        // LED Index to Physical Position
        {0, 0}, {32, 0}
    }, {
        // LED Index to Flag
        4, 4
    }
};

#ifdef __cplusplus
}
#endif
