/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#include "M5Chain.h"

#define TXD_PIN GPIO_NUM_21  // Tx
#define RXD_PIN GPIO_NUM_22  // Rx

Chain M5Chain;

device_list_t *devices_list = NULL;
chain_status_t chain_status;
uint16_t device_nums     = 0;
uint8_t operation_status = 0;

uint8_t rgb_test[5][3] = {
    {0xFF, 0x00, 0x00}, {0x00, 0xFF, 0x00}, {0x00, 0x00, 0xFF}, {0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00},
};

void printDeviceList(device_list_t *devices);
void printChainOperationStatus(chain_status_t status);
void printWorkStatus(work_status_t gpio11_status, work_status_t gpio12_status);
void setup()
{
    Serial.begin(115200);
    Serial.println("M5Chain ToF Test");
    M5Chain.begin(&Serial2, 115200, RXD_PIN, TXD_PIN);

    if (M5Chain.isDeviceConnected()) {
        Serial.println("devices is connected");
        chain_status = M5Chain.getDeviceNum(&device_nums);
        if (chain_status == CHAIN_OK) {
            devices_list          = (device_list_t *)malloc(sizeof(device_list_t));
            devices_list->count   = device_nums;
            devices_list->devices = (device_info_t *)malloc(sizeof(device_info_t) * device_nums);
            if (M5Chain.getDeviceList(devices_list)) {
                Serial.println("devices list get success");
                printDeviceList(devices_list);
            } else {
                Serial.println("devices list get failed");
            }
        } else {
            Serial.printf("error status:%d \r\n", chain_status);
            Serial.printf("devices num get failed.\r\n");
        }
    } else {
        Serial.println("devices is not connected.");
    }

    if (devices_list) {
        for (uint8_t i = 0; i < devices_list->count; i++) {
            if (devices_list->devices[i].device_type == CHAIN_UART_TYPE_CODE) {
                chain_status = M5Chain.setRGBLight(devices_list->devices[i].id, 100, &operation_status);
                if (chain_status == CHAIN_OK && operation_status) {
                    Serial.printf("ID[%d] set rgb light success\r\n", devices_list->devices[i].id);
                } else {
                    Serial.printf("ID[%d] set rgb light failed, chain_status:%d  operation_status:%d \r\n",
                                  devices_list->devices[i].id, chain_status, operation_status);
                }
                for (uint8_t j = 0; j < 5; j++) {
                    uint8_t rgb[3] = {0};
                    chain_status =
                        M5Chain.setRGBValue(devices_list->devices[i].id, 0, 1, rgb_test[j], 3, &operation_status);
                    if (chain_status == CHAIN_OK && operation_status) {
                        Serial.printf("ID[%d] set rgb %d %d %d success\r\n", devices_list->devices[i].id,
                                      rgb_test[j][0], rgb_test[j][1], rgb_test[j][2]);
                    } else {
                        Serial.printf("ID[%d] set rgb %d %d %d failed, chain_status:%d  operation_status:%d \r\n",
                                      devices_list->devices[i].id, rgb_test[j][0], rgb_test[j][1], rgb_test[j][2],
                                      chain_status, operation_status);
                    }
                    chain_status = M5Chain.getRGBValue(devices_list->devices[i].id, 0, 1, rgb, 3, &operation_status);
                    if (chain_status == CHAIN_OK && operation_status) {
                        Serial.printf("ID[%d] get rgb %d %d %d success \r\n", devices_list->devices[i].id, rgb[0],
                                      rgb[1], rgb[2]);
                    } else {
                        Serial.printf("ID[%d] get rgb %d %d %d failed, chain_status:%d  operation_status:%d \r\n",
                                      devices_list->devices[i].id, rgb[0], rgb[1], rgb[2], chain_status,
                                      operation_status);
                    }
                    delay(500);
                }

                // chain_status =
                //     M5Chain.setUartI2cMode(devices_list->devices[i].id, CHAIN_I2C_LOW_SPEED_100KHZ,
                //     &operation_status);
                // if (chain_status == CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set i2c mode success \r\n", devices_list->devices[i].id);
                // } else {
                //     Serial.printf(
                //         "UART ID[%d] set i2c mode fail, chain_status:%d  operation_status:%d \r\n",
                //         devices_list->devices[i].id, chain_status, operation_status);
                // }

                // chain_status = M5Chain.setUartOutputMode(devices_list->devices[i].id, CHAIN_GPIO_PIN_1,
                //                                          CHAIN_GPIO_RESET, &operation_status);
                // if (chain_status == CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set gpio mode success \r\n", devices_list->devices[i].id);
                // } else {
                //     Serial.printf("UART ID[%d] set gpio mode fail, chain_status:%d  operation_status:%d \r\n",
                //                   devices_list->devices[i].id, chain_status, operation_status);
                // }

                // chain_status = M5Chain.setUartOutputMode(devices_list->devices[i].id, CHAIN_GPIO_PIN_2,
                // CHAIN_GPIO_SET,
                //                                          &operation_status);
                // if (chain_status == CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set gpio output success \r\n", devices_list->devices[i].id);
                // } else {
                //     Serial.printf("UART ID[%d] set gpio output fail, chain_status:%d  operation_status:%d \r\n",
                //                   devices_list->devices[i].id, chain_status, operation_status);
                // }

                // chain_status = M5Chain.setUartInputMode(devices_list->devices[i].id, CHAIN_GPIO_PIN_1,
                //                                         CHAIN_GPIO_PULL_DOWN, &operation_status);
                // if (chain_status == CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set gpio input success \r\n", devices_list->devices[i].id);
                // } else {
                //     Serial.printf("UART ID[%d] set gpio input fail, chain_status:%d  operation_status:%d \r\n",
                //                   devices_list->devices[i].id, chain_status, operation_status);
                // }

                // chain_status = M5Chain.setUartNvicMode(devices_list->devices[i].id, CHAIN_GPIO_PIN_1,
                // CHAIN_GPIO_PULL_UP, CHAIN_GPIO_MODE_IT_RISING_FALLING, &operation_status); if(chain_status ==
                // CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set nvic mode success \r\n", devices_list->devices[i].id);
                // }else {
                //     Serial.printf("UART ID[%d] set nvic mode fail, chain_status:%d  operation_status:%d \r\n",
                //                   devices_list->devices[i].id, chain_status, operation_status);
                // }

                // chain_status = M5Chain.setUartNvicMode(devices_list->devices[i].id, CHAIN_GPIO_PIN_2,
                // CHAIN_GPIO_PULL_UP, CHAIN_GPIO_MODE_IT_RISING_FALLING, &operation_status); if(chain_status ==
                // CHAIN_OK && operation_status == 1) {
                //     Serial.printf("UART ID[%d] set nvic mode success \r\n", devices_list->devices[i].id);
                // }else {
                //     Serial.printf("UART ID[%d] set nvic mode fail, chain_status:%d  operation_status:%d \r\n",
                //                   devices_list->devices[i].id, chain_status, operation_status);
                // }

                chain_status = M5Chain.setUartAdcMode(devices_list->devices[i].id, CHAIN_ADC_CHANNEL_ENABLE,
                                                      CHAIN_ADC_CHANNEL_ENABLE, &operation_status);
                if (chain_status == CHAIN_OK && operation_status == 1) {
                    Serial.printf("UART ID[%d] set adc mode success \r\n", devices_list->devices[i].id);
                } else {
                    Serial.printf("UART ID[%d] set adc mode fail, chain_status:%d  operation_status:%d \r\n",
                                  devices_list->devices[i].id, chain_status, operation_status);
                }
            }
        }
    } else {
        Serial.println("devices list is NULL");
    }

    delay(5);
}

