/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "keyboard.h"
#include "keycodes.h"
#include "quantum_keycodes.h"

#ifdef __cplusplus
extern "C" {
#endif

static uint16_t keymaps[][1][KBD_COL_NUM] = {
    /*
    * ┌────────┬────────┐
    * │CTRL + C│CTRL + V│
    * └────────┴────────┘
    */
    [0] = {
        {LCTL(HID_KEY_C),  LCTL(HID_KEY_V)},
    },
    [1] = {
        {KC_PAGE_UP, KC_PAGE_DOWN},
    },
    [2] = {
        {KC_KB_VOLUME_UP, KC_KB_VOLUME_DOWN},
    },
    [3] = {
        {KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK},
    },
};

#ifdef __cplusplus
}
#endif
