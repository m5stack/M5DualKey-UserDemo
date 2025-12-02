#include "chain_bus.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "esp_timer.h"

static const char* TAG = "chain_bus";

// Chain实例 - 左右两条总线
Chain chain_left;
Chain chain_right;

// 总线状态
chain_bus_status_t bus_status[2] = {0};  // 0:左总线, 1:右总线
static Chain* chains[]           = {&chain_left, &chain_right};
static const char* bus_names[]   = {"LEFT", "RIGHT"};

extern QueueHandle_t status_refresh_queue;

typedef enum {
    STATUS_REFRESH_NORMAL,    // 正常刷新
    STATUS_REFRESH_IMMEDIATE  // 立即刷新
} status_refresh_type_t;

// 获取设备类型名称
const char* chain_device_type_name(chain_device_type_t type)
{
    switch (type) {
        case CHAIN_ENCODER_TYPE_CODE:
            return "ENCODER";
        case CHAIN_ANGLE_TYPE_CODE:
            return "ANGLE";
        case CHAIN_KEY_TYPE_CODE:
            return "KEY";
        case CHAIN_JOYSTICK_TYPE_CODE:
            return "JOYSTICK";
        case CHAIN_TOF_TYPE_CODE:
            return "TOF";
        case CHAIN_UART_TYPE_CODE:
            return "UART";
        case CHAIN_SWITCH_TYPE_CODE:
            return "SWITCH";
        case CHAIN_PEDAL_TYPE_CODE:
            return "PEDAL";
        case CHAIN_MIC_TYPE_CODE:
            return "MIC";
        case CHAIN_PIR_TYPE_CODE:
            return "PIR";
        default:
            return "UNKNOWN";
    }
}