uint8_t i2c_scan_addr[128] = {0};
uint8_t i2c_scan_addr_num  = 0;
work_status_t gpio1;
work_status_t gpio2;
void loop()
{
    if (devices_list) {
        for (uint8_t i = 0; i < devices_list->count; i++) {
            if (devices_list->devices[i].id == CHAIN_UART_TYPE_CODE) {
                chain_status = M5Chain.getUartWorkMode(devices_list->devices[i].id, &gpio1, &gpio2);
                if (chain_status == CHAIN_OK) {
                    printWorkStatus((work_status_t)gpio1, (work_status_t)gpio2);
                    if (gpio1 == CHAIN_I2C_WORK_STATUS && gpio2 == CHAIN_I2C_WORK_STATUS) {
                        chain_status =
                            M5Chain.getUartI2cScanAddr(devices_list->devices[i].id, &i2c_scan_addr_num, i2c_scan_addr,
                                                       sizeof(i2c_scan_addr), &operation_status);
                        if (operation_status == CHAIN_OK && operation_status == 1) {
                            if (operation_status == 1) {
                                Serial.printf("UART ID[%d] scan i2c device success, num: %d \r\n",
                                              devices_list->devices[i].id, i2c_scan_addr_num);
                                for (uint8_t j = 0; j < i2c_scan_addr_num; j++) {
                                    Serial.printf("UART ID[%d] i2c addr%d:0x%02x \r\n", devices_list->devices[i].id, j,
                                                  i2c_scan_addr[j]);
                                }
                            }
                        } else {
                            Serial.printf(
                                "UART ID[%d] get i2c scan addr fail, chain_status:%d  operation_status:%d \r\n",
                                devices_list->devices[i].id, chain_status, operation_status);
                        }

                    } else {
                        if (gpio1 == CHAIN_INPUT_WORK_STATUS) {
                            uint8_t gpio1_status = 0;
                            chain_status = M5Chain.getUartInputLevel(devices_list->devices[i].id, CHAIN_GPIO_PIN_1,
                                                                     &gpio1_status, &operation_status);
                            if (chain_status == CHAIN_OK && operation_status == 1) {
                                Serial.printf("UART ID[%d] gpio1 status: %d \r\n", devices_list->devices[i].id,
                                              gpio1_status);
                            } else {
                                Serial.printf(
                                    "UART ID[%d] get gpio1 status fail, chain_status:%d  operation_status:%d \r\n",
                                    devices_list->devices[i].id, chain_status, operation_status);
                            }
                        }

                        if (gpio2 == CHAIN_INPUT_WORK_STATUS) {
                            uint8_t gpio2_status = 0;
                            chain_status = M5Chain.getUartInputLevel(devices_list->devices[i].id, CHAIN_GPIO_PIN_2,
                                                                     &gpio2_status, &operation_status);
                            if (chain_status == CHAIN_OK && operation_status == 1) {
                                Serial.printf("UART ID[%d] gpio2 status: %d \r\n", devices_list->devices[i].id,
                                              gpio2_status);
                            } else {
                                Serial.printf(
                                    "UART ID[%d] get gpio2 status fail, chain_status:%d  operation_status:%d \r\n",
                                    devices_list->devices[i].id, chain_status, operation_status);
                            }
                        }

                        if (gpio1 == CHAIN_NVIC_WORK_STATUS || gpio2 == CHAIN_NVIC_WORK_STATUS) {
                            uint16_t nvic_status = 0;
                            while (M5Chain.getUartNvicTriggerStatus(devices_list->devices[i].id, &nvic_status)) {
                                Serial.printf("UART ID[%d] nvic status: 0x%04x \r\n", devices_list->devices[i].id,
                                              nvic_status);
                            }
                        }

                        if (gpio1 == CHAIN_ADC_WORK_STATUS || gpio2 == CHAIN_ADC_WORK_STATUS) {
                            uint8_t adc_channel_nums = 0;
                            uint8_t adc_buffer[4]    = {0};
                            chain_status = M5Chain.getUartAdcValue(devices_list->devices[i].id, &adc_channel_nums,
                                                                   adc_buffer, 4, &operation_status);
                            if (chain_status == CHAIN_OK && operation_status == 1) {
                                for (uint8_t j = 0; j < adc_channel_nums * 2; j = j + 2) {
                                    Serial.printf("UART ID[%d] adc value: %d \r\n", devices_list->devices[i].id,
                                                  adc_buffer[j + 1] << 8 | adc_buffer[j]);
                                }
                            } else {
                                Serial.printf(
                                    "UART ID[%d] get adc value fail, chain_status:%d  operation_status:%d \r\n",
                                    devices_list->devices[i].id, chain_status, operation_status);
                            }
                        }
                    }
                } else {
                    Serial.printf("UART ID[%d] get work mode fail, chain_status:%d  operation_status:%d \r\n",
                                  devices_list->devices[i].id, chain_status, operation_status);
                }
            }
        }
    }
    delay(100);
}

