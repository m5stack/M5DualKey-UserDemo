#ifndef CHAIN_BUS_CONFIG_H
#define CHAIN_BUS_CONFIG_H

#include "esp_err.h"
#include <stdint.h>
#include "M5Chain.h"
#include "chain_bus.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHAIN_UID_SIZE 12  // 使用12字节UID

// 设备配置存储结构
typedef struct {
    uint8_t uid[CHAIN_UID_SIZE];  // 设备UID
    chain_device_type_t device_type;  // 设备类型
    chain_device_hid_config_t hid_config;  // HID配置
    uint32_t crc32;  // CRC32校验
} chain_device_saved_config_t;

/**
 * @brief 初始化配置存储系统
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_config_init(void);

/**
 * @brief 保存设备配置到NVS
 * @param uid 设备UID
 * @param device_type 设备类型
 * @param hid_config HID配置
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_config_save(const uint8_t* uid, chain_device_type_t device_type, 
                                 const chain_device_hid_config_t* hid_config);

/**
 * @brief 从NVS加载设备配置
 * @param uid 设备UID
 * @param device_type 设备类型（用于验证）
 * @param hid_config 返回的HID配置
 * @return esp_err_t ESP_OK 成功，ESP_ERR_NOT_FOUND 未找到配置
 */
esp_err_t chain_bus_config_load(const uint8_t* uid, chain_device_type_t device_type,
                                 chain_device_hid_config_t* hid_config);

/**
 * @brief 删除设备配置
 * @param uid 设备UID
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_config_delete(const uint8_t* uid);

/**
 * @brief 清空所有设备配置
 * @return esp_err_t ESP_OK 成功
 */
esp_err_t chain_bus_config_clear_all(void);

/**
 * @brief 获取已保存的配置数量
 * @return int 配置数量
 */
int chain_bus_config_get_count(void);

/**
 * @brief 将UID转换为字符串（用于日志和显示）
 * @param uid UID数据
 * @param str_buf 字符串缓冲区（至少25字节）
 */
void chain_bus_uid_to_string(const uint8_t* uid, char* str_buf);

#ifdef __cplusplus
}
#endif

#endif // CHAIN_BUS_CONFIG_H

