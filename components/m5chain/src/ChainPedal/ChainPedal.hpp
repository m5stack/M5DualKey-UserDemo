/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef _CHAIN_PEDAL_HPP_
#define _CHAIN_PEDAL_HPP_
#include <ChainCommon.hpp>

/**
 * @brief Maximum RGB overlay value for the pedal device.
 */
#define PEDAL_OVERLAY_VALUE_MAX (0x0F)

/**
 * @brief Maximum configuration entry time in seconds.
 */
#define PEDAL_ENTER_CONFIG_TIME_MAX (30)

/**
 * @brief Enumeration of pedal working modes.
 */
typedef enum {
    CHAIN_PEDAL_SINGLE_MODE = 0, /**< Single button mode */
    CHAIN_PEDAL_SWITCH_MODE = 1, /**< Switch/toggle mode */
} chain_pedal_work_mode_t;

/**
 * @brief Enumeration of pedal switch statuses.
 */
typedef enum {
    CHAIN_PEDAL_SWITCH_OFF_STATUS = 0, /**< Pedal switch is OFF */
    CHAIN_PEDAL_SWITCH_ON_STATUS  = 1, /**< Pedal switch is ON */
} chain_pedal_switch_status_t;

/**
 * @brief Enumeration of pedal press events.
 */
typedef enum {
    CHAIN_PEDAL_SINGLE_CLICK = 0x0000, /**< Single click event detected */
    CHAIN_PEDAL_SWITCH_OFF   = 0x0400, /**< Switch OFF event */
    CHAIN_PEDAL_SWITCH_ON    = 0x0401, /**< Switch ON event */
} chain_pedal_press_event_t;

/**
 * @brief Enumeration of pedal command codes.
 */
typedef enum {
    CHAIN_PEDAL_GET_BUTTON_STATUS     = 0xE1, /**< Get current button status */
    CHAIN_PEDAL_GET_SWITCH_STATUS     = 0xE2, /**< Get current switch status */
    CHAIN_PEDAL_SET_WORK_MODE         = 0xE3, /**< Set working mode (single/switch) */
    CHAIN_PEDAL_GET_WORK_MODE         = 0xE4, /**< Get current working mode */
    CHAIN_PEDAL_SET_BUTTON_MODE       = 0xE5, /**< Set button mode */
    CHAIN_PEDAL_GET_BUTTON_MODE       = 0xE6, /**< Get current button mode */
    CHAIN_PEDAL_SET_ENTER_CONFIG_TIME = 0xE7, /**< Set configuration entry time */
    CHAIN_PEDAL_GET_ENTER_CONFIG_TIME = 0xE8, /**< Get configuration entry time */
    CHAIN_PEDAL_SET_RGB_OVERLAY_VALUE = 0xE9, /**< Set RGB overlay brightness value */
    CHAIN_PEDAL_GET_RGB_OVERLAY_VALUE = 0xEA, /**< Get RGB overlay brightness value */
} chain_pedal_cmd_t;

