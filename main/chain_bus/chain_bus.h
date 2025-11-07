#ifndef CHAIN_BUS_H
#define CHAIN_BUS_H

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "M5Chain.h"
#include "chain_bus_hid.h"

// UART测试配置
#define UART_TEST_TXD1 48  // UART1 TX pin
#define UART_TEST_RXD1 47  // UART1 RX pin
#define UART_TEST_TXD2 6   // UART2 TX pin
#define UART_TEST_RXD2 5   // UART2 RX pin

#define UART_TEST_BAUD_RATE 115200
#define UART_TEST_BUF_SIZE  1024

// Chain总线配置
#define CHAIN_BUS_SCAN_INTERVAL_MS      60000  // 设备扫描间隔 60秒
#define CHAIN_BUS_POLL_INTERVAL_MS      100    // 数据轮询间隔 200ms (优化性能)
#define CHAIN_BUS_POLL_INTERVAL_FAST_MS 5      // 快速轮询间隔 5ms (摇杆等需要快速响应的设备)
#define CHAIN_BUS_EVENT_CHECK_MS        100    // 事件检查间隔 100ms (优化性能)
#define CHAIN_BUS_DEVICE_TIMEOUT_MS     50     // 设备操作超时 50ms (减少等待时间)

// Chain设备数据联合体（根据设备类型存储不同数据）
typedef union {
    struct {
        uint8_t button_status;
    } key_data;

    struct {
        pir_detect_result_t detect_status;
        uint16_t trigger_count;
    } pir_data;

    struct {
        int16_t x_value;
        int16_t y_value;
        uint8_t button_status;
    } joystick_data;

    struct {
        int16_t encoder_value;
        int16_t last_encoder_value;
        uint8_t button_status;
    } encoder_data;

    struct {
        uint16_t distance;
        uint16_t last_distance;
    } tof_data;

    struct {
        uint16_t angle_value;
        uint16_t last_angle_value;
    } angle_data;

    struct {
        switch_status_type_t switch_status;
        uint16_t switch_count;
    } switch_data;

    struct {
        uint8_t reserved[16];  // 为其他设备类型预留空间
    } generic_data;
} chain_device_data_t;

// 设备HID配置结构

// KEY设备HID配置 - 只有按键事件
typedef struct {
    hid_func_type_t single_click;   // 单击功能
    hid_func_type_t double_click;   // 双击功能
    hid_func_type_t long_press;     // 长按功能
    hid_func_type_t press_down;     // 按下功能
    hid_func_type_t press_release;  // 释放功能
} chain_key_hid_config_t;

// JOYSTICK设备HID配置 - 按键事件 + XY轴状态
typedef struct {
    // 按键事件配置
    hid_func_type_t single_click;   // 单击功能
    hid_func_type_t double_click;   // 双击功能
    hid_func_type_t long_press;     // 长按功能
    hid_func_type_t press_down;     // 按下功能
    hid_func_type_t press_release;  // 释放功能

    // XY轴配置
    hid_func_type_t xy_move_func;  // XY轴移动功能类型
    bool xy_move_reverse;          // XY轴移动反向
} chain_joystick_hid_config_t;

// ENCODER设备HID配置 - 按键事件 + 编码器状态
typedef struct {
    // 按键事件配置 (通常不使用)
    hid_func_type_t single_click;   // 单击功能
    hid_func_type_t double_click;   // 双击功能
    hid_func_type_t long_press;     // 长按功能
    hid_func_type_t press_down;     // 按下功能
    hid_func_type_t press_release;  // 释放功能

    // 编码器旋转配置
    hid_func_type_t rotate_cw_func;   // 顺时针旋转功能
    hid_func_type_t rotate_ccw_func;  // 逆时针旋转功能
} chain_encoder_hid_config_t;

// ANGLE设备HID配置 - 只有ADC状态值，无事件
typedef struct {
    hid_func_type_t angle_func;  // 角度变化功能（通常为音量控制）
} chain_angle_hid_config_t;