// 扫描指定总线的设备
void chain_bus_scan_single(int bus_idx)
{
    Chain* chain               = chains[bus_idx];
    chain_bus_status_t* status = &bus_status[bus_idx];

    // ESP_LOGI(TAG, "[%s] 开始扫描设备...", bus_names[bus_idx]);

    // 检查设备连接
    if (!chain->isDeviceConnected(3, CHAIN_BUS_DEVICE_TIMEOUT_MS)) {
        // ESP_LOGW(TAG, "[%s] 未检测到设备连接", bus_names[bus_idx]);

        // 清理旧的设备列表
        if (status->device_list) {
            if (status->device_list->devices) {
                free(status->device_list->devices);
            }
            free(status->device_list);
            status->device_list = NULL;
        }
        if (status->device_status) {
            free(status->device_status);
            status->device_status = NULL;
        }
        status->device_count = 0;
        return;
    }

    // 获取设备数量
    uint16_t device_count    = 0;
    chain_status_t chain_ret = chain->getDeviceNum(&device_count, 1000);
    if (chain_ret != CHAIN_OK) {
        ESP_LOGE(TAG, "[%s] 获取设备数量失败: %d", bus_names[bus_idx], chain_ret);
        return;
    }

    // ESP_LOGI(TAG, "[%s] 检测到 %d 个设备", bus_names[bus_idx], device_count);

    // 如果设备数量变化，重新分配内存
    if (status->device_count != device_count) {
        // 备份现有设备的HID配置
        chain_device_hid_config_t* backup_hid_configs = NULL;
        uint16_t backup_count                         = status->device_count;

        if (status->device_status && backup_count > 0) {
            backup_hid_configs = (chain_device_hid_config_t*)malloc(sizeof(chain_device_hid_config_t) * backup_count);
            if (backup_hid_configs) {
                for (uint16_t i = 0; i < backup_count; i++) {
                    memcpy(&backup_hid_configs[i], &status->device_status[i].hid_config,
                           sizeof(chain_device_hid_config_t));
                }
                ESP_LOGI(TAG, "[%s] 备份了 %d 个设备的HID配置", bus_names[bus_idx], backup_count);
            }
        }

        // 清理旧的设备列表
        if (status->device_list) {
            if (status->device_list->devices) {
                free(status->device_list->devices);
            }
            free(status->device_list);
        }
        if (status->device_status) {
            free(status->device_status);
        }

        // 分配新的设备列表
        status->device_list          = (device_list_t*)malloc(sizeof(device_list_t));
        status->device_list->count   = device_count;
        status->device_list->devices = (device_info_t*)malloc(sizeof(device_info_t) * device_count);
        status->device_status        = (chain_device_status_t*)malloc(sizeof(chain_device_status_t) * device_count);
        status->device_count         = device_count;

        // 初始化设备状态
        memset(status->device_status, 0, sizeof(chain_device_status_t) * device_count);

        // 恢复备份的HID配置
        if (backup_hid_configs) {
            uint16_t restore_count = (backup_count < device_count) ? backup_count : device_count;
            for (uint16_t i = 0; i < restore_count; i++) {
                memcpy(&status->device_status[i].hid_config, &backup_hid_configs[i], sizeof(chain_device_hid_config_t));
                status->device_status[i].hid_config_initialized = true;
            }
            free(backup_hid_configs);
            ESP_LOGI(TAG, "[%s] 恢复了 %d 个设备的HID配置", bus_names[bus_idx], restore_count);
        }

        // 新设备的HID配置标记为未初始化
        for (uint16_t i = backup_count; i < device_count; i++) {
            status->device_status[i].hid_config_initialized = false;
        }
    }

    // 获取设备列表
    if (!chain->getDeviceList(status->device_list, 2000)) {
        ESP_LOGE(TAG, "[%s] 获取设备列表失败", bus_names[bus_idx]);
        return;
    }

    // 更新设备状态
    for (uint16_t i = 0; i < device_count; i++) {
        // 如果是现有设备，先备份RGB颜色
        uint32_t backup_rgb_color = 0x000000;
        if (i < status->device_count) {
            backup_rgb_color = status->device_status[i].rgb_color;
        }

        status->device_status[i].id             = status->device_list->devices[i].id;
        status->device_status[i].type           = status->device_list->devices[i].device_type;
        status->device_status[i].connected      = true;
        status->device_status[i].event_updated  = false;
        status->device_status[i].status_updated = true;  // 新设备需要发送状态

        // 保持备份的RGB颜色（新设备为0x000000黑色）
        status->device_status[i].rgb_color = backup_rgb_color;

        status->device_status[i].rgb_updated        = false;
        status->device_status[i].rgb_setting        = false;
        status->device_status[i].data_updated       = false;
        status->device_status[i].communication_flag = true;

        // 获取设备UID
        uint8_t uid[12]          = {0};
        uint8_t operation_status = 0;
        chain_status_t uid_ret   = chain->getUID(status->device_status[i].id, UID_TYPE_12_BYTE, uid, 12,
                                                 &operation_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
        if (uid_ret == CHAIN_OK) {
            memcpy(status->device_status[i].uid, uid, 12);
            status->device_status[i].uid_valid = true;

            char uid_str[25];
            chain_bus_uid_to_string(uid, uid_str);
            ESP_LOGI(TAG, "[%s] 设备 %d UID: %s", bus_names[bus_idx], status->device_status[i].id, uid_str);
        } else {
            status->device_status[i].uid_valid = false;
            ESP_LOGW(TAG, "[%s] 设备 %d 获取UID失败: %d", bus_names[bus_idx], status->device_status[i].id, uid_ret);
        }

        // 初始化设备HID配置：优先加载保存的配置，否则使用默认配置
        if (!status->device_status[i].hid_config_initialized) {
            bool config_loaded = false;

            // 如果UID有效，尝试加载保存的配置
            if (status->device_status[i].uid_valid) {
                uint32_t loaded_rgb_color = 0;
                esp_err_t load_ret = chain_bus_config_load(status->device_status[i].uid, status->device_status[i].type,
                                                           &status->device_status[i].hid_config, &loaded_rgb_color);
                if (load_ret == ESP_OK) {
                    config_loaded                                   = true;
                    status->device_status[i].hid_config_initialized = true;
                    ESP_LOGI(TAG, "[%s] 设备 %d 已加载保存的配置", bus_names[bus_idx], status->device_status[i].id);
                    chain_bus_set_device_rgb(bus_idx, status->device_status[i].id, loaded_rgb_color);
                }
            }

            // 如果没有加载到配置，使用默认配置
            if (!config_loaded) {
                chain_bus_init_device_hid_config(&status->device_status[i], bus_idx);
            }
        }

        // 初始化动态轮询参数
        status->device_status[i].consecutive_no_change = 0;
        switch (status->device_status[i].type) {
            case CHAIN_JOYSTICK_TYPE_CODE:
            case CHAIN_ENCODER_TYPE_CODE:
                status->device_status[i].poll_priority   = 0;     // 最高优先级
                status->device_status[i].needs_fast_poll = true;  // 需要快速轮询
                break;
            case CHAIN_KEY_TYPE_CODE:
            case CHAIN_ANGLE_TYPE_CODE:
                status->device_status[i].poll_priority   = 1;  // 高优先级
                status->device_status[i].needs_fast_poll = false;
                break;
            case CHAIN_PIR_TYPE_CODE:
                status->device_status[i].poll_priority   = 2;  // 中优先级
                status->device_status[i].needs_fast_poll = false;
                break;
            default:
                status->device_status[i].poll_priority   = 3;  // 低优先级
                status->device_status[i].needs_fast_poll = false;
                break;
        }
        // 移除last_update_time，时间由前端记录

        ESP_LOGD(TAG, "[%s] 设备 %d: ID=%d, 类型=%s (0x%04X)", bus_names[bus_idx], i + 1, status->device_status[i].id,
                 chain_device_type_name(status->device_status[i].type), status->device_status[i].type);
    }

    ESP_LOGI(TAG, "[%s] 设备扫描完成", bus_names[bus_idx]);
}

// 扫描所有总线的设备
void chain_bus_scan_devices(void)
{
    uint32_t current_time = esp_timer_get_time() / 1000;  // 转换为毫秒

    for (int i = 0; i < 2; i++) {
        // 检查枚举请求
        uint16_t enum_count = chains[i]->getEnumPleaseNum();
        if (enum_count > 0) {
            ESP_LOGI(TAG, "[%s] 收到 %d 个枚举请求，立即扫描设备", bus_names[i], enum_count);
            chain_bus_scan_single(i);
            bus_status[i].last_scan_time = current_time;
        } else if (bus_status[i].last_scan_time == 0 ||
                   current_time - bus_status[i].last_scan_time >= CHAIN_BUS_SCAN_INTERVAL_MS) {
            // 定时扫描
            chain_bus_scan_single(i);
            bus_status[i].last_scan_time = current_time;
        }
    }
}

// 轮询指定设备的数据
void chain_bus_poll_device_data(int bus_idx, int dev_idx)
{
    Chain* chain                      = chains[bus_idx];
    chain_device_status_t* dev_status = &bus_status[bus_idx].device_status[dev_idx];

    uint32_t current_time = esp_timer_get_time() / 1000;

    // 动态轮询间隔：根据设备类型和变化频率动态调整
    uint32_t poll_interval = CHAIN_BUS_POLL_INTERVAL_MS;
    if (dev_status->needs_fast_poll) {
        poll_interval = CHAIN_BUS_POLL_INTERVAL_FAST_MS;
    }

    // 如果设备长时间无变化，降低轮询频率
    if (dev_status->consecutive_no_change > 10) {
        poll_interval *= 2;  // 降低频率
    }
    if (dev_status->consecutive_no_change > 50) {
        poll_interval *= 4;  // 进一步降低频率
    }

    if (current_time - dev_status->last_poll_time < poll_interval) {
        return;
    }

    dev_status->last_poll_time = current_time;
    // 移除last_update_time，时间由前端记录

    uint8_t operation_status = 0;
    chain_status_t status    = CHAIN_OK;
    bool data_changed        = false;

    switch (dev_status->type) {
        case CHAIN_KEY_TYPE_CODE: {
            // 获取按键状态
            uint8_t button_status = 0;
            status = chain->getKeyButtonStatus(dev_status->id, &button_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.key_data.button_status != button_status) {
                    dev_status->device_data.key_data.button_status = button_status;
                    dev_status->data_updated                       = true;
                    data_changed                                   = true;
                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }
                    hid_func_params_t params;
                    params.mouse_value.buttons = button_status == 1 ? 1 : 0;
                    hid_func_type_t func_type  = button_status == 1 ? dev_status->hid_config.key_config.press_down
                                                                    : dev_status->hid_config.key_config.press_release;
                    chain_bus_hid_trigger_function(func_type, &params);
                }
                // ESP_LOGI(TAG, "[%s] KEY设备 %d: 按键状态=%d", bus_names[bus_idx], dev_status->id, button_status);
            }
            break;
        }

        case CHAIN_PIR_TYPE_CODE: {
            // 获取PIR检测状态
            pir_detect_result_t pir_status;
            status = chain->getIRStatus(dev_status->id, &pir_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.pir_data.detect_status != pir_status) {
                    dev_status->device_data.pir_data.detect_status = pir_status;
                    dev_status->data_updated                       = true;
                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }
                }
                // ESP_LOGI(TAG, "[%s] PIR设备 %d: 检测状态=%s",
                //          bus_names[bus_idx], dev_status->id,
                //          (pir_status == CHAIN_PIR_PERSON) ? "有人" : "无人");
            }
            break;
        }

        case CHAIN_JOYSTICK_TYPE_CODE: {
            // 获取摇杆ADC值
            int8_t x_value, y_value;
            bool joystick_changed          = false;
            hid_func_params_t mouse_report = {0};
            status = chain->getJoystickMappedInt8Value(dev_status->id, &x_value, &y_value, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                // 检查XY值变化
                if (dev_status->device_data.joystick_data.x_value != x_value ||
                    dev_status->device_data.joystick_data.y_value != y_value) {
                    dev_status->device_data.joystick_data.x_value = x_value;
                    dev_status->device_data.joystick_data.y_value = y_value;
                    joystick_changed                              = true;
                    data_changed                                  = true;

                    // 触发HID摇杆移动功能 (只有实际变化时才处理)
                    if (dev_status->hid_config.joystick_config.xy_move_func != HID_FUNC_NONE) {
                        // 优化的阈值和灵敏度设置
                        const int8_t threshold  = 2;  // 增大死区，减少微小抖动
                        const float sensitivity = 1;  // 增大灵敏度，获得更好的响应

                        int8_t mouse_x = 0, mouse_y = 0;

                        if (abs(x_value) > threshold) {
                            mouse_x = (int8_t)(x_value * sensitivity);
                            mouse_x = (mouse_x > 127) ? 127 : ((mouse_x < -127) ? -127 : mouse_x);
                        }

                        if (abs(y_value) > threshold) {
                            mouse_y = (int8_t)(y_value * sensitivity);
                            mouse_y = (mouse_y > 127) ? 127 : ((mouse_y < -127) ? -127 : mouse_y);
                        }

                        if (dev_status->hid_config.joystick_config.xy_move_func == HID_FUNC_MOUSE_MOVE) {
                            if (dev_status->hid_config.joystick_config.xy_move_reverse) {
                                mouse_report.mouse_value.x = -mouse_x;  // 翻转X轴
                                mouse_report.mouse_value.y = mouse_y;
                            } else {
                                mouse_report.mouse_value.x = mouse_x;
                                mouse_report.mouse_value.y = -mouse_y;
                            }
                        } else if (dev_status->hid_config.joystick_config.xy_move_func == HID_FUNC_MOUSE_SCROLL ||
                                   dev_status->hid_config.joystick_config.xy_move_func == HID_FUNC_MOUSE_PAN) {
                            if (dev_status->hid_config.joystick_config.xy_move_reverse) {
                                mouse_report.mouse_value.wheel = -mouse_y;
                                mouse_report.mouse_value.pan   = -mouse_x;
                            } else {
                                mouse_report.mouse_value.wheel = mouse_y;
                                mouse_report.mouse_value.pan   = mouse_x;
                            }
                        }

                        chain_bus_hid_trigger_function(dev_status->hid_config.joystick_config.xy_move_func,
                                                       &mouse_report);
                    }
                }
            }

            // 检查按键状态变化
            uint8_t button_status = 0;
            status = chain->getJoystickButtonStatus(dev_status->id, &button_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK && dev_status->device_data.joystick_data.button_status != button_status) {
                dev_status->device_data.joystick_data.button_status = button_status;
                joystick_changed                                    = true;
                data_changed                                        = true;

                hid_func_params_t params;
                params.mouse_value.buttons = button_status == 1 ? 1 : 0;
                hid_func_type_t func_type  = button_status == 1 ? dev_status->hid_config.joystick_config.press_down
                                                                : dev_status->hid_config.joystick_config.press_release;
                chain_bus_hid_trigger_function(func_type, &params);
            }

            // 只有数据变化时才发送HID报告和刷新队列
            if (joystick_changed) {
                dev_status->data_updated = true;

                if (status_refresh_queue) {
                    status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                    xQueueSend(status_refresh_queue, &refresh_msg, 0);
                }
            }
            break;
        }

        case CHAIN_ENCODER_TYPE_CODE: {
            // 获取编码器值
            int16_t encoder_value;
            status = chain->getEncoderValue(dev_status->id, &encoder_value, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.encoder_data.encoder_value != encoder_value) {
                    dev_status->device_data.encoder_data.last_encoder_value =
                        dev_status->device_data.encoder_data.encoder_value;
                    dev_status->device_data.encoder_data.encoder_value = encoder_value;
                    dev_status->data_updated                           = true;
                    data_changed                                       = true;

                    // 触发HID编码器旋转功能
                    int16_t encoder_delta = encoder_value - dev_status->device_data.encoder_data.last_encoder_value;
                    if (encoder_delta != 0) {
                        hid_func_type_t hid_func = (encoder_delta > 0)
                                                       ? dev_status->hid_config.encoder_config.rotate_cw_func
                                                       : dev_status->hid_config.encoder_config.rotate_ccw_func;

                        hid_func_params_t params;
                        if (hid_func == HID_FUNC_MOUSE_SCROLL) {
                            params.mouse_value.wheel = encoder_delta;
                        } else if (hid_func == HID_FUNC_MOUSE_PAN) {
                            params.mouse_value.pan = encoder_delta;
                        } else if (hid_func == HID_FUNC_MOUSE_MOVE) {
                            params.mouse_value.x = encoder_delta;
                        }
                        chain_bus_hid_trigger_function(hid_func, &params);
                    }

                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }
                }
                // ESP_LOGI(TAG, "[%s] ENCODER设备 %d: 值=%d",
                //          bus_names[bus_idx], dev_status->id, encoder_value);
            }

            // 获取编码器按键状态
            uint8_t button_status = 0;
            status = chain->getEncoderButtonStatus(dev_status->id, &button_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.encoder_data.button_status != button_status) {
                    dev_status->device_data.encoder_data.button_status = button_status;
                    dev_status->data_updated                           = true;
                    data_changed                                       = true;

                    hid_func_params_t params;
                    params.mouse_value.buttons = button_status == 1 ? 1 : 0;
                    hid_func_type_t func_type  = button_status == 1
                                                     ? dev_status->hid_config.encoder_config.press_down
                                                     : dev_status->hid_config.encoder_config.press_release;
                    chain_bus_hid_trigger_function(func_type, &params);
                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }
                }
                // ESP_LOGI(TAG, "[%s] ENCODER设备 %d: 按键状态=%d",
                //          bus_names[bus_idx], dev_status->id, button_status);
            }
            break;
        }

        case CHAIN_TOF_TYPE_CODE: {
            // 获取距离值
            uint16_t distance;
            status = chain->getToFDistance(dev_status->id, &distance, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.tof_data.distance != distance) {
                    dev_status->device_data.tof_data.last_distance = dev_status->device_data.tof_data.distance;
                    dev_status->device_data.tof_data.distance      = distance;
                    dev_status->data_updated                       = true;
                }
                // ESP_LOGI(TAG, "[%s] TOF设备 %d: 距离=%d mm", bus_names[bus_idx], dev_status->id, distance);
            }
            break;
        }

        case CHAIN_ANGLE_TYPE_CODE: {
            // 获取角度ADC值
            uint16_t angle_value;
            status = chain->getAngle12BitAdc(dev_status->id, &angle_value, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.angle_data.angle_value != angle_value) {
                    dev_status->device_data.angle_data.last_angle_value =
                        dev_status->device_data.angle_data.angle_value;
                    dev_status->device_data.angle_data.angle_value = angle_value;
                    dev_status->data_updated                       = true;
                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }

                    // static bool is_first = true;
                    // if (is_first) {
                    //     is_first = false;
                    //     break;
                    // }
                    uint16_t volume_level = (uint16_t)(angle_value * 100 / 4095);
                    uint16_t last_volume_level =
                        (uint16_t)(dev_status->device_data.angle_data.last_angle_value * 100 / 4095);
                    int8_t pan_value = volume_level - last_volume_level;
                    if (volume_level != last_volume_level) {
                        hid_func_params_t params;
                        hid_func_type_t func_type = dev_status->hid_config.angle_config.angle_func;
                        if (func_type == HID_FUNC_MOUSE_PAN) {
                            params.mouse_value.pan = pan_value;
                        } else if (func_type == HID_FUNC_MOUSE_SCROLL) {
                            params.mouse_value.wheel = pan_value;
                        }
                        chain_bus_hid_trigger_function(func_type, &params);
                    }
                }
                // ESP_LOGI(TAG, "[%s] ANGLE设备 %d: ADC值=%d", bus_names[bus_idx], dev_status->id, angle_value);
            }
            break;
        }

        case CHAIN_SWITCH_TYPE_CODE: {
            // 获取开关状态
            switch_status_type_t switch_status;
            status = chain->getSwitchStatus(dev_status->id, &switch_status, CHAIN_BUS_DEVICE_TIMEOUT_MS);
            if (status == CHAIN_OK) {
                if (dev_status->device_data.switch_data.switch_status != switch_status) {
                    dev_status->device_data.switch_data.switch_status = switch_status;
                    dev_status->data_updated                          = true;
                    if (status_refresh_queue) {
                        status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                        xQueueSend(status_refresh_queue, &refresh_msg, 0);
                    }
                }
                const char* status_str = (switch_status == CHAIN_SWITCH_OPEN) ? "打开" : "关闭";
                // ESP_LOGI(TAG, "[%s] SWITCH设备 %d: 开关状态=%s", bus_names[bus_idx], dev_status->id, status_str);
            }
            break;
        }

        case CHAIN_PEDAL_TYPE_CODE: {
            // 获取踏板状态
            // ESP_LOGD(TAG, "[%s] PEDAL设备 %d: 轮询中",
            //          bus_names[bus_idx], dev_status->id);
            break;
        }

        case CHAIN_MIC_TYPE_CODE: {
            // 获取麦克风状态
            // ESP_LOGD(TAG, "[%s] MIC设备 %d: 轮询中",
            //          bus_names[bus_idx], dev_status->id);
            break;
        }

        default:
            // 对于其他类型的设备，只获取基本信息
            ESP_LOGD(TAG, "[%s] 设备 %d (%s): 轮询中", bus_names[bus_idx], dev_status->id,
                     chain_device_type_name(dev_status->type));
            break;
    }

    if (status != CHAIN_OK) {
        ESP_LOGW(TAG, "[%s] 设备 %d 数据轮询失败: %d", bus_names[bus_idx], dev_status->id, status);
        bus_status[bus_idx].last_scan_time                            = 0;
        bus_status[bus_idx].device_status[dev_idx].communication_flag = false;
    } else {
        bus_status[bus_idx].device_status[dev_idx].communication_flag = true;

        // 动态轮询统计：跟踪设备变化频率
        if (data_changed) {
            dev_status->consecutive_no_change = 0;  // 重置无变化计数
        } else {
            dev_status->consecutive_no_change++;
            // 防止计数器溢出
            if (dev_status->consecutive_no_change > 200) {
                dev_status->consecutive_no_change = 200;
            }
        }

        // 标记状态已更新，如果数据有变化也标记状态更新
        if (bus_status[bus_idx].device_status[dev_idx].data_updated) {
            bus_status[bus_idx].device_status[dev_idx].status_updated = true;
            bus_status[bus_idx].device_status[dev_idx].data_updated   = false;  // 重置数据更新标记
        }
    }
}

