/*******************************************************************************
    Lab 4 Source File

    Company:
        Microchip Technology Inc.

    File Name:
        lab4.c

    Summary:
        Event-driven light sensor sampling with DMA and sleep-walking.

    Description:
        This file implements event-driven sampling that:
        - Uses RTC/TC events to trigger ADC conversions
        - Transfers ADC results to memory via DMA
        - Wakes the CPU on window comparator events
        - Transmits data only when threshold conditions are met
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "pic32cm6408pl10048.h"
#include "component/mclk.h"
#include "definitions.h"                // SYS function prototypes
#include "peripheral/sercom/usart/plib_sercom1_usart.h"
#include <stdio.h>
#include "lab4.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Variables
// *****************************************************************************
// *****************************************************************************

uint16_t IncrementWrapLightSensorDataIndex(uint16_t bufferIndex);
uint16_t lightSensorData[NUMLIGHTSENSORMEASUREMENTSTOBUFFER];
uint16_t adcDMATransferredCount = 0U;
dmac_descriptor_registers_t adcTransferDescriptor __ALIGNED(16);

static bool IsLightAboveThreshold(uint16_t threshold)
{
    for (uint8_t i = 0; i < NUMLIGHTSENSORMEASUREMENTSTOBUFFER; i++) {
        if (lightSensorData[i] > threshold) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Transmit buffered light sensor data over UART
 *
 * Sends the string "Bright!" followed by all buffered samples, starting
 * from the oldest sample to preserve chronological order.
 *
 * @param dataBuffer Pointer to the circular buffer of samples
 * @param bufferIndex Current write index in the circular buffer
 */
void TransmitLightSensorData_SleepWalking(uint16_t * dataBuffer, uint16_t bufferIndex)
{
    uint8_t i = 0;
    
    printf("\r\n\nBright!\r\n");
    for (i = 0; i < NUMLIGHTSENSORMEASUREMENTSTOBUFFER; i++) {
        printf("%d\r\n", *(dataBuffer + bufferIndex)); 
        bufferIndex = IncrementWrapLightSensorDataIndex(bufferIndex); 
     }
}

/**
 * @brief Increment circular buffer index with wraparound
 *
 * @param bufferIndex Current buffer position
 * @return Next buffer position with wraparound
 */
uint16_t IncrementWrapLightSensorDataIndex(uint16_t bufferIndex)
{
    bufferIndex++;
    if (bufferIndex >= NUMLIGHTSENSORMEASUREMENTSTOBUFFER) {
        bufferIndex = 0;
    }
    return bufferIndex;
}

/**
 * @brief Configure voltage regulator to remain active during standby mode
 */
void WaitForULP(void)
{
    /* Request VREG to stay on in standby (ULP); bit lives in SUPC_VREG */
    SUPC_REGS->SUPC_VREG |= SUPC_VREG_RUNSTDBY_Msk;

    /* Wait for the write to latch but avoid hanging forever if HW rejects it */
    uint32_t timeout = 100000U;
    while (((SUPC_REGS->SUPC_VREG & SUPC_VREG_RUNSTDBY_Msk) == 0U) && (timeout-- > 0U)) {
        /* wait */
    }
}

/**
 * @brief Disable DMA interrupts to avoid CPU wakeups
 *
 * The DMA operates in the background in sleepwalking mode. Disabling the
 * DMA interrupt prevents unnecessary CPU wakeups during transfers.
 */
void DisableDMAInterrupts(void)
{
    NVIC_DisableIRQ(DMAC_IRQn); 
}

/**
 * @brief Configure DMA descriptors for ADC result transfers
 *
 * Sets up a linked-list DMA descriptor to continuously move ADC results
 * into the circular buffer without CPU intervention.
 */
void SetADCTransferDescriptors(void)
{
    adcTransferDescriptor.DMAC_BTCNT    = NUMLIGHTSENSORMEASUREMENTSTOBUFFER;
    adcTransferDescriptor.DMAC_BTCTRL   = DMAC_BTCTRL_STEPSIZE(0) | DMAC_BTCTRL_STEPSEL(0) | DMAC_BTCTRL_DSTINC(1) | DMAC_BTCTRL_SRCINC(0) | DMAC_BTCTRL_BEATSIZE(1) | DMAC_BTCTRL_BLOCKACT(0) | DMAC_BTCTRL_EVOSEL(0) | DMAC_BTCTRL_VALID(1);
    adcTransferDescriptor.DMAC_DESCADDR = (uint32_t) (&adcTransferDescriptor);
    adcTransferDescriptor.DMAC_DSTADDR  = ((uint32_t) (&lightSensorData[0])) + (NUMLIGHTSENSORMEASUREMENTSTOBUFFER * 2);
    adcTransferDescriptor.DMAC_SRCADDR  = (uint32_t) (&(ADC0_REGS->ADC_RESULT));
    DMAC_ChannelLinkedListTransfer (DMAC_CHANNEL_0, &adcTransferDescriptor);
}

