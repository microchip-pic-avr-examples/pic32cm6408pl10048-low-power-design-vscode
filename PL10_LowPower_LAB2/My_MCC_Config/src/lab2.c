/*******************************************************************************
    Lab 2 Source File

    Company:
        Microchip Technology Inc.

    File Name:
        lab2.c

    Summary:
        RTC-timed light sensor sampling and data transmission.

    Description:
        This file implements a low-power sampling system that:
        - Collects light sensor readings every 100 ms using RTC timing
        - Averages 16 ADC samples per reading to reduce noise
        - Maintains a circular buffer of 20 averaged readings
        - Conditionally transmits data over UART when brightness exceeds threshold
        - Provides optional CPU clock speed management for power savings
*******************************************************************************/

#include "definitions.h"                // SYS function prototypes
#include "lab2.h"
#include <stdio.h>
#include <string.h>

/* =============================================================================
   Global Variables - Interrupt Flags
   ============================================================================= */
volatile bool timer100msOccured = false;        /* RTC 100ms timeout flag */
volatile bool timer10msOccured = false;         /* TC1 10ms sensor warmup flag */
volatile bool adcConversionComplete = false;    /* ADC result ready flag */

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
 * @brief RTC Compare 0 interrupt callback handler
 *
 * Invoked from the RTC interrupt when the compare register 0 matches. This
 * callback performs minimal work (sets a volatile flag) so that the main loop
 * can perform non-time-critical processing outside of interrupt context.
 *
 * Behavior:
 *  - Checks whether the CMP0 bit is set in the provided interrupt mask.
 *  - Sets `timer100msOccured` to true to signal the main task that a 100 ms
 *    sampling period has elapsed.
 *
 * @param intCause Bitmask of interrupt causes that triggered this callback
 *                 (RTC_TIMER32_INT_MASK). Only CMP0 is handled here.
 * @param context  User-defined context pointer (unused)
 *
 * @note Keep this routine short and non-blocking. Avoid calling blocking APIs
 *       or performing heavy computation inside this ISR.
 */
void LightSensor_RTC_CMP0Callback(RTC_TIMER32_INT_MASK intCause, uintptr_t context) {
    if (intCause & RTC_TIMER32_INT_MASK_CMP0) {
        timer100msOccured = true;
    }
}

/**
 * @brief TC1 overflow interrupt callback
 *
 * Called when the TC1 timer overflows (used as a short warm-up timer for the
 * light sensor). This handler sets a flag that indicates the sensor has had
 * sufficient stabilization time and the ADC can begin sampling.
 *
 * @param status  Timer status flags (TC_TIMER_STATUS). Not inspected here.
 * @param context User-defined context pointer (unused)
 *
 * @note Executes in interrupt context; do not block. The main loop polls
 *       `timer10msOccured` to proceed with ADC sampling.
 */
void LightSensor_TC1_OVFCallback(TC_TIMER_STATUS status, uintptr_t context) {
    timer10msOccured = true;
}

/**
 * @brief ADC conversion complete interrupt callback
 *
 * Notified by the ADC peripheral when a conversion completes. This callback
 * sets the `adcConversionComplete` flag which the sampling routine uses to
 * detect completion without busy-polling the hardware status registers.
 *
 * @param status  ADC_STATUS indicating the cause of the interrupt. Only
 *                ADC_STATUS_RESRDY is considered here.
 * @param context User-defined context pointer (unused)
 *
 * @note The ISR only sets a volatile flag; the actual ADC result is read by
 *       the foreground sampling code (StartADCDataCollection) to avoid doing
 *       heavier work inside the interrupt.
 */
void LightSensor_ADC_RESRDYCallback(ADC_STATUS status, uintptr_t context) {
    if (status == ADC_STATUS_RESRDY) {
        adcConversionComplete = true;
    }
}