// 轮询所有设备数据
void chain_bus_poll_data(void)
{
    for (int bus_idx = 0; bus_idx < 2; bus_idx++) {
        if (!bus_status[bus_idx].initialized || bus_status[bus_idx].device_count == 0) {
            continue;
        }

        for (int dev_idx = 0; dev_idx < bus_status[bus_idx].device_count; dev_idx++) {
            if (bus_status[bus_idx].device_status[dev_idx].connected) {
                chain_bus_poll_device_data(bus_idx, dev_idx);
            }
        }
    }
}

// 检查设备事件
void chain_bus_check_events(void)
{
    for (int bus_idx = 0; bus_idx < 2; bus_idx++) {
        Chain* chain = chains[bus_idx];

        if (!bus_status[bus_idx].initialized || bus_status[bus_idx].device_count == 0) {
            continue;
        }

        // 检查各设备的事件
        for (int dev_idx = 0; dev_idx < bus_status[bus_idx].device_count; dev_idx++) {
            chain_device_status_t* dev_status = &bus_status[bus_idx].device_status[dev_idx];

            if (!dev_status->connected) {
                continue;
            }

            switch (dev_status->type) {
                case CHAIN_KEY_TYPE_CODE: {
                    // 检查按键事件
                    chain_button_press_type_t press_type;
                    while (chain->getKeyButtonPressStatus(dev_status->id, &press_type)) {
                        const char* press_str = "";
                        switch (press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                press_str = "single click";
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                press_str = "double click";
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                press_str = "long press";
                                break;
                        }

                        // 记录事件信息
                        snprintf(dev_status->last_event_desc, sizeof(dev_status->last_event_desc), "key event: %s",
                                 press_str);
                        dev_status->last_event_time = esp_timer_get_time() / 1000;
                        dev_status->event_updated   = true;
                        dev_status->status_updated  = true;

                        // 处理HID映射
                        hid_func_type_t hid_func = HID_FUNC_NONE;
                        switch (press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                hid_func = dev_status->hid_config.key_config.single_click;
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                hid_func = dev_status->hid_config.key_config.double_click;
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                hid_func = dev_status->hid_config.key_config.long_press;
                                break;
                        }
                        hid_func_params_t params;
                        params.mouse_value.buttons = 1;
                        chain_bus_hid_trigger_function(hid_func, &params);

                        if (status_refresh_queue) {
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            xQueueSend(status_refresh_queue, &refresh_msg, 0);
                        }
                        ESP_LOGI(TAG, "[%s] KEY设备 %d 事件: %s", bus_names[bus_idx], dev_status->id, press_str);
                    }
                    break;
                }

                case CHAIN_PIR_TYPE_CODE: {
                    // 检查PIR事件
                    pir_detect_report_t pir_trigger;
                    while (chain->getPIRDetectTrigger(dev_status->id, &pir_trigger)) {
                        const char* detect_str = "";
                        switch (pir_trigger) {
                            case CHAIN_PIR_REPORT_NO_PERSON:
                                detect_str = "no person";
                                break;
                            case CHAIN_PIR_REPORT_PERSON:
                                detect_str = "person";
                                break;
                        }

                        // 记录事件信息
                        snprintf(dev_status->last_event_desc, sizeof(dev_status->last_event_desc),
                                 "detection status: %s", detect_str);
                        dev_status->last_event_time = esp_timer_get_time() / 1000;
                        dev_status->event_updated   = true;
                        dev_status->status_updated  = true;

                        if (status_refresh_queue) {
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            xQueueSend(status_refresh_queue, &refresh_msg, 0);
                        }
                        ESP_LOGI(TAG, "[%s] PIR设备 %d 事件: %s", bus_names[bus_idx], dev_status->id, detect_str);
                    }
                    break;
                }

                case CHAIN_JOYSTICK_TYPE_CODE: {
                    // 检查摇杆按键事件
                    chain_button_press_type_t joy_press_type;
                    while (chain->getJoystickButtonPressStatus(dev_status->id, &joy_press_type)) {
                        const char* press_str = "";
                        switch (joy_press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                press_str = "single click";
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                press_str = "double click";
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                press_str = "long press";
                                break;
                        }

                        // 记录事件信息
                        snprintf(dev_status->last_event_desc, sizeof(dev_status->last_event_desc),
                                 "joystick button: %s", press_str);
                        dev_status->last_event_time = esp_timer_get_time() / 1000;
                        dev_status->event_updated   = true;
                        dev_status->status_updated  = true;

                        // 处理HID映射
                        hid_func_type_t hid_func = HID_FUNC_NONE;
                        switch (joy_press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                hid_func = dev_status->hid_config.joystick_config.single_click;
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                hid_func = dev_status->hid_config.joystick_config.double_click;
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                hid_func = dev_status->hid_config.joystick_config.long_press;
                                break;
                        }
                        hid_func_params_t params;
                        params.mouse_value.buttons = 1;
                        chain_bus_hid_trigger_function(hid_func, &params);

                        if (status_refresh_queue) {
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            xQueueSend(status_refresh_queue, &refresh_msg, 0);
                        }
                        ESP_LOGI(TAG, "[%s] JOYSTICK设备 %d 按键事件: %s", bus_names[bus_idx], dev_status->id,
                                 press_str);
                    }
                    break;
                }

                case CHAIN_SWITCH_TYPE_CODE: {
                    // 检查开关事件
                    switch_report_type_t switch_trigger;
                    while (chain->getSwitchTrigger(dev_status->id, &switch_trigger)) {
                        const char* slip_str = "";
                        switch (switch_trigger) {
                            case CHAIN_SWITCH_REPORT_CLOSE:
                                slip_str = "closed";
                                break;
                            case CHAIN_SWITCH_REPORT_OPEN:
                                slip_str = "opened";
                                break;
                        }

                        // 记录事件信息
                        snprintf(dev_status->last_event_desc, sizeof(dev_status->last_event_desc), "switch status: %s",
                                 slip_str);
                        dev_status->last_event_time = esp_timer_get_time() / 1000;
                        dev_status->event_updated   = true;
                        dev_status->status_updated  = true;

                        if (status_refresh_queue) {
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            xQueueSend(status_refresh_queue, &refresh_msg, 0);
                        }
                        ESP_LOGI(TAG, "[%s] SWITCH设备 %d 事件: %s", bus_names[bus_idx], dev_status->id, slip_str);
                    }
                    break;
                }

                case CHAIN_ENCODER_TYPE_CODE: {
                    // 检查编码器按键事件
                    chain_button_press_type_t press_type;
                    while (chain->getEncoderButtonPressStatus(dev_status->id, &press_type)) {
                        const char* press_str = "";
                        switch (press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                press_str = "single click";
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                press_str = "double click";
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                press_str = "long press";
                                break;
                        }

                        // 记录事件信息
                        snprintf(dev_status->last_event_desc, sizeof(dev_status->last_event_desc), "encoder button: %s",
                                 press_str);
                        dev_status->last_event_time = esp_timer_get_time() / 1000;
                        dev_status->event_updated   = true;
                        dev_status->status_updated  = true;

                        // 处理HID映射
                        hid_func_type_t hid_func = HID_FUNC_NONE;
                        switch (press_type) {
                            case CHAIN_BUTTON_PRESS_SINGLE:
                                hid_func = dev_status->hid_config.encoder_config.single_click;
                                break;
                            case CHAIN_BUTTON_PRESS_DOUBLE:
                                hid_func = dev_status->hid_config.encoder_config.double_click;
                                break;
                            case CHAIN_BUTTON_PRESS_LONG:
                                hid_func = dev_status->hid_config.encoder_config.long_press;
                                break;
                        }
                        hid_func_params_t params;
                        params.mouse_value.buttons = 1;
                        chain_bus_hid_trigger_function(hid_func, &params);

                        if (status_refresh_queue) {
                            status_refresh_type_t refresh_msg = STATUS_REFRESH_IMMEDIATE;
                            xQueueSend(status_refresh_queue, &refresh_msg, 0);
                        }
                        ESP_LOGI(TAG, "[%s] ENCODER设备 %d 按键事件: %s", bus_names[bus_idx], dev_status->id,
                                 press_str);
                    }
                    break;
                }

                case CHAIN_PEDAL_TYPE_CODE: {
                    // 踏板设备事件检查
                    ESP_LOGD(TAG, "[%s] PEDAL设备 %d: 暂不支持事件检查", bus_names[bus_idx], dev_status->id);
                    break;
                }

                case CHAIN_MIC_TYPE_CODE: {
                    // 麦克风设备事件检查
                    ESP_LOGD(TAG, "[%s] MIC设备 %d: 暂不支持事件检查", bus_names[bus_idx], dev_status->id);
                    break;
                }

                default:
                    // 其他类型设备暂时不处理事件
                    break;
            }
        }
    }
}

