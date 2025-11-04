#pragma once

#include "esp_log.h"
#include "tinyusb_hid.h"
#include "btn_progress.h"
#include <stdint.h>
#include "usb_descriptors.h"
#include "bsp/keycodes.h"

#ifdef __cplusplus
extern "C" {
#endif

// HID功能类型枚举
typedef enum {
    HID_FUNC_NONE = 0,          // 无功能
    
    // 键盘功能类型
    HID_FUNC_COPY,              // 复制 (Ctrl+C)
    HID_FUNC_PASTE,             // 粘贴 (Ctrl+V)
    HID_FUNC_CUT,               // 剪切 (Ctrl+X)
    HID_FUNC_UNDO,              // 撤销 (Ctrl+Z)
    HID_FUNC_REDO,              // 重做 (Ctrl+Y)
    HID_FUNC_SELECT_ALL,        // 全选 (Ctrl+A)
    HID_FUNC_DELETE,            // 删除 (Delete)
    HID_FUNC_BACKSPACE,         // 退格 (Backspace)
    HID_FUNC_ENTER,             // 回车 (Enter)
    HID_FUNC_ESC,               // Esc
    HID_FUNC_TAB,               // Tab
    HID_FUNC_PAGE_UP,           // 上翻页
    HID_FUNC_PAGE_DOWN,         // 下翻页
    
    // 鼠标功能类型
    HID_FUNC_MOUSE_BUTTON_LEFT_DOWN, // 鼠标左键按下
    HID_FUNC_MOUSE_BUTTON_LEFT_UP,   // 鼠标左键释放
    HID_FUNC_MOUSE_BUTTON_RIGHT_DOWN, // 鼠标右键按下
    HID_FUNC_MOUSE_BUTTON_RIGHT_UP,   // 鼠标右键释放
    HID_FUNC_MOUSE_MOVE,        // 鼠标移动
    HID_FUNC_MOUSE_MOVE_LEFT,   // 鼠标向左移动
    HID_FUNC_MOUSE_MOVE_RIGHT,  // 鼠标向右移动
    HID_FUNC_MOUSE_MOVE_UP,     // 鼠标向上移动
    HID_FUNC_MOUSE_MOVE_DOWN,   // 鼠标向下移动
    HID_FUNC_MOUSE_SCROLL,      // 鼠标竖向滚轮
    HID_FUNC_MOUSE_SCROLL_UP,   // 鼠标竖向滚轮向上
    HID_FUNC_MOUSE_SCROLL_DOWN, // 鼠标竖向滚轮向下
    HID_FUNC_MOUSE_PAN,         // 鼠标横向滚轮
    HID_FUNC_MOUSE_PAN_LEFT,    // 鼠标横向滚轮向左
    HID_FUNC_MOUSE_PAN_RIGHT,   // 鼠标横向滚轮向右
    
    // 音量功能类型
    HID_FUNC_VOLUME_UP,         // 音量增加
    HID_FUNC_VOLUME_DOWN,       // 音量减少
    HID_FUNC_VOLUME_MUTE,       // 静音切换
    
    // 媒体控制功能类型
    HID_FUNC_MEDIA_PLAY_PAUSE,  // 播放/暂停
    HID_FUNC_MEDIA_NEXT,        // 下一首
    HID_FUNC_MEDIA_PREV,        // 上一首
    HID_FUNC_MEDIA_STOP,        // 停止
    
    HID_FUNC_MAX
} hid_func_type_t;

// HID功能参数结构
typedef struct {
    uint8_t buttons; /**< buttons mask for currently pressed buttons in the mouse. */
    int8_t  x;       /**< Current delta x movement of the mouse. */
    int8_t  y;       /**< Current delta y movement on the mouse. */
    int8_t  wheel;   /**< Current delta wheel movement on the mouse. */
    int8_t  pan;     // using AC Pan
} mouse_report_t;

typedef struct {
    mouse_report_t mouse_value;// 鼠标移动量
    uint8_t volume_level;       // 音量等级 (0-100)
} hid_func_params_t;

// HID功能描述表（类似keymap.h的形式）
typedef struct {
    hid_func_type_t func_type;
    const char* name;
    const char* description;
} hid_func_desc_t;

// HID功能表声明 (定义在.cpp文件中)
extern const hid_func_desc_t hid_func_table[];

#define HID_FUNC_TABLE_SIZE (sizeof(hid_func_table) / sizeof(hid_func_table[0]))

// 函数声明

/**
 * @brief 初始化Chain HID系统
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_init(void);

/**
 * @brief 启用/禁用HID映射
 * @param enabled 是否启用
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_set_enabled(bool enabled);

/**
 * @brief 获取HID映射启用状态
 * @return bool 是否启用
 */
bool chain_bus_hid_is_enabled(void);

/**
 * @brief 设置HID报告类型
 * @param report_type 报告类型
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_set_report_type(btn_report_type_t report_type);

/**
 * @brief 设置摇杆参数
 * @param sensitivity 灵敏度
 * @param threshold 死区阈值
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_set_joystick_params(float sensitivity, uint16_t threshold);

/**
 * @brief 设置编码器灵敏度
 * @param sensitivity 灵敏度
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_set_encoder_sensitivity(float sensitivity);

/**
 * @brief 统一的HID功能触发接口
 * @param func_type 功能类型
 * @param params 功能参数（可为NULL）
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_trigger_function(hid_func_type_t func_type, hid_func_params_t* params);

/**
 * @brief 获取HID功能描述
 * @param func_type 功能类型
 * @return const hid_func_desc_t* 功能描述，NULL表示无效
 */
const hid_func_desc_t* chain_bus_hid_get_func_desc(hid_func_type_t func_type);

/**
 * @brief 获取所有可用HID功能列表
 * @param count 返回功能数量
 * @return const hid_func_desc_t* 功能描述数组
 */
const hid_func_desc_t* chain_bus_hid_get_all_functions(size_t* count);

/**
 * @brief 更新鼠标移动目标数据
 * @param x X轴移动量
 * @param y Y轴移动量
 * @param buttons 按键状态
 * @param wheel 滚轮状态
 * @param pan 平移状态
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_hid_update_mouse_movement(int16_t x, int16_t y, uint8_t buttons, int8_t wheel, int8_t pan);

#ifdef __cplusplus
}
#endif