/**
 * @brief Compute the arithmetic mean of an array of 16-bit values
 *
 * Sums `sizeBuffer` entries pointed to by `dataBuffer` using a 32-bit
 * accumulator to prevent overflow, then returns the integer average as a
 * 16-bit value.
 *
 * @param dataBuffer Pointer to the first element of the input array
 * @param sizeBuffer Number of elements to include in the average (must be >0)
 * @return The integer average of the provided values (uint16_t)
 *
 * @note If `sizeBuffer` is zero the behavior is undefined; callers must
 *       guarantee a non-zero count. Using a 32-bit accumulator prevents
 *       overflow for typical ADC sample ranges and reasonable buffer sizes.
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
 * @brief Transmit the full circular buffer contents over the console/UART
 *
 * Sends a leading "Bright!" notification followed by every averaged sample
 * stored in the circular buffer. Transmission begins at the oldest stored
 * sample (the element immediately after the current write index) to preserve
 * chronological order.
 *
 * @param dataBuffer  Pointer to the circular buffer containing averaged samples
 * @param bufferIndex Current write position in the circular buffer (0..N-1)
 *
 * @note This function uses `printf` for output and therefore assumes the
 *       console/UART has been enabled and configured by the caller. It does
 *       not enable/disable the UART itself — that responsibility belongs to
 *       higher-level code (see TransmitLightSensorDataToTerminal).
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
 * @brief Advance a circular buffer index with wraparound
 *
 * Increments the provided index and wraps it to zero when it reaches the
 * configured buffer length (`NUMLIGHTSENSORMEASUREMENTSTOBUFFER`). This
 * helper centralizes wrap logic so callers can avoid manual modulo math.
 *
 * @param bufferIndex Current position in the circular buffer (0..N-1)
 * @return The next position in the circular buffer (wrapped to 0 if at end)
 */
uint8_t IncrementWrapLightSensorDataIndex(uint8_t bufferIndex) {
    bufferIndex++;
    if (bufferIndex >= NUMLIGHTSENSORMEASUREMENTSTOBUFFER) {
        bufferIndex = 0;  /* Wrap to beginning */
    }
    return bufferIndex;
}

/**
 * @brief Register peripheral interrupt callbacks used by the application
 *
 * Installs callback handlers for the RTC compare (sampling interval), the
 * TC1 warm-up timer, and the ADC conversion completion. Call this function
 * during system initialization before enabling the corresponding peripherals
 * or starting timers so that interrupts are handled correctly.
 *
 * @note Callbacks are lightweight and set volatile flags; main application
 *       logic polls those flags and performs heavier processing outside ISR
 *       context.
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
 * @brief Perform a burst of ADC measurements from the light sensor
 *
 * This routine configures the ADC input to AIN26 (PA26), ensures the ADC
 * peripheral is enabled, and then collects `NUMADCMEASUREMENTSTOAVERAGE`
 * samples into the global `adcResults[]` array. Each conversion is started
 * explicitly and the function waits for completion using either the
 * `adcConversionComplete` ISR flag or by polling the peripheral with a
 * timeout to avoid indefinite blocking.
 *
 * Steps performed:
 *  1. Select ADC channel AIN26 (PA26).
 *  2. Enable ADC peripheral if disabled.
 *  3. For each sample:
 *     - Clear the conversion-complete flag and start conversion.
 *     - Wait for either the ISR to set `adcConversionComplete` or for the
 *       peripheral to report completion; a timeout prevents lockups.
 *     - Read the conversion result and store it in `adcResults[measurementCount]`.
 *
 * @note This function uses the global `measurementCount` index and sets
 *       `adcConversionComplete` as part of the conversion handshake with the
 *       ADC ISR. Keep time-critical ISR handlers minimal; the sampling loop
 *       handles result collection in the foreground.
 */