// 主任务循环
void chain_bus_task(void* pvParameters)
{
    ESP_LOGI(TAG, "Chain Bus任务启动");

    TickType_t last_event_check = 0;

    while (1) {
        TickType_t current_tick = xTaskGetTickCount();

        // 扫描设备 (包括处理枚举请求)
        chain_bus_scan_devices();

        // 轮询设备数据
        chain_bus_poll_data();

        // 高频检查事件
        if (current_tick - last_event_check >= pdMS_TO_TICKS(CHAIN_BUS_EVENT_CHECK_MS)) {
            chain_bus_check_events();
            last_event_check = current_tick;
        }

        // 任务延时
        vTaskDelay(pdMS_TO_TICKS(CHAIN_BUS_EVENT_CHECK_MS));
    }
}

// 初始化Chain Bus
void chain_bus_init(void)
{
    ESP_LOGI(TAG, "初始化Chain Bus...");

    // 初始化配置存储系统
    esp_err_t config_ret = chain_bus_config_init();
    if (config_ret == ESP_OK) {
        int config_count = chain_bus_config_get_count();
        ESP_LOGI(TAG, "配置存储系统初始化成功，已保存 %d 个设备配置", config_count);
    } else {
        ESP_LOGE(TAG, "配置存储系统初始化失败: %s", esp_err_to_name(config_ret));
    }

    // 初始化左总线 (UART2)
    esp_err_t ret = chain_left.begin(UART_NUM_2, UART_TEST_BAUD_RATE, UART_TEST_TXD2, UART_TEST_RXD2);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "左总线 (UART2) 初始化成功");
        bus_status[0].initialized    = true;
        bus_status[0].last_scan_time = 0;  // 立即进行首次扫描
    } else {
        ESP_LOGE(TAG, "左总线 (UART2) 初始化失败: %s", esp_err_to_name(ret));
    }

    // 初始化右总线 (UART1)
    ret = chain_right.begin(UART_NUM_1, UART_TEST_BAUD_RATE, UART_TEST_TXD1, UART_TEST_RXD1);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "右总线 (UART1) 初始化成功");
        bus_status[1].initialized    = true;
        bus_status[1].last_scan_time = 0;  // 立即进行首次扫描
    } else {
        ESP_LOGE(TAG, "右总线 (UART1) 初始化失败: %s", esp_err_to_name(ret));
    }

    // 初始化HID系统
    esp_err_t hid_ret = chain_bus_hid_init();
    if (hid_ret == ESP_OK) {
        ESP_LOGI(TAG, "Chain HID系统初始化成功");
    } else {
        ESP_LOGE(TAG, "Chain HID系统初始化失败: %s", esp_err_to_name(hid_ret));
    }

    // 创建Chain Bus任务
    BaseType_t task_ret = xTaskCreate(chain_bus_task, "chain_bus_task", 8192, NULL, 5, NULL);
    if (task_ret == pdPASS) {
        ESP_LOGI(TAG, "Chain Bus任务创建成功");
    } else {
        ESP_LOGE(TAG, "Chain Bus任务创建失败");
    }
}