// 通用设备HID配置联合体
typedef union {
    chain_key_hid_config_t key_config;
    chain_joystick_hid_config_t joystick_config;
    chain_encoder_hid_config_t encoder_config;
    chain_angle_hid_config_t angle_config;
} chain_device_hid_config_t;

// Chain设备状态
typedef struct {
    uint8_t id;
    chain_device_type_t type;
    bool connected;
    uint32_t last_poll_time;
    // last_update_time 已移除，时间由前端记录

    // 设备唯一标识
    uint8_t uid[12];  // 设备UID (12字节)
    bool uid_valid;   // UID是否有效

    // 动态轮询优化
    uint8_t poll_priority;          // 轮询优先级 (0=最高, 255=最低)
    uint8_t consecutive_no_change;  // 连续无变化次数 (用于降低轮询频率)
    bool needs_fast_poll;           // 是否需要快速轮询 (摇杆等实时设备)

    // RGB状态
    uint32_t rgb_color;  // 当前RGB颜色 (0xRRGGBB)
    bool rgb_updated;    // RGB颜色是否有更新
    bool rgb_setting;    // RGB是否正在设置中

    // 设备数据
    chain_device_data_t device_data;  // 设备特定数据
    bool data_updated;                // 数据是否有更新

    // HID映射配置
    chain_device_hid_config_t hid_config;  // HID功能配置
    bool hid_config_initialized;           // 是否已初始化HID配置

    // 事件记录字段
    uint32_t last_event_time;  // 最后事件时间
    char last_event_desc[64];  // 最后事件描述
    bool event_updated;        // 事件是否有更新（用于前端显示）
    bool status_updated;       // 状态是否有更新（用于减少通信）
    bool communication_flag;   // 通信是否成功
} chain_device_status_t;

// Chain总线状态
typedef struct {
    device_list_t* device_list;
    chain_device_status_t* device_status;
    uint16_t device_count;
    uint32_t last_scan_time;
    bool initialized;
} chain_bus_status_t;

// 函数声明
void chain_bus_init(void);
void chain_bus_scan_devices(void);
void chain_bus_poll_data(void);
void chain_bus_check_events(void);
const char* chain_device_type_name(chain_device_type_t type);
void chain_bus_scan_single(int bus_idx);

// Chain设备RGB控制函数
esp_err_t chain_bus_set_device_rgb(int bus_idx, uint8_t device_id, uint32_t rgb_color);
esp_err_t chain_bus_find_device_index(int bus_idx, uint8_t device_id, int* device_index);

// Chain设备HID配置函数
/**
 * @brief 初始化设备默认HID配置
 * @param device_status 设备状态指针
 */
void chain_bus_init_device_hid_config(chain_device_status_t* device_status, int bus_idx);

/**
 * @brief 设置设备HID配置
 * @param bus_idx 总线索引
 * @param device_id 设备ID
 * @param config 设备配置
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_set_device_hid_config(int bus_idx, uint8_t device_id, const chain_device_hid_config_t* config);

/**
 * @brief 获取设备HID配置
 * @param bus_idx 总线索引
 * @param device_id 设备ID
 * @param config 返回的配置指针
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_get_device_hid_config(int bus_idx, uint8_t device_id, chain_device_hid_config_t** config);

/**
 * @brief 保存设备当前HID配置到NVS
 * @param bus_idx 总线索引
 * @param device_id 设备ID
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_save_device_config(int bus_idx, uint8_t device_id);

/**
 * @brief 获取设备UID字符串
 * @param bus_idx 总线索引
 * @param device_id 设备ID
 * @param uid_str UID字符串缓冲区（至少25字节）
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_get_device_uid_string(int bus_idx, uint8_t device_id, char* uid_str);

// Chain Bus状态 (从chain_bus.cpp获取实际数据)
extern chain_bus_status_t bus_status[2];

#include "chain_bus_config.h"

#ifdef __cplusplus
}
#endif

#endif  // CHAIN_BUS_H
