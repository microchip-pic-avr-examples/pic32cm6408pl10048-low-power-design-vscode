/*******************************************************************************
  Main Source File - Lab 4: Event-Driven Light Sensor

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Event-driven ADC sampling with DMA and standby mode.

  Description:
    This application demonstrates event-driven, low-power sampling by:
    - Triggering TC0/TC1 from RTC via the event system
    - Starting ADC conversions on TC1 overflow events
    - Moving ADC results to a buffer using DMA
    - Waking the CPU on ADC window comparator via TC2
    - Entering standby between samples and transmitting on threshold
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>                      // Defines printf
#include "definitions.h"                // SYS function prototypes
#include "lab4.h"

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
  /* =========================================================================
     INITIALIZATION PHASE
     =========================================================================
     Configure all peripherals, callbacks, and power settings before entering
     the event-driven sampling loop.
  */

  /* System Initialization - Configure clocks, peripherals, and I/O pins */
    SYS_Initialize ( NULL );

  /* 1. Disable DMA interrupts */
    DisableDMAInterrupts();
    
  /* 2. Set ADC channel descriptors */
    SetADCTransferDescriptors();
    
  /* 3. Enable timers */
    TC0_CompareStart(); 
    TC1_TimerStart();
    TC2_TimerStart();
      
  /* 4. Enable ADC and configure event start mode */
    ADC0_Enable();
    ADC0_ConversionStartModeSet(ADC_STARTMODE_EVENT);

  /* 5. Announce entry to sleep-walking mode, then disable UART */
    SERCOM1_USART_Enable();
    printf("\r\nEntering Sleep-Walking Mode\r\n");
    printf("RTC will trigger ADC via Event System\r\n");

    /* Disable SERCOM1 to save power until needed */
    SERCOM1_USART_Disable();
    
    /* 6. Start the RTC timer */
    RTC_Timer32Start();
    
    /* 7. Wait until ULPVREF is ready */
    WaitForULP();
    
    ReduceCPUSpeed();

    /* =========================================================================
       MAIN APPLICATION LOOP
       =========================================================================
       Enter standby until events wake the CPU, then transmit and return to sleep.
    */

    while ( true )
    {
        /* =====================================================================
         * MAIN EVENT LOOP: Wake → Process → Sleep Cycle
         * =====================================================================
         * This loop implements ultra-low-power operation:
         * 1. Sleep in standby mode at 32.768 kHz CPU clock
         * 2. Wake on ADC window comparator interrupt
         * 3. Process at 8 MHz to transmit data via UART
         * 4. Return to sleep
         *
         * Critical ordering:
         * - Clock MUST be restored before suspending DMA (peripheral timing)
         * - Clock MUST be restored before UART transmission (baud rate)
         * - DMA MUST be suspended during transmission (data coherency)
         * ===================================================================== */

        /* Enter standby mode - CPU will sleep until ADC window comparator wakes it */
        PM_StandbyModeEnter();

        /* Restore CPU to 8 MHz and reinitialize UART for correct baud rate.
         * This must happen BEFORE DMA suspend for reliable peripheral access. */
        SetCPUSpeedToNormal();

        /* Suspend DMA channel to prevent data corruption while reading buffer.
         * Suspension at full clock speed ensures reliable register access. */
        DMAC_ChannelSuspend(DMAC_CHANNEL_0);

        /* Transmit the light sensor data to terminal (UART requires 8 MHz clock) */
        TransmitLightSensorDataToTerminal();

        /* Resume DMA channel to continue background ADC sampling */
        DMAC_ChannelResume(DMAC_CHANNEL_0);

        /* Reduce CPU clock back to 32.768 kHz for ultra-low power consumption.
         * This also disables UART to save power. */
        ReduceCPUSpeed();
    }
    
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

/*******************************************************************************
 End of File
*/
