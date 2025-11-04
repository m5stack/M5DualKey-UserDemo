/*
 *SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 *SPDX-License-Identifier: MIT
 */

#ifndef CHAIN_PIR_HPP_
#define CHAIN_PIR_HPP_

#include <ChainCommon.hpp>

typedef enum {
    CHAIN_PIR_NO_PERSON = 0x00, /**< Status indicating the PIR is not detecting any person. */
    CHAIN_PIR_PERSON = 0x01,    /**< Status indicating the PIR is detecting a person. */
} pir_detect_result_t;          /**< Enumeration for recording the PIR's current detect result of the pir device. */


typedef enum {
    CHAIN_PIR_REPORT_NO_PERSON = 0x0300, /**< Status indicating the PIR is not detecting any person. */
    CHAIN_PIR_REPORT_PERSON = 0x0301,    /**< Status indicating the PIR is detecting a person. */
} pir_detect_report_t;          /**< Enumeration for recording the PIR's current detect result of the pir device at Report Mode. */

/**
 * @brief Chain PIR detect mode enumeration.
 */
typedef enum {
    CHAIN_DETECT_NONE_REPORT_MODE = 0x00, /**< None report mode. */
    CHAIN_DETECT_REPORT_MODE      = 0x01, /**< Report mode. */
} chain_detect_mode_t;

/**
 * @brief Enumeration for CHAIN_SWITCH device commands.
 *
 * This enumeration defines command codes for various operations of the CHAIN_SWITCH device.
 */
typedef enum {
    CHAIN_PIR_GET_IR_STATUS           = 0x37, /**< Command to get the current IR induction status */
    CHAIN_PIR_SET_AUTO_SEND_IR_STATUS = 0xE1,    /**< Command to set the auto-send ir status */
    CHAIN_PIR_GET_AUTO_SEND_IR_STATUS = 0xE2,    /**< Command to get the auto-send ir status */
} CHAIN_PIR_CMD_T;                         /**< Command types for Chain_PIR device operations */

class ChainPIR : virtual public ChainCommon {
public:
    /**
     * @brief Retrieves the current slip's switch status of the Switch device at the specified position in
     * the chain.
     *
     * This function gets the current slip direction change status of the Switch device. It determines whether the
     * rotation direction is increasing or decreasing in the down-to-up direction.
     *
     * @param id The position of the Switch device in the chain (starting from 1).
     * @param ir_status Pointer to store the current slip's switch status. (1 for open, 0 for close).
     * @param timeout The timeout duration for the operation in milliseconds (default is 100ms).
     *
     * @return The operation status (e.g., `CHAIN_OK`, `CHAIN_TIMEOUT`, etc.).
     */
    chain_status_t getIRStatus(uint8_t id, pir_detect_result_t *ir_status, unsigned long timeout = 100);


    /**
     * @brief Sets the PIR detect trigger mode for the PIR device at the specified position in the chain.
     *
     * This function sets the PIR detect trigger mode for the PIR device. The mode can either be 
     * 'CHAIN_DETECT_NONE_REPORT_MODE' or 'CHAIN_DETECT_REPORT_MODE'.
     *
     * @param id The position of the PIR device in the chain (starting from 1).
     * @param auto_status The desired trigger mode (CHAIN_DETECT_REPORT_MODE or CHAIN_DETECT_NONE_REPORT_MODE).
     * @param operationStatus Pointer to store the operation status.
     * @param timeout The timeout duration for the operation in milliseconds (default is 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t setPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t auto_status, uint8_t* operationStatus, unsigned long timeout = 100);

    /**
     * @brief Gets the PIR detect trigger mode for the PIR device at the specified position in the chain.
     *
     * This function retrieves the current PIR detect trigger mode of the PIR device.
     *
     * @param id The position of the PIR device in the chain (starting from 1).
     * @param auto_status Pointer to store the current trigger mode (CHAIN_DETECT_REPORT_MODE or CHAIN_DETECT_NONE_REPORT_MODE).
     * @param timeout The timeout duration for the operation in milliseconds (default is 100ms).
     *
     * @return Operation status (e.g., CHAIN_OK, CHAIN_BUSY, etc.).
     */
    chain_status_t getPIRDetectTriggerMode(uint8_t id, chain_detect_mode_t* auto_status, unsigned long timeout = 100);

    /**
     * @brief Gets the PIR detect report status.
     *
     * This function retrieves the PIR detect report status, indicating whether the PIR is detecting a person or not.
     *
     * @param id The position of the PIR device in the chain (starting from 1).
     * @param triggerStatus Pointer to store the PIR detect report status (CHAIN_PIR_REPORT_NO_PERSON or CHAIN_PIR_REPORT_PERSON).
     *
     * @return Returns true if the operation was successful, otherwise returns false.
     */
    bool getPIRDetectTrigger(uint8_t id, pir_detect_report_t *triggerStatus);

private:
};

#endif  // CHAIN_PIR_HPP_