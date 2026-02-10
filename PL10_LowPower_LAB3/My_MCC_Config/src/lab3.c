/*******************************************************************************
    Lab 3 Source File

    Company:
        Microchip Technology Inc.

    File Name:
        lab3.c

    Summary:
        Standby-aware light sensor sampling and data transmission.

    Description:
        This file implements an ultra-low-power sampling system that:
        - Collects light sensor readings every 100 ms using RTC timing
        - Averages 16 ADC samples per reading to reduce noise
        - Maintains a circular buffer of 20 averaged readings
        - Conditionally transmits data over UART when brightness exceeds threshold
        - Manages CPU clock speed and standby modes for minimal power consumption
*******************************************************************************/

#include "device.h"                    // Device header for register definitions
#include "definitions.h"                // SYS function prototypes
#include "component/mclk.h"
#include "lab3.h"
#include <stdio.h>
#include <string.h>

/* =============================================================================
   Global Variables - Interrupt Flags
   ============================================================================= */

volatile bool timer100msOccured = false;        /* RTC 100ms timeout flag */
volatile bool timer10msOccured = false;         /* TC1 10ms sensor warmup flag */
volatile bool adcConversionComplete = false;    /* ADC result ready flag */
volatile bool systicktimer10msOccured = false;  /* Unused systick flag (reserved) */

/* =============================================================================
   Global Variables - Data Buffers and Counters
   ============================================================================= */

uint8_t  measurementCount = 0;                                      /* Current sample index (0-15) */
uint16_t adcResults[NUMADCMEASUREMENTSTOAVERAGE];                   /* Raw ADC samples (16 values) */
uint16_t lightSensorData[NUMLIGHTSENSORMEASUREMENTSTOBUFFER] = {0}; /* Averaged samples (20 values) */
uint8_t  lightSensorDataIndex = 0;                                  /* Circular buffer write position */

/* =============================================================================
   Function Implementations
   ============================================================================= */ 

/**
 * @brief Configure voltage regulator to remain active during standby mode
 * 
 * This function enables the ultra-low-power (ULP) voltage regulator to stay on
 * during standby sleep mode, allowing the device to maintain power while consuming
 * minimal current. The function waits with timeout to ensure the setting is applied.
 * 
 * @param None
 * @return None
 */
void WaitForULP(void) {
    /* Request VREG to stay on in standby (ULP); bit lives in SUPC_VREG */
    SUPC_REGS->SUPC_VREG |= SUPC_VREG_RUNSTDBY_Msk;

    /* Wait for the write to latch but avoid hanging forever if HW rejects it */
    uint32_t timeout = 100000U;
    while (((SUPC_REGS->SUPC_VREG & SUPC_VREG_RUNSTDBY_Msk) == 0U) && (timeout-- > 0U)) {
        /* wait */
    }
}

/**
 * @brief RTC Compare 0 interrupt callback handler
 * 
 * This callback is invoked every 100 ms when the RTC timer reaches the compare
 * value. It sets a flag to wake the main loop from standby mode and trigger
 * the next light sensor sampling cycle.
 * 
 * @param intCause Bitmask of interrupt causes that triggered this callback
 * @param context User-defined context pointer (unused)
 * @return None
 */
void LightSensor_RTC_CMP0Callback(RTC_TIMER32_INT_MASK intCause, uintptr_t context) {
    if (intCause & RTC_TIMER32_INT_MASK_CMP0) {
        timer100msOccured = true;
    }
}
 
/**
 * @brief TC1 timer overflow interrupt callback handler
 * 
 * This callback is invoked when the TC1 timer overflows after 10 ms. It signals
 * that the light sensor has had sufficient warm-up time and is ready for ADC sampling.
 * 
 * @param status Timer status flags
 * @param context User-defined context pointer (unused)
 * @return None
 */
void LightSensor_TC1_OVFCallback(TC_TIMER_STATUS status, uintptr_t context) {
    timer10msOccured = true;
}

/**
 * @brief ADC result ready interrupt callback handler
 * 
 * This callback is invoked when an ADC conversion completes and the result is
 * ready to be read. It sets a flag to notify the sampling loop that conversion
 * has finished.
 * 
 * @param status ADC status flags indicating the interrupt cause
 * @param context User-defined context pointer (unused)
 * @return None
 */
