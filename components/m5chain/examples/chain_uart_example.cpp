/*
 * ESP-IDF M5Chain库使用示例
 * 
 * 此示例展示如何在ESP-IDF项目中使用重构后的M5Chain库
 * 基于您提供的uart_manager参考，使用ESP-IDF原生UART驱动
 */

#include "M5Chain.h"
#include "esp_log.h"
 
static const char *TAG = "chain_example";
 
// 创建Chain对象
Chain chain;
 
extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "=== M5Chain ESP-IDF 示例开始 ===");
    
    // 初始化Chain - 使用UART1，波特率115200，参考uart_manager的引脚配置
    esp_err_t ret = chain.begin(UART_NUM_1, 115200, 48, 47);  // TX=48, RX=47
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Chain初始化失败: %s", esp_err_to_name(ret));
        return;
    }
    
    ESP_LOGI(TAG, "Chain初始化成功");
    
    // 检查设备连接
    ESP_LOGI(TAG, "检查设备连接...");
    if (chain.isDeviceConnected(3, 100)) {
        ESP_LOGI(TAG, "Chain设备已连接");
        
        // 获取设备数量
        uint16_t deviceNum = 0;
        if (chain.getDeviceNum(&deviceNum, 1000) == CHAIN_OK) {
            ESP_LOGI(TAG, "检测到 %d 个设备", deviceNum);
            
            // 遍历所有设备，获取设备类型
            for (uint16_t i = 1; i <= deviceNum; i++) {
                chain_device_type_t deviceType;
                if (chain.getDeviceType(i, &deviceType, 100) == CHAIN_OK) {
                    ESP_LOGI(TAG, "设备 %d 类型: 0x%04X", i, deviceType);
                }
            }
        }
        
        // 示例：设置RGB LED（如果有支持的设备）
        uint8_t rgbData[] = {255, 0, 0, 0, 255, 0, 0, 0, 255}; // 红、绿、蓝
        uint8_t operationStatus;
        
        chain_status_t status = chain.setRGBValue(1, 0, 3, rgbData, sizeof(rgbData), &operationStatus, 500);
        if (status == CHAIN_OK && operationStatus == 1) {
            ESP_LOGI(TAG, "RGB设置成功");
        } else {
            ESP_LOGW(TAG, "RGB设置失败: status=%d, operation=%d", status, operationStatus);
        }
        
    } else {
        ESP_LOGW(TAG, "未检测到Chain设备");
    }
    
    // 主循环 - 持续监听来自设备的数据
    ESP_LOGI(TAG, "开始主循环，监听设备数据...");
    while (1) {
        // 处理来自设备的主动数据
        chain.processIncomingData();
        
        // 检查是否有枚举请求
        uint16_t enumCount = chain.getEnumPleaseNum();
        if (enumCount > 0) {
            ESP_LOGI(TAG, "收到 %d 个枚举请求", enumCount);
        }
        
        // 延时
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}