// 查找设备索引
esp_err_t chain_bus_find_device_index(int bus_idx, uint8_t device_id, int* device_index)
{
    if (bus_idx < 0 || bus_idx >= 2 || device_index == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    if (!bus_status[bus_idx].initialized || bus_status[bus_idx].device_count == 0) {
        return ESP_ERR_NOT_FOUND;
    }

    for (int i = 0; i < bus_status[bus_idx].device_count; i++) {
        if (bus_status[bus_idx].device_status[i].id == device_id && bus_status[bus_idx].device_status[i].connected) {
            *device_index = i;
            return ESP_OK;
        }
    }

    return ESP_ERR_NOT_FOUND;
}

// 设置Chain设备RGB颜色
esp_err_t chain_bus_set_device_rgb(int bus_idx, uint8_t device_id, uint32_t rgb_color)
{
    if (bus_idx < 0 || bus_idx >= 2) {
        ESP_LOGE(TAG, "无效的总线索引: %d", bus_idx);
        return ESP_ERR_INVALID_ARG;
    }

    int device_index;
    esp_err_t ret = chain_bus_find_device_index(bus_idx, device_id, &device_index);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[%s] 未找到设备 ID=%d", bus_names[bus_idx], device_id);
        return ret;
    }

    chain_device_status_t* dev_status = &bus_status[bus_idx].device_status[device_index];
    Chain* chain                      = chains[bus_idx];

    // 检查是否正在设置RGB
    if (dev_status->rgb_setting) {
        ESP_LOGW(TAG, "[%s] 设备 %d RGB正在设置中，请稍后再试", bus_names[bus_idx], device_id);
        return ESP_ERR_INVALID_STATE;
    }

    // 标记RGB设置状态
    dev_status->rgb_setting = true;

    ESP_LOGI(TAG, "[%s] 开始设置设备 %d RGB颜色: 0x%06lX", bus_names[bus_idx], device_id, rgb_color);

    // 提取RGB分量
    uint8_t rgb[3] = {0};
    rgb[0]         = (rgb_color >> 16) & 0xFF;
    rgb[1]         = (rgb_color >> 8) & 0xFF;
    rgb[2]         = rgb_color & 0xFF;

    uint8_t operationStatus = 0;

    chain_status_t status = chain->setRGBValue(device_id, 0, 1, rgb, 3, &operationStatus, CHAIN_BUS_DEVICE_TIMEOUT_MS);

    // 更新设备RGB状态
    if (status == CHAIN_OK) {
        dev_status->rgb_color      = rgb_color;
        dev_status->rgb_updated    = true;
        dev_status->status_updated = true;
        ESP_LOGI(TAG, "[%s] 设备 %d RGB设置成功", bus_names[bus_idx], device_id);
        ret = ESP_OK;
    } else {
        ESP_LOGE(TAG, "[%s] 设备 %d RGB设置失败: %d", bus_names[bus_idx], device_id, status);
        ret = ESP_FAIL;
    }

    chain_bus_save_device_config(bus_idx, device_id);

    // 重置RGB设置状态
    dev_status->rgb_setting = false;

    return ret;
}