/**
 * @brief Transmit buffered samples to the terminal (sleepwalking mode)
 *
 * Enables SERCOM1 for transmission, sends the buffered samples, clears
 * the ADC window comparator interrupt flag, and then disables SERCOM1.
 */
void TransmitLightSensorDataToTerminal(void)
{
    uint16_t highThreshold = (uint16_t)ADC0_REGS->ADC_WINHT;

    if (!IsLightAboveThreshold(highThreshold)) {
        ADC0_REGS->ADC_INTFLAG = (ADC_INTFLAGSET_WCMP_Msk);
        return;
    }

    SERCOM1_USART_Enable();
    TransmitLightSensorData_SleepWalking(&lightSensorData[0], DMAC_ChannelGetTransferredCount(DMAC_CHANNEL_0)); 
    ADC0_REGS->ADC_INTFLAG = (ADC_INTFLAGSET_WCMP_Msk);
    SERCOM1_USART_Disable();
}

/**
 * @brief Configure Generic Clock Generator 0 (GCLK0)
 *
 * Sets the source and integer division factor for GCLK0 by writing the
 * configuration to GCLK_GENCTRL[0] and waiting for synchronization. This
 * directly affects the CPU/root clock, so callers should manage peripherals
 * that are sensitive to clock changes (for example, UART baud rate).
 *
 * @param src  Generic clock source selection (see plib_clock.c/device headers)
 * @param div  Integer division factor applied to the selected source
 */
static void SetGclk0(uint32_t src, uint16_t div)
{
    GCLK_REGS->GCLK_GENCTRL[0] =
        GCLK_GENCTRL_DIV(div) | GCLK_GENCTRL_SRC(src) | GCLK_GENCTRL_GENEN_Msk;
    while ((GCLK_REGS->GCLK_SYNCBUSY & GCLK_SYNCBUSY_GENCTRL0_Msk) != 0U) { }
}

/**
 * @brief Reduce CPU clock speed for power savings during idle periods
 * 
 * Lowers the system clock by reconfiguring GCLK0 to use a low-frequency
 * source with a divider, then updates MCLK to apply the change. This
 * reduces active power consumption between sampling events.
 * 
 * @param None
 * @return None
 */
void ReduceCPUSpeed(void)
{
     /* UART must be disabled before lowering GCLK0 */
     SERCOM1_USART_Disable();

     /* Switch CPU clock to OSCHF / 6 = 4 MHz / 64 = 62.5 kHz */
     SetGclk0(0UL, 6UL); /* SRC=OSCHF per plib_clock.c */
     MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_CPUDIV_DIV64;

    /* Wait for clock ready (avoid hanging forever) */
    uint32_t timeout = 100000U;
    while (((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) == 0U) && (timeout-- > 0U))
    {
        /* Busy-wait for MCLK ready */
    }
}
/**
 * @brief Restore CPU clock to normal operating speed
 * 
 * Reconfigures GCLK0 and MCLK to return the CPU to its normal operating
 * frequency (e.g., OSCHF/3 = 8 MHz). Waits for the clock ready flag to
 * ensure the system clock is stable before returning.
 * 
 * @param None
 * @return None
 */
void SetCPUSpeedToNormal(void)
{
     /* Restore CPU clock to OSCHF / 3 = 8 MHz */
     SetGclk0(0UL, 3UL); /* SRC=OSCHF per plib_clock.c */
     MCLK_REGS->MCLK_CPUDIV = MCLK_CPUDIV_CPUDIV_DIV1;

    /* Wait for clock ready (avoid hanging forever) */
    uint32_t timeout = 100000U;
    while (((MCLK_REGS->MCLK_INTFLAG & MCLK_INTFLAG_CKRDY_Msk) == 0U) && (timeout-- > 0U))
    {
        /* Busy-wait for MCLK ready */
    }

    /* Reinitialize USART so the baud generator matches the restored clock */
    SERCOM1_USART_Initialize();
}