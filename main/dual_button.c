/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "button_interface.h"
#include "dual_button.h"

static const char *TAG = "dual_button";

typedef struct {
    button_driver_t base;          /**< button driver */
    int32_t gpio_num[2];           /**< num of gpio */
    uint8_t active_level;          /**< gpio level when press down */
} button_dual_obj;

static uint8_t button_dual_get_key_level(button_driver_t *button_driver)
{
    button_dual_obj *btn = __containerof(button_driver, button_dual_obj, base);
    int level[2] = {0};
    level[0] = gpio_get_level(btn->gpio_num[0]);
    level[1] = gpio_get_level(btn->gpio_num[1]);
    if (level[0] == btn->active_level && level[1] == btn->active_level) {
        return 1;
    }
    return 0;
}

esp_err_t iot_button_new_dual_button_device(const button_config_t *button_config, const button_dual_config_t *dual_btn_cfg, button_handle_t *ret_button)
{
    button_dual_obj *obj = NULL;
    esp_err_t ret = ESP_OK;
    ESP_GOTO_ON_FALSE(button_config && dual_btn_cfg && ret_button, ESP_ERR_INVALID_ARG, err, TAG, "Invalid argument");
    ESP_GOTO_ON_FALSE(dual_btn_cfg->skip_gpio_init, ESP_ERR_INVALID_ARG, err, TAG, "Not support dual button init yet");
    obj = (button_dual_obj *)calloc(1, sizeof(button_dual_obj));
    ESP_GOTO_ON_FALSE(obj, ESP_ERR_NO_MEM, err, TAG, "No memory for gpio button");
    memcpy(&obj->gpio_num, &dual_btn_cfg->gpio_num, sizeof(obj->gpio_num));
    obj->active_level = dual_btn_cfg->active_level;

    obj->base.get_key_level = button_dual_get_key_level;

    ret = iot_button_create(button_config, &obj->base, ret_button);
    ESP_GOTO_ON_FALSE(ret == ESP_OK, ESP_FAIL, err, TAG, "Create button failed");
    return ESP_OK;
err:
    if (obj) {
        free(obj);
    }
    return ret;
}