// 初始化设备默认HID配置
void chain_bus_init_device_hid_config(chain_device_status_t* device_status, int bus_idx)
{
    if (!device_status) {
        return;
    }

    // 清空配置
    memset(&device_status->hid_config, 0, sizeof(device_status->hid_config));

    switch (device_status->type) {
        case CHAIN_KEY_TYPE_CODE:
            // KEY设备默认配置: 单击复制、双击粘贴、长按全选
            device_status->hid_config.key_config.single_click  = HID_FUNC_COPY;
            device_status->hid_config.key_config.double_click  = HID_FUNC_PASTE;
            device_status->hid_config.key_config.long_press    = HID_FUNC_SELECT_ALL;
            device_status->hid_config.key_config.press_down    = HID_FUNC_NONE;
            device_status->hid_config.key_config.press_release = HID_FUNC_NONE;
            ESP_LOGI(TAG, "初始化KEY设备 %d 默认HID配置: 单击复制、双击粘贴、长按全选", device_status->id);
            break;

        case CHAIN_JOYSTICK_TYPE_CODE:
            // JOYSTICK设备默认配置: 按下鼠标左键按下，释放鼠标左键释放，XY轴映射鼠标移动
            device_status->hid_config.joystick_config.single_click  = HID_FUNC_NONE;
            device_status->hid_config.joystick_config.double_click  = HID_FUNC_NONE;
            device_status->hid_config.joystick_config.long_press    = HID_FUNC_NONE;
            device_status->hid_config.joystick_config.press_down    = HID_FUNC_MOUSE_BUTTON_RIGHT_DOWN;
            device_status->hid_config.joystick_config.press_release = HID_FUNC_MOUSE_BUTTON_RIGHT_UP;
            device_status->hid_config.joystick_config.xy_move_func  = HID_FUNC_MOUSE_MOVE;
            device_status->hid_config.joystick_config.xy_move_reverse =
                bus_idx == 0 ? true : false;  // 左总线默认翻转，右总线默认不翻转
            ESP_LOGI(TAG, "初始化JOYSTICK设备 %d 默认HID配置: 按下鼠标左键、XY轴鼠标移动", device_status->id);
            break;

        case CHAIN_ENCODER_TYPE_CODE:
            // ENCODER设备默认配置: 编码值变化映射鼠标竖向滚轮
            device_status->hid_config.encoder_config.single_click    = HID_FUNC_NONE;
            device_status->hid_config.encoder_config.double_click    = HID_FUNC_NONE;
            device_status->hid_config.encoder_config.long_press      = HID_FUNC_NONE;
            device_status->hid_config.encoder_config.press_down      = HID_FUNC_MOUSE_BUTTON_LEFT_DOWN;
            device_status->hid_config.encoder_config.press_release   = HID_FUNC_MOUSE_BUTTON_LEFT_UP;
            device_status->hid_config.encoder_config.rotate_cw_func  = HID_FUNC_MOUSE_SCROLL;  // 顺时针向下滚动
            device_status->hid_config.encoder_config.rotate_ccw_func = HID_FUNC_MOUSE_SCROLL;  // 逆时针向上滚动
            ESP_LOGI(TAG, "初始化ENCODER设备 %d 默认HID配置: 编码器旋转映射滚轮", device_status->id);
            break;

        case CHAIN_ANGLE_TYPE_CODE:
            // ANGLE设备默认配置: ADC值映射音量值
            device_status->hid_config.angle_config.angle_func = HID_FUNC_MOUSE_SCROLL;
            ESP_LOGI(TAG, "初始化ANGLE设备 %d 默认HID配置: 角度映射音量", device_status->id);
            break;

        default:
            ESP_LOGD(TAG, "设备类型 %s 无默认HID配置", chain_device_type_name(device_status->type));
            break;
    }

    device_status->hid_config_initialized = true;
}

