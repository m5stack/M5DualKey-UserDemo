#include "chain_bus_config.h"
#include "chain_bus.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "string.h"
#include "esp_crc.h"

static const char* TAG           = "chain_config";
static const char* NVS_NAMESPACE = "chain_cfg";

static nvs_handle_t s_nvs_handle = 0;
static bool config_initialized   = false;

// 初始化配置存储系统
esp_err_t chain_bus_config_init(void)
{
    if (config_initialized) {
        return ESP_OK;
    }

    esp_err_t ret = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &s_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "打开NVS命名空间失败: %s", esp_err_to_name(ret));
        return ret;
    }

    config_initialized = true;
    ESP_LOGI(TAG, "配置存储系统初始化成功");
    return ESP_OK;
}

// 将UID转换为字符串
void chain_bus_uid_to_string(const uint8_t* uid, char* str_buf)
{
    if (!uid || !str_buf) {
        return;
    }

    sprintf(str_buf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5],
            uid[6], uid[7], uid[8], uid[9], uid[10], uid[11]);
}

// 生成NVS键名
static void generate_nvs_key(const uint8_t* uid, char* key_buf)
{
    // 使用UID的前6字节生成 12 字符的键名
    sprintf(key_buf, "c_%02X%02X%02X%02X%02X%02X", uid[0], uid[1], uid[2], uid[3], uid[4], uid[5]);
}

// 计算配置的CRC32
static uint32_t calculate_config_crc(const chain_device_saved_config_t* config)
{
    return esp_crc32_le(0, (const uint8_t*)config, sizeof(chain_device_saved_config_t) - sizeof(uint32_t));
}

// 保存设备配置到NVS
esp_err_t chain_bus_config_save(const uint8_t* uid, chain_device_type_t device_type,
                                const chain_device_hid_config_t* hid_config, uint32_t rgb_color)
{
    if (!config_initialized) {
        ESP_LOGE(TAG, "配置系统未初始化");
        return ESP_ERR_INVALID_STATE;
    }

    if (!uid || !hid_config) {
        return ESP_ERR_INVALID_ARG;
    }

    chain_device_saved_config_t saved_config;
    memset(&saved_config, 0, sizeof(saved_config));
    memcpy(saved_config.uid, uid, CHAIN_UID_SIZE);
    saved_config.device_type = device_type;
    memcpy(&saved_config.hid_config, hid_config, sizeof(chain_device_hid_config_t));
    saved_config.rgb_color = rgb_color;
    saved_config.crc32 = calculate_config_crc(&saved_config);

    char nvs_key[16];
    generate_nvs_key(uid, nvs_key);

    esp_err_t ret = nvs_set_blob(s_nvs_handle, nvs_key, &saved_config, sizeof(saved_config));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "保存配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = nvs_commit(s_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    char uid_str[25];
    chain_bus_uid_to_string(uid, uid_str);
    ESP_LOGI(TAG, "设备配置已保存 [UID:%s]", uid_str);

    return ESP_OK;
}

// 从NVS加载设备配置
esp_err_t chain_bus_config_load(const uint8_t* uid, chain_device_type_t device_type,
                                chain_device_hid_config_t* hid_config, uint32_t* rgb_color)
{
    if (!config_initialized) {
        ESP_LOGE(TAG, "配置系统未初始化");
        return ESP_ERR_INVALID_STATE;
    }

    if (!uid || !hid_config || !rgb_color) {
        return ESP_ERR_INVALID_ARG;
    }

    char nvs_key[16];
    generate_nvs_key(uid, nvs_key);

    chain_device_saved_config_t saved_config;
    size_t required_size = sizeof(saved_config);

    esp_err_t ret = nvs_get_blob(s_nvs_handle, nvs_key, &saved_config, &required_size);
    if (ret == ESP_ERR_NVS_NOT_FOUND) {
        char uid_str[25];
        chain_bus_uid_to_string(uid, uid_str);
        ESP_LOGD(TAG, "设备配置未找到 [UID:%s]", uid_str);
        return ESP_ERR_NOT_FOUND;
    }

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "读取配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    // 验证CRC
    uint32_t calculated_crc = calculate_config_crc(&saved_config);
    if (calculated_crc != saved_config.crc32) {
        ESP_LOGE(TAG, "配置CRC校验失败");
        return ESP_ERR_INVALID_CRC;
    }

    // 验证设备类型
    if (saved_config.device_type != device_type) {
        char uid_str[25];
        chain_bus_uid_to_string(uid, uid_str);
        ESP_LOGW(TAG, "设备类型不匹配 [UID:%s] 保存:%d 当前:%d", uid_str, saved_config.device_type, device_type);
        return ESP_ERR_INVALID_ARG;
    }

    memcpy(hid_config, &saved_config.hid_config, sizeof(chain_device_hid_config_t));
    *rgb_color = saved_config.rgb_color;

    char uid_str[25];
    chain_bus_uid_to_string(uid, uid_str);
    ESP_LOGI(TAG, "设备配置已加载 [UID:%s]", uid_str);

    return ESP_OK;
}

// 删除设备配置
esp_err_t chain_bus_config_delete(const uint8_t* uid)
{
    if (!config_initialized) {
        ESP_LOGE(TAG, "配置系统未初始化");
        return ESP_ERR_INVALID_STATE;
    }

    if (!uid) {
        return ESP_ERR_INVALID_ARG;
    }

    char nvs_key[16];
    generate_nvs_key(uid, nvs_key);

    esp_err_t ret = nvs_erase_key(s_nvs_handle, nvs_key);
    if (ret != ESP_OK && ret != ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE(TAG, "删除配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = nvs_commit(s_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交删除失败: %s", esp_err_to_name(ret));
        return ret;
    }

    char uid_str[25];
    chain_bus_uid_to_string(uid, uid_str);
    ESP_LOGI(TAG, "设备配置已删除 [UID:%s]", uid_str);

    return ESP_OK;
}

// 清空所有设备配置
esp_err_t chain_bus_config_clear_all(void)
{
    if (!config_initialized) {
        ESP_LOGE(TAG, "配置系统未初始化");
        return ESP_ERR_INVALID_STATE;
    }

    esp_err_t ret = nvs_erase_all(s_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "清空配置失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ret = nvs_commit(s_nvs_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "提交清空失败: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "所有设备配置已清空");
    return ESP_OK;
}

// 获取已保存的配置数量
int chain_bus_config_get_count(void)
{
    if (!config_initialized) {
        return 0;
    }

    nvs_iterator_t it = NULL;
    esp_err_t ret     = nvs_entry_find(NVS_DEFAULT_PART_NAME, NVS_NAMESPACE, NVS_TYPE_BLOB, &it);

    int count = 0;
    while (ret == ESP_OK) {
        count++;
        ret = nvs_entry_next(&it);
    }

    nvs_release_iterator(it);
    return count;
}