void StartADCDataCollection (void)
{
    /* Ensure ADC is sampling the light sensor on AIN26 (PA26). PA26 is configured as analog in pin_configurations.csv */
    ADC0_ChannelSelect( ADC_POSINPUT_AIN26, ADC_NEGINPUT_GND );

    /* Re-enable ADC peripheral if it was previously disabled */
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
 * @brief Compute and store averaged light sensor reading
 *
 * Uses `AverageBuffer` to compute the arithmetic mean of the last
 * `NUMADCMEASUREMENTSTOAVERAGE` raw ADC samples stored in `adcResults` and
 * writes the averaged value into the circular buffer at `lightSensorDataIndex`.
 *
 * @note This function does not advance the circular buffer index; callers
 *       should call `StoreLightSensorDataToBuffer()` after any required
 *       post-processing to move to the next slot.
 */
void AverageLightSensorData(void)
{
    lightSensorData[lightSensorDataIndex] = AverageBuffer(&adcResults[0], NUMADCMEASUREMENTSTOAVERAGE);
}

/**
 * @brief Transmit buffered light sensor data when brightness threshold exceeded
 *
 * If the most recent averaged sample at `lightSensorDataIndex` exceeds
 * `LIGHTLEVELTHRESHOLD`, this function enables the UART (SERCOM1), disables
 * the receiver to avoid RX/TX interference, transmits a leading "Bright!"
 * notification and all buffered samples via `TransmitLightSensorData`, waits
 * for the transmission to complete (with timeout), flushes any received data,
 * and then disables the UART to minimize power consumption.
 *
 * Side effects:
 *  - Temporarily enables SERCOM1 for transmission.
 *  - Disables the receiver during transmission and flushes any pending RX
 *    bytes afterwards.
 *
 * @note The timeout values are a safety to prevent indefinite blocking. The
 *       caller should ensure this function is executed in a context where a
 *       blocking transmit is acceptable.
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
 * @brief Move the circular buffer write pointer to the next slot
 *
 * Advances the global `lightSensorDataIndex` using `IncrementWrapLightSensorDataIndex`
 * so the next averaged reading will be stored in the following buffer slot.
 * This preserves a fixed-size rolling history of readings.
 */
void StoreLightSensorDataToBuffer(void)
{
    lightSensorDataIndex = IncrementWrapLightSensorDataIndex(lightSensorDataIndex);  
}

/**
 * @brief Configure Generic Clock Generator 0 (GCLK0)
 *
 * Sets the source and integer division factor for Generic Clock Generator 0
 * by writing the corresponding value to GCLK_GENCTRL[0] and waiting for
 * the synchronization bit to clear. This directly affects the CPU/root
 * clock, so it has system-wide side effects (peripheral baud rates,
 * timers, and timekeeping may change).
 *
 * @param src  Generic clock source selection (see plib_clock.c/device headers)
 * @param div  Integer division factor applied to the selected source
 *
 * @note Callers must handle peripherals that depend on clock timing
 *       (for example, disable UART or reconfigure baud) before lowering
 *       the clock to avoid communication errors. This helper is used by
 *       ReduceCPUSpeed() and SetCPUSpeedToNormal().
 */

static void SetGclk0(uint32_t src, uint16_t div)
{
    GCLK_REGS->GCLK_GENCTRL[0] =
        GCLK_GENCTRL_DIV(div) | GCLK_GENCTRL_SRC(src) | GCLK_GENCTRL_GENEN_Msk;
    while ((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL0_Msk) != 0U) { }
}

/**
 * @brief Reduce CPU clock speed to save power
 *
 * Lowers the system clock by reconfiguring GCLK0 to use a low-frequency
 * source with a divider, then updates MCLK to apply the change. This
 * is useful for reducing active power consumption between periodic
 * sampling events. The routine waits for the MCLK ready flag to ensure
 * the change has taken effect.
 *
 * Side effects:
 *  - Alters the root clock frequency and may affect peripheral timings
 *    (UART baud, timers, etc.). Callers should disable or quiesce
 *    timing-sensitive peripherals before invoking this function.
 */
void ReduceCPUSpeed(void)
{
    /* UART must be disabled before lowering GCLK0 */
    SERCOM1_USART_Disable();

    /* Switch CPU clock to OSC32K / 16 = 2.048 kHz */
    SetGclk0(3UL, 16UL); /* SRC=OSC32K per plib_clock.c */
    MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_CPUDIV_DIV1;
    while ((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) == 0U) { }
}
/**
 * @brief Restore CPU clock to normal operating frequency
 *
 * Reconfigures GCLK0 and MCLK to return the CPU to its normal operating
 * frequency (e.g., OSCHF/3 = 8 MHz). Waits for the clock ready flag to
 * ensure the system clock is stable before returning.
 *
 * @note After calling this function, callers should re-enable or
 *       reconfigure peripherals (UART, timers) if required for full-speed
 *       operation.
 */
void SetCPUSpeedToNormal(void)
{
    /* Restore CPU clock to OSCHF / 3 = 8 MHz */
    SetGclk0(0UL, 3UL); /* SRC=OSCHF per plib_clock.c */
    MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_CPUDIV_DIV1;
    while ((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) == 0U) { }
}

