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
        {LCMD(HID_KEY_C), LCMD(HID_KEY_V)},
    },
    [2] = {
        {LCTL(HID_KEY_Z),  LCTL(HID_KEY_Y)},
    },
    [3] = {
        {LCMD(HID_KEY_Z), LCMD(HID_KEY_Y)},
    },
    [4] = {
        {LCMD(HID_KEY_Z), LSG(HID_KEY_Z)},
    },
    [5] = {
        {LCTL(KC_TAB),  LCS(KC_TAB)},
    },
    [6] = {
        {QK_LALT, KC_TAB},
    },
    [7] = {
        {QK_LGUI, KC_TAB},
    },
    [8] = {
        {LCTL(KC_KP_MINUS),  LCTL(KC_KP_PLUS)},
    },
    [9] = {
        {LCMD(KC_KP_MINUS), LCMD(KC_KP_PLUS)},
    },
    [10] = {
        {KC_PAGE_UP, KC_PAGE_DOWN},
    },
    [11] = {
        {KC_KB_VOLUME_UP, KC_KB_VOLUME_DOWN},
    },
    [12] = {
        {KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK},
    },
    [13] = {
        {KC_MEDIA_PLAY_PAUSE, KC_MEDIA_STOP},
    },
    [14] = {
        {KC_HOME, KC_END},
    },
    [15] = {
        {KC_UP, KC_DOWN},
    },
    [16] = {
        {KC_LEFT, KC_RIGHT},
    },
};

#ifdef __cplusplus
}
#endif
