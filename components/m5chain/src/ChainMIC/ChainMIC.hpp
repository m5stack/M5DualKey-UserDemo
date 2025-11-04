/*
 *SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_MIC_HPP_
#define _CHAIN_MIC_HPP_

#include <ChainCommon.hpp>

/**
 * @brief Maximum threshold value for the MIC device.
 */
#define MIC_THRESHOLD_MAX (4095)

/**
 * @brief Maximum trigger cycle value for the MIC device.
 */
#define MIC_TRIGGER_CYCLE_MAX (1000)

/**
 * @brief Minimum trigger cycle value for the MIC device.
 */
#define MIC_TRIGGER_CYCYE_MIN (300)

/**
 * @brief Enumeration of commands for the ChainMIC class.
 */
typedef enum {
    CHAIN_MIC_GET_12ADC           = 0x30, /** @brief Command to get 12-bit ADC value */
    CHAIN_MIC_SET_THRESHOLD_VALUE = 0x31, /** @brief Command to set threshold value */
    CHAIN_MIC_GET_THRESHOLD_VALUE = 0x32, /** @brief Command to get threshold value */
    CHAIN_MIC_SET_MODE            = 0xE1, /** @brief Command to set mode */
    CHAIN_MIC_GET_MODE            = 0xE2, /** @brief Command to get mode */
    CHAIN_MIC_SET_TRIGGER_CYCLE   = 0xE3, /** @brief Command to set trigger cycle */
    CHAIN_MIC_GET_TRIGGER_CYCLE   = 0xE4, /** @brief Command to get trigger cycle */
} chain_mic_cmd_t;

/**
 * @brief Enumeration of modes for the ChainMIC class.
 */
typedef enum {
    CHAIN_MIC_NONE_REPORT_MODE = 0x00, /**< None report mode. */
    CHAIN_MIC_REPORT_MODE      = 0x01, /**< Report mode. */
} chain_mic_mode_t;

/**
 * @brief Enumeration of trigger types for the ChainMIC class.
 */
typedef enum {
    CHAIN_MIC_LOW_THRESHOLD_TRIGGER = 0x0300, /**< Low threshold trigger. */
    CHAIN_MIC_HIGH_THRESHOLD_TRIGGER  = 0x0301, /**< High threshold trigger. */
} chain_mic_trigger_t;

class ChainMIC : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the 12-bit ADC value of the MIC device.
     *
     * This function fetches the 12-bit ADC value from the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param adcValue Pointer to store the retrieved 12-bit ADC value.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMIC12BitAdc(uint8_t id, uint16_t *adcValue, unsigned long timeout = 100);

    /**
     * @brief Sets the threshold value for the MIC device.
     *
     * This function sets the threshold value for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param thresholdValue The threshold value to set.
     * @param saveToFlash Whether to save the threshold value to flash (default is CHAIN_SAVE_FLASH_DISABLE).
     * @param operationStatus Pointer to store the operation status.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICThresholdValue(uint8_t id, uint16_t thresholdValue, uint8_t *operationStatus,
                                        chain_save_flash_t saveToFlash = CHAIN_SAVE_FLASH_DISABLE,
                                        unsigned long timeout          = 100);

    /**
     * @brief Gets the threshold value of the MIC device.
     *
     * This function retrieves the threshold value of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param thresholdValue Pointer to store the retrieved threshold value.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICThresholdValue(uint8_t id, uint16_t *thresholdValue, unsigned long timeout = 100);

    /**
     * @brief Sets the mode for the MIC device.
     *
     * This function sets the mode for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param mode The mode to set.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICMode(uint8_t id, chain_mic_mode_t mode, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the mode of the MIC device.
     *
     * This function retrieves the mode of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param mode Pointer to store the retrieved mode.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICMode(uint8_t id, chain_mic_mode_t *mode, unsigned long timeout = 100);

    /**
     * @brief Sets the trigger cycle for the MIC device.
     *
     * This function sets the trigger cycle for the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param triggerCycle The trigger cycle to set.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t setMICTriggerCycle(uint8_t id, uint16_t triggerCircle, uint8_t *operationStatus,
                                      unsigned long timeout = 100);

    /**
     * @brief Gets the trigger cycle of the MIC device.
     *
     * This function retrieves the trigger cycle of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param triggerCycle Pointer to store the retrieved trigger cycle.
     * @param timeout Timeout duration in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., CHAIN_OK, CHAIN_TIMEOUT, etc.).
     */
    chain_status_t getMICTriggerCycle(uint8_t id, uint16_t *triggerCycle, unsigned long timeout = 100);

    /**
     * @brief Gets the trigger status of the MIC device.
     *
     * This function retrieves the trigger status of the specified MIC device.
     *
     * @param id The position of the MIC device in the chain (starting from 1).
     * @param triggerStatus Pointer to store the retrieved trigger status.
     *
     * @return True if the trigger status is valid, false otherwise.
     */
    bool getMICTriggerStatus(uint8_t id, chain_mic_trigger_t *triggerStatus);

private:
};

#endif  // _CHAIN_MIC_HPP_