void printDeviceList(device_list_t *devices)
{
    if (devices == NULL) {
        Serial.println("devices is NULL");
        return;
    }

    Serial.print("devices count: ");
    Serial.println(devices->count);

    for (uint8_t i = 0; i < devices->count; i++) {
        Serial.print("devices ID: ");
        Serial.println(devices->devices[i].id);
        Serial.print("devices type: ");
        Serial.println(devices->devices[i].device_type);
    }
}

void printWorkStatus(work_status_t gpio11_status, work_status_t gpio12_status)
{
    Serial.print("  >>> GPIO_1: ");
    switch (gpio11_status) {
        case CHAIN_NOT_WORK_STATUS:
            Serial.println("Not configured working status");
            break;
        case CHAIN_OUTPUT_WORK_STATUS:
            Serial.println("Output status");
            break;
        case CHAIN_INPUT_WORK_STATUS:
            Serial.println("Input status");
            break;
        case CHAIN_NVIC_WORK_STATUS:
            Serial.println("External interrupt working status");
            break;
        case CHAIN_ADC_WORK_STATUS:
            Serial.println("ADC working status");
            break;
        case CHAIN_I2C_WORK_STATUS:
            Serial.println("I2C working status");
            break;
        default:
            Serial.println("Unrecognized work status");
            break;
    }

    Serial.print("  >>> GPIO_2: ");
    switch (gpio12_status) {
        case CHAIN_NOT_WORK_STATUS:
            Serial.println("Not configured working status.");
            break;
        case CHAIN_OUTPUT_WORK_STATUS:
            Serial.println("Output status.");
            break;
        case CHAIN_INPUT_WORK_STATUS:
            Serial.println("Input status.");
            break;
        case CHAIN_NVIC_WORK_STATUS:
            Serial.println("External interrupt working status.");
            break;
        case CHAIN_ADC_WORK_STATUS:
            Serial.println("ADC working status.");
            break;
        case CHAIN_I2C_WORK_STATUS:
            Serial.println("I2C working status.");
            break;
        default:
            Serial.println("Unrecognized work status.");
            break;
    }
}
