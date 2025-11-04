/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef _CHAIN_BUZZER_HPP_
#define _CHAIN_BUZZER_HPP_

#include <ChainCommon.hpp>

/**
 * @brief Minimum buzzer frequency in Hz.
 */
#define BUZZER_FREQ_MIN     (100)

/**
 * @brief Maximum buzzer frequency in Hz.
 */
#define BUZZER_FREQ_MAX     (10000)

/**
 * @brief Maximum buzzer duty cycle in percent.
 */
#define BUZZER_DUTY_MAX     (100)

/**
 * @brief Enumeration for buzzer modes.
 */
typedef enum {
    BUZZER_MODE_AUTO_PLAY   = 0x00, /**< Buzzer plays automatically for a set duration. */
    BUZZER_MODE_MANUAL_PLAY = 0x01, /**< Buzzer requires manual start/stop control. */
} buzzer_mode_t;

/**
 * @brief Enumeration for buzzer on/off status.
 */
typedef enum {
    BUZZER_STATUS_OFF = 0x00, /**< Buzzer output disabled. */
    BUZZER_STATUS_ON  = 0x01, /**< Buzzer output enabled. */
} buzzer_status_t;

/**
 * @brief Command types for Chain_Buzzer device control.
 */
typedef enum {
    CHAIN_BUZZER_SET_MODE      = 0x30, /**< Set buzzer mode (auto/manual). */
    CHAIN_BUZZER_GET_MODE      = 0x31, /**< Get buzzer mode. */
    CHAIN_BUZZER_SET_AUTO_PLAY = 0x32, /**< Start auto-play mode with given frequency/duty/duration. */
    CHAIN_BUZZER_SET_FREQ      = 0x33, /**< Set buzzer frequency. */
    CHAIN_BUZZER_GET_FREQ      = 0x34, /**< Get buzzer frequency. */
    CHAIN_BUZZER_SET_DUTY      = 0x35, /**< Set buzzer duty cycle. */
    CHAIN_BUZZER_GET_DUTY      = 0x36, /**< Get buzzer duty cycle. */
    CHAIN_BUZZER_SET_STATUS    = 0x37, /**< Set buzzer on/off status. */
    CHAIN_BUZZER_GET_STATUS    = 0x38, /**< Get buzzer on/off status. */
} chain_buzzer_cmd_t;

/**
 * @brief Class to control a ChainBuzzer device in a multi-device chain.
 */
class ChainBuzzer : virtual public ChainCommon {
public:
    /**
     * @brief Set the buzzer operating mode.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param mode Desired buzzer mode (auto-play or manual-play).
     * @param operationStatus Pointer to store the operation result (1: success, 0: failure).
     * @param timeout Operation timeout in milliseconds (default 100 ms).
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t setBuzzerMode(uint8_t id, buzzer_mode_t mode, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Get the buzzer operating mode.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param mode Pointer to store the current buzzer mode.
     * @param timeout Operation timeout in milliseconds (default 100 ms).
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t getBuzzerMode(uint8_t id, buzzer_mode_t *mode, unsigned long timeout = 100);

    /**
     * @brief Start buzzer auto-play mode.
     *
     * Automatically plays a tone at the specified frequency and duty for a set duration.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param freq Buzzer frequency in Hz (range: BUZZER_FREQ_MIN - BUZZER_FREQ_MAX).
     * @param duty Duty cycle in percent (range: 0 - BUZZER_DUTY_MAX).
     * @param duration Duration in milliseconds.
     * @param operationStatus Pointer to store the operation result (1: success, 0: failure，2: mode error).
     * @param timeout Operation timeout in milliseconds (default 100 ms).
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t setBuzzerAutoPlay(uint8_t id, uint16_t freq, uint8_t duty, uint16_t duration,
                                     uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Set the buzzer frequency.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param freq Frequency value in Hz (range: BUZZER_FREQ_MIN - BUZZER_FREQ_MAX).
     * @param operationStatus Pointer to store the operation result.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code.
     */
    chain_status_t setBuzzerFreq(uint8_t id, uint16_t freq, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Get the current buzzer frequency.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param freq Pointer to store the retrieved frequency in Hz.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t getBuzzerFreq(uint8_t id, uint16_t *freq, unsigned long timeout = 100);

    /**
     * @brief Set the buzzer duty cycle.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param duty Duty cycle in percent (range: 0 - BUZZER_DUTY_MAX).
     * @param operationStatus Pointer to store the operation result.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code.
     */
    chain_status_t setBuzzerDuty(uint8_t id, uint8_t duty, uint8_t *operationStatus, unsigned long timeout = 100);

    /**
     * @brief Get the buzzer duty cycle.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param duty Pointer to store the retrieved duty cycle in percent.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t getBuzzerDuty(uint8_t id, uint8_t *duty, unsigned long timeout = 100);

    /**
     * @brief Set the buzzer ON/OFF status.
     *
     * When in manual-play mode, this directly controls buzzer output.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param buzzerStatus Desired buzzer status (BUZZER_STATUS_ON / BUZZER_STATUS_OFF).
     * @param operationStatus Pointer to store the operation result.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t setBuzzerStatus(uint8_t id, buzzer_status_t buzzerStatus, uint8_t *operationStatus,
                                   unsigned long timeout = 100);

    /**
     * @brief Get the buzzer ON/OFF status.
     *
     * @param id The position of the buzzer in the chain (starting from 1).
     * @param buzzerStatus Pointer to store the current buzzer status.
     * @param timeout Operation timeout in milliseconds.
     * @return Operation status code (e.g., CHAIN_OK, CHAIN_PARAMETER_ERROR).
     */
    chain_status_t getBuzzerStatus(uint8_t id, buzzer_status_t *buzzerStatus, unsigned long timeout = 100);

private:
};

#endif  // _CHAIN_BUZZER_HPP_