esp_err_t chain_bus_set_device_hid_config(int bus_idx, uint8_t device_id, const chain_device_hid_config_t* config)
{
    if (bus_idx < 0 || bus_idx >= 2 || device_id == 0 || config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    Chain* chain               = chains[bus_idx];
    chain_bus_status_t* status = &bus_status[bus_idx];

    int device_index;
    esp_err_t ret = chain_bus_find_device_index(bus_idx, device_id, &device_index);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[%s] 未找到设备 ID=%d", bus_names[bus_idx], device_id);
        return ret;
    }

    switch (status->device_status[device_index].type) {
        case CHAIN_KEY_TYPE_CODE:
            memcpy(&status->device_status[device_index].hid_config.key_config, &config->key_config,
                   sizeof(chain_key_hid_config_t));
            break;
        case CHAIN_JOYSTICK_TYPE_CODE:
            memcpy(&status->device_status[device_index].hid_config.joystick_config, &config->joystick_config,
                   sizeof(chain_joystick_hid_config_t));
            break;
        case CHAIN_ENCODER_TYPE_CODE:
            memcpy(&status->device_status[device_index].hid_config.encoder_config, &config->encoder_config,
                   sizeof(chain_encoder_hid_config_t));
            break;
        case CHAIN_ANGLE_TYPE_CODE:
            memcpy(&status->device_status[device_index].hid_config.angle_config, &config->angle_config,
                   sizeof(chain_angle_hid_config_t));
            break;
        default:
            ESP_LOGE(TAG, "不支持的设备类型: %s", chain_device_type_name(status->device_status[device_index].type));
            return ESP_ERR_INVALID_ARG;
    }

    return ESP_OK;
}

