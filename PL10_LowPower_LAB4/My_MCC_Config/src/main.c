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
      /* Enter standby mode - CPU will sleep until RTC wakes it up */
        PM_StandbyModeEnter();
        
      /* Suspend the DMA channel */
        SetCPUSpeedToNormal();
        DMAC_ChannelSuspend( DMAC_CHANNEL_0);

        /* Transmit the light sensor data to terminal */
        TransmitLightSensorDataToTerminal();
        
        /* Resume the DMA channel */
        DMAC_ChannelResume(DMAC_CHANNEL_0);

        ReduceCPUSpeed();
    }
    
    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

/*******************************************************************************
 End of File
*/
