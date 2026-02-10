/* *****************************************************************************
   Lab 2 Header File

  Company:
    Microchip Technology Inc.

  File Name:
    lab2.h

   Summary:
      Interface definitions for RTC-timed light sensor sampling.

  Description:
    This header file provides function prototypes, constants, and macros for
    the Lab2 power-optimized light sensor sampling system. It defines the
    sampling parameters, threshold values, and interfaces for data collection,
    processing, transmission, and power management.
***************************************************************************** */

#ifndef LAB2_H
#define LAB2_H

#include <stdint.h>
#include <stdbool.h>
#include "definitions.h"    // device types like ADC_STATUS, TC_TIMER_STATUS, etc.

// *****************************************************************************
// *****************************************************************************
// Section: Configuration Constants
// *****************************************************************************
// *****************************************************************************

#define NUMADCMEASUREMENTSTOAVERAGE 16      /* Number of ADC samples averaged per reading */
#define NUMLIGHTSENSORMEASUREMENTSTOBUFFER 20  /* Size of circular buffer for averaged readings */
#define LIGHTLEVELTHRESHOLD 1500            /* Brightness threshold for UART transmission */

// *****************************************************************************
// *****************************************************************************
// Section: External Global Variables - Interrupt Flags
// *****************************************************************************
// *****************************************************************************

extern volatile bool timer100msOccured;         /* Set by RTC every 100ms to trigger sampling */
extern volatile bool timer10msOccured;          /* Set by TC1 after 10ms sensor warm-up */

// *****************************************************************************
// *****************************************************************************
// Section: Macros - Timing and Synchronization
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Wait for 100 ms RTC timeout interrupt
 * 
 * This macro blocks until the RTC timer fires its 100 ms compare interrupt.
 * It then atomically clears the flag to prevent race conditions with the ISR.
 */
#define WAIT_FOR_100mS_RTC_TIMEOUT()    \
    do {                                \
        while(!timer100msOccured);      \
        __disable_irq();                \
        timer100msOccured = false;      \
        __enable_irq();                 \
    } while(0)

/**
 * @brief Start TC1 timer and wait for 10 ms overflow
 * 
 * This macro starts the TC1 timer for sensor warm-up and blocks until
 * the 10 ms overflow interrupt fires, indicating the sensor is ready.
 */
#define TC1_10mS_TIMER_START()        \
    do {                              \
        timer10msOccured = false;     \
        TC1_TimerStart();             \
        while(!timer10msOccured);     \
    } while(0)

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief RTC timer32 interrupt mask type used by callbacks
 */
typedef uint32_t RTC_TIMER32_INT_MASK;

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - RTC Helper Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Enable RTC timer32 interrupts
 * @param interruptMask Bitmask of interrupts to enable
 */
void RTC_Timer32InterruptEnable(uint32_t interruptMask);

/**
 * @brief Register a callback function for RTC timer32 interrupts
 * @param callback Pointer to callback function
 * @param context User-defined context pointer passed to callback
 */
void RTC_Timer32CallbackRegister(void (*callback)(uint32_t intCause, uintptr_t context), uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Data Processing Utilities
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Calculate arithmetic mean of buffer values
 * @param dataBuffer Pointer to array of 16-bit values
 * @param sizeBuffer Number of elements to average
 * @return Average value as 16-bit unsigned integer
 */
uint16_t AverageBuffer(uint16_t * dataBuffer, uint8_t sizeBuffer);

/**
 * @brief Increment circular buffer index with wraparound
 * @param bufferIndex Current buffer position
 * @return Next buffer position with wraparound at NUMLIGHTSENSORMEASUREMENTSTOBUFFER
 */
uint8_t IncrementWrapLightSensorDataIndex(uint8_t bufferIndex);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Interrupt Callbacks
// *****************************************************************************
// *****************************************************************************

/**
 * @brief RTC Compare 0 interrupt callback (100ms interval)
 * @param intCause Interrupt cause bitmask
 * @param context User context pointer
 */
void LightSensor_RTC_CMP0Callback(uint32_t intCause, uintptr_t context);

/**
 * @brief TC1 timer overflow callback (10ms sensor warm-up)
 * @param status Timer status flags
 * @param context User context pointer
 */
void LightSensor_TC1_OVFCallback(TC_TIMER_STATUS status, uintptr_t context);

/**
 * @brief ADC result ready interrupt callback
 * @param status ADC status flags
 * @param context User context pointer
 */
void LightSensor_ADC_RESRDYCallback(ADC_STATUS status, uintptr_t context);

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Application Functions
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Transmit all 20 buffered light readings over UART
 * @param dataBuffer Pointer to circular buffer containing averaged samples
 * @param bufferIndex Current write position in buffer
 */
void TransmitLightSensorData(uint16_t * dataBuffer, uint8_t bufferIndex);

/**
 * @brief Register all interrupt callback functions
 */
void RegisterCallbacks(void);

/**
 * @brief Collect 16 ADC samples from light sensor
 */
void StartADCDataCollection(void);

/**
 * @brief Average 16 ADC samples and store result
 */
void AverageLightSensorData(void);

/**
 * @brief Conditionally transmit data if above brightness threshold
 */
void TransmitLightSensorDataToTerminal(void);

/**
 * @brief Advance circular buffer index to next position
 */
void StoreLightSensorDataToBuffer(void);

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

#endif /* LAB2_H */