esp_err_t chain_bus_get_device_hid_config(int bus_idx, uint8_t device_id, chain_device_hid_config_t** config)
{
    if (bus_idx < 0 || bus_idx >= 2 || device_id == 0 || config == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    Chain* chain               = chains[bus_idx];
    chain_bus_status_t* status = &bus_status[bus_idx];

    int device_index;
    esp_err_t ret = chain_bus_find_device_index(bus_idx, device_id, &device_index);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[%s] 未找到设备 ID=%d", bus_names[bus_idx], device_id);
        return ret;
    }

    *config = &status->device_status[device_index].hid_config;
    return ESP_OK;
}

esp_err_t chain_bus_save_device_config(int bus_idx, uint8_t device_id)
{
    if (bus_idx < 0 || bus_idx >= 2 || device_id == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    int device_index;
    esp_err_t ret = chain_bus_find_device_index(bus_idx, device_id, &device_index);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[%s] 未找到设备 ID=%d", bus_names[bus_idx], device_id);
        return ret;
    }

    chain_device_status_t* dev_status = &bus_status[bus_idx].device_status[device_index];

    if (!dev_status->uid_valid) {
        ESP_LOGE(TAG, "[%s] 设备 %d UID无效，无法保存配置", bus_names[bus_idx], device_id);
        return ESP_ERR_INVALID_STATE;
    }

    ret = chain_bus_config_save(dev_status->uid, dev_status->type, &dev_status->hid_config, dev_status->rgb_color);
    if (ret == ESP_OK) {
        char uid_str[25];
        chain_bus_uid_to_string(dev_status->uid, uid_str);
        ESP_LOGI(TAG, "[%s] 设备 %d 配置已保存 [UID:%s]", bus_names[bus_idx], device_id, uid_str);
    }

    return ret;
}

esp_err_t chain_bus_get_device_uid_string(int bus_idx, uint8_t device_id, char* uid_str)
{
    if (bus_idx < 0 || bus_idx >= 2 || device_id == 0 || uid_str == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    int device_index;
    esp_err_t ret = chain_bus_find_device_index(bus_idx, device_id, &device_index);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "[%s] 未找到设备 ID=%d", bus_names[bus_idx], device_id);
        return ret;
    }

    chain_device_status_t* dev_status = &bus_status[bus_idx].device_status[device_index];

    if (!dev_status->uid_valid) {
        strcpy(uid_str, "INVALID");
        return ESP_ERR_INVALID_STATE;
    }

    chain_bus_uid_to_string(dev_status->uid, uid_str);
    return ESP_OK;
}