void LightSensor_ADC_RESRDYCallback(ADC_STATUS status, uintptr_t context) {
    if (status == ADC_STATUS_RESRDY) {
        adcConversionComplete = true;
    }
}


/**
 * @brief Calculate the arithmetic mean of values in a buffer
 * 
 * Sums all values in the provided buffer and divides by the buffer size
 * to compute the average. Uses 32-bit accumulator to prevent overflow.
 * 
 * @param dataBuffer Pointer to the first element of the data array
 * @param sizeBuffer Number of elements in the buffer to average
 * @return The calculated average value as a 16-bit unsigned integer
 */
uint16_t AverageBuffer(uint16_t * dataBuffer, uint8_t sizeBuffer) {
    uint32_t accumulator = 0;
    uint16_t average = 0;
    uint8_t i = 0;
    
    /* Sum all values in the buffer */
    for (i = 0; i < sizeBuffer; i++) {
        accumulator += *dataBuffer;
        dataBuffer++;
    }
    
    /* Calculate and return the average */
    average = (uint16_t)(accumulator / sizeBuffer);
    return average;
}

/**
 * @brief Transmit all buffered light sensor data over UART
 * 
 * Sends the string "Bright" followed by all 20 averaged light sensor readings
 * from the circular buffer. Data is transmitted starting from the oldest sample
 * to maintain chronological order.
 * 
 * @param dataBuffer Pointer to the circular buffer containing 20 averaged samples
 * @param bufferIndex Current write position in the circular buffer
 * @return None
 */
void TransmitLightSensorData(uint16_t * dataBuffer, uint8_t bufferIndex) { 
    /* Transmit "Bright" message followed by all 20 averaged samples */
    printf("\r\nBright!\r\n");
    
    /* Start from the oldest sample (next index after current) and wrap around */
    uint8_t index = IncrementWrapLightSensorDataIndex(bufferIndex);
    
    /* Transmit all samples in chronological order */
    for (uint8_t i = 0; i < NUMLIGHTSENSORMEASUREMENTSTOBUFFER; i++) {
        printf("%u\r\n", dataBuffer[index]);
        index = IncrementWrapLightSensorDataIndex(index);
    }
}

/**
 * @brief Increment circular buffer index with wraparound
 * 
 * Increments the buffer index and wraps back to zero when reaching the end
 * of the buffer, implementing circular buffer behavior for the 20-sample
 * light sensor data array.
 * 
 * @param bufferIndex Current position in the circular buffer (0-19)
 * @return Next position in the circular buffer, wrapped to 0 if at end
 */
uint8_t IncrementWrapLightSensorDataIndex(uint8_t bufferIndex) {
    bufferIndex++;
    if (bufferIndex >= NUMLIGHTSENSORMEASUREMENTSTOBUFFER) {
        bufferIndex = 0;  /* Wrap to beginning */
    }
    return bufferIndex;
}

/**
 * @brief Register all interrupt callback functions for peripherals
 * 
 * Registers callback functions for the RTC (100 ms sampling interval),
 * TC1 (10 ms sensor warm-up timer), and ADC (conversion complete notification).
 * Must be called during initialization before starting any timers or peripherals.
 * 
 * @param None
 * @return None
 */
void RegisterCallbacks(void)
{
    /* Register RTC 100ms timeout callback */
    RTC_Timer32CallbackRegister(LightSensor_RTC_CMP0Callback, (uintptr_t) NULL ); 
    
    /* Register TC1 10ms timer callback */
    TC1_TimerCallbackRegister(LightSensor_TC1_OVFCallback, (uintptr_t) NULL ); 
    
    /* Register ADC conversion complete callback */
    ADC0_CallbackRegister(LightSensor_ADC_RESRDYCallback , (uintptr_t) NULL );
}

/**
 * @brief Collect 16 ADC samples from the light sensor
 * 
 * Configures the ADC to sample the light sensor on pin AIN26 (PA26) and
 * collects 16 consecutive readings. Each reading waits for conversion completion
 * using interrupt flag or polling with timeout. Results are stored in the
 * global adcResults array for subsequent averaging.
 * 
 * @param None
 * @return None
 */
