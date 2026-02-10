/*******************************************************************************
	Lab 4 Header File

	Company:
		Microchip Technology Inc.

	File Name:
		lab4.h

	Summary:
		Interface definitions for event-driven light sensor sampling.

	Description:
		This header file provides function prototypes and shared data for the
		Lab 4 DMA-based, sleepwalking light sensor application. It defines
		buffer sizing, DMA-related globals, and interfaces for data transfer,
		transmission, and power management.
*******************************************************************************/

#ifndef LAB4_H
#define LAB4_H

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Constants
// *****************************************************************************
// *****************************************************************************

#define NUMLIGHTSENSORMEASUREMENTSTOBUFFER 20

// *****************************************************************************
// *****************************************************************************
// Section: External Global Variables
// *****************************************************************************
// *****************************************************************************

extern uint16_t adcDMATransferredCount;
extern uint16_t lightSensorData[NUMLIGHTSENSORMEASUREMENTSTOBUFFER];

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Application Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Transmit all buffered samples (sleepwalking mode)
 * @param dataBuffer Pointer to circular buffer containing samples
 * @param bufferIndex Current write position in buffer
 */
void TransmitLightSensorData_SleepWalking(uint16_t * dataBuffer, uint16_t bufferIndex);

/**
 * @brief Conditionally transmit buffered samples to the terminal
 */
void TransmitLightSensorDataToTerminal(void);

/**
 * @brief Transmit buffered light sensor data (legacy helper)
 * @param dataBuffer Pointer to circular buffer containing samples
 * @param bufferIndex Current write position in buffer
 */
void TransmitLightSensorData(uint16_t * dataBuffer, uint8_t bufferIndex);

/**
 * @brief Disable DMA interrupts to avoid CPU wakeups
 */
void DisableDMAInterrupts(void);

/**
 * @brief Configure and enable DMA transfer descriptors
 */
void SetADCTransferDescriptors(void);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Power Management
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Reduce CPU clock speed for power savings
 */
void ReduceCPUSpeed(void);

/**
 * @brief Restore CPU to normal operating speed
 */
void SetCPUSpeedToNormal(void);

/**
 * @brief Configure voltage regulator for standby mode operation
 */
void WaitForULP(void);

#endif /* LAB4_H */