class ChainPedal : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the button status of a pedal device.
     *
     * This function reads the current button status of the pedal device at the specified
     * position in the chain.
     *
     * @param id             Position of the pedal device in the chain (1 = first device).
     * @param buttonStatus   Pointer to store the retrieved button status (0: released, 1: pressed).
     * @param timeout        Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalButtonStatus(uint8_t id, uint8_t *buttonStatus, unsigned long timeout = 100);

    /**
     * @brief Retrieves the switch status of a pedal device.
     *
     * This function reads the current ON/OFF status of the pedal's internal switch.
     *
     * @param id            Position of the pedal device in the chain (starting from 1).
     * @param switchStatus  Pointer to store the retrieved switch status.
     * @param timeout       Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalSwitchStatus(uint8_t id, chain_pedal_switch_status_t *switchStatus,
                                        unsigned long timeout = 100);

    /**
     * @brief Sets the working mode of a pedal device.
     *
     * This function sets the working mode of the pedal device to either single button mode or
     * switch/toggle mode. Optionally, this setting can be stored in device flash memory.
     *
     * @param id              Device position in the chain (starting from 1).
     * @param workMode        Desired working mode (single or switch).
     * @param operationStatus Pointer to store the result code returned by the device.
     * @param saveToFlash     Whether to store this setting in device flash (default: disable).
     * @param timeout         Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t setPedalWorkMode(uint8_t id, chain_pedal_work_mode_t workMode, uint8_t *operationStatus,
                                    chain_save_flash_t saveToFlash = CHAIN_SAVE_FLASH_DISABLE,
                                    unsigned long timeout          = 100);

    /**
     * @brief Retrieves the current working mode of a pedal device.
     *
     * @param id         Device position in the chain (starting from 1).
     * @param workMode   Pointer to store the retrieved working mode.
     * @param timeout    Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalWorkMode(uint8_t id, chain_pedal_work_mode_t *workMode, unsigned long timeout = 100);

    /**
     * @brief Sets the button mode of a pedal device.
     *
     * This function defines the button behavior (e.g., momentary or latching) of the pedal device.
     *
     * @param id              Device position in the chain (starting from 1).
     * @param buttonMode      Desired button mode.
     * @param operationStatus Pointer to store the result code returned by the device.
     * @param timeout         Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t setPedalButtonMode(uint8_t id, chain_button_mode_t buttonMode, uint8_t *operationStatus,
                                      unsigned long timeout = 100);

    /**
     * @brief Retrieves the button mode of a pedal device.
     *
     * @param id          Device position in the chain (starting from 1).
     * @param buttonMode  Pointer to store the retrieved button mode.
     * @param timeout     Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalButtonMode(uint8_t id, chain_button_mode_t *buttonMode, unsigned long timeout = 100);

    /**
     * @brief Sets the configuration entry time of a pedal device.
     *
     * This function defines how long (in seconds) the device remains in configuration mode.
     * The setting can optionally be stored in flash memory.
     *
     * @param id              Device position in the chain (starting from 1).
     * @param enterConfigTime Time in seconds (0 - 30) before exiting config mode.
     * @param operationStatus Pointer to store the result code returned by the device.
     * @param saveToFlash     Whether to store this setting in device flash (default: disable).
     * @param timeout         Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t setPedalEnterConfigTime(uint8_t id, uint8_t enterConfigTime, uint8_t *operationStatus,
                                           chain_save_flash_t saveToFlash = CHAIN_SAVE_FLASH_DISABLE,
                                           unsigned long timeout          = 100);

    /**
     * @brief Retrieves the configuration entry time of a pedal device.
     *
     * @param id              Device position in the chain (starting from 1).
     * @param enterConfigTime Pointer to store the retrieved time in seconds.
     * @param timeout         Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalEnterConfigTime(uint8_t id, uint8_t *enterConfigTime, unsigned long timeout = 100);

    /**
     * @brief Sets the RGB overlay value of a pedal device.
     *
     * This function sets the brightness level of the RGB overlay (0x00 - 0x0F).
     * The setting can optionally be stored in flash memory.
     *
     * @param id              Device position in the chain (starting from 1).
     * @param overlayValue    RGB overlay brightness value.
     * @param operationStatus Pointer to store the result code returned by the device.
     * @param saveToFlash     Whether to store this setting in device flash (default: disable).
     * @param timeout         Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t setPedalRGBOverlayValue(uint8_t id, uint8_t overlayValue, uint8_t *operationStatus,
                                           chain_save_flash_t saveToFlash = CHAIN_SAVE_FLASH_DISABLE,
                                           unsigned long timeout          = 100);

    /**
     * @brief Retrieves the RGB overlay value of a pedal device.
     *
     * @param id           Device position in the chain (starting from 1).
     * @param overlayMode  Pointer to store the retrieved overlay value.
     * @param timeout      Timeout in milliseconds (default: 100ms).
     *
     * @return chain_status_t Operation result: CHAIN_OK, CHAIN_BUSY, CHAIN_TIMEOUT, etc.
     */
    chain_status_t getPedalRGBOverlayValue(uint8_t id, uint8_t *overlayMode, unsigned long timeout = 100);

    /**
     * @brief Retrieves the latest trigger event of a pedal device.
     *
     * @param id             Device position in the chain (starting from 1).
     * @param triggerStatus  Pointer to store the retrieved press event.
     *
     * @return true if the trigger event was successfully read; false otherwise.
     */
    bool getPedalTriggerStatus(uint8_t id, chain_pedal_press_event_t *triggerStatus);

private:
};

#endif  // _CHAIN_PEDAL_HPP_