void StartADCDataCollection (void)
{
    /* Ensure ADC is sampling the light sensor on AIN26 (PA26). PA26 is configured as analog in pin_configurations.csv */
    ADC0_ChannelSelect( ADC_POSINPUT_AIN26, ADC_NEGINPUT_GND );

    /* CRITICAL: Re-enable ADC in case it was disabled by standby or clock gating */
    if ((ADC0_REGS->ADC_CTRLA & ADC_CTRLA_ENABLE_Msk) == 0U) {
        ADC0_Enable();
    }

    for (measurementCount = 0; measurementCount < NUMADCMEASUREMENTSTOAVERAGE; measurementCount++){
            adcConversionComplete = false;

            ADC0_ConversionStart();

            /* Wait for conversion to finish (use ISR flag; fallback to polling with timeout) */
            uint32_t adcTimeout = 100000U;

            while((!adcConversionComplete) && (!ADC0_ConversionIsFinished()) && (adcTimeout-- > 0U))
            {
                /* wait */
            }

            /* If ISR didn't run but conversion finished, accept the result */
            if ((!adcConversionComplete) && (ADC0_ConversionIsFinished())) {
                adcConversionComplete = true;
            }

            uint16_t sample = (uint16_t)ADC0_ConversionResultGet();
            adcResults[measurementCount] = sample;
        }
}

/**
 * @brief Average the 16 ADC samples and store in the data buffer
 * 
 * Calculates the arithmetic mean of the 16 raw ADC samples collected by
 * StartADCDataCollection() and stores the result in the circular buffer
 * at the current index position.
 * 
 * @param None
 * @return None
 */
void AverageLightSensorData(void)
{
    lightSensorData[lightSensorDataIndex] = AverageBuffer(&adcResults[0], NUMADCMEASUREMENTSTOAVERAGE);
}

/**
 * @brief Conditionally transmit light sensor data if threshold exceeded
 * 
 * Checks if the most recent averaged light reading exceeds the brightness
 * threshold (2000). If so, enables UART, transmits "Bright" message and all
 * 20 buffered samples, then disables UART to conserve power. The UART receiver
 * is disabled during transmission to prevent data corruption.
 * 
 * @param None
 * @return None
 */
void TransmitLightSensorDataToTerminal(void)
{
    /* Only transmit if light level exceeds threshold */
    if (lightSensorData[lightSensorDataIndex] > LIGHTLEVELTHRESHOLD)
    {
        /* Enable UART only for transmission window */
        SERCOM1_USART_Enable();

        /* Disable RX to prevent incoming data from corrupting TX output */
        SERCOM1_USART_ReceiverDisable();

        /* Transmit "Bright" message and all 20 averaged samples */
        TransmitLightSensorData(&lightSensorData[0], lightSensorDataIndex);

        /* Wait for transmission to complete */
        uint32_t timeout = 2000000U;
        while((!SERCOM1_USART_TransmitComplete()) && (timeout-- > 0U)) {
            /* wait */
        }

        /* Clear any received data that may have accumulated */
        SERCOM1_USART_ReceiverEnable();
        uint8_t dummy;
        while((SERCOM1_REGS->USART.SERCOM_INTFLAG & (uint8_t)SERCOM_USART_INTFLAG_RXC_Msk) == (uint8_t)SERCOM_USART_INTFLAG_RXC_Msk)
        {
            dummy = (uint8_t)SERCOM1_REGS->USART.SERCOM_DATA;
        }

        /* Disable UART immediately after transmission - critical for power savings */
        SERCOM1_USART_Disable();
    }
}

/**
 * @brief Advance the circular buffer write pointer to the next position
 * 
 * Increments the global buffer index with wraparound, preparing the next
 * storage location for the subsequent light sensor reading. This maintains
 * a rolling window of the most recent 20 averaged samples.
 * 
 * @param None
 * @return None
 */
void StoreLightSensorDataToBuffer(void)
{
    lightSensorDataIndex = IncrementWrapLightSensorDataIndex(lightSensorDataIndex);  
}