/*******************************************************************************
  Main Source File - Lab 3: Standby Light Sensor

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Light sensor sampling with standby sleep between operations.

  Description:
    This application demonstrates ultra-low-power operation by:
    - Sampling a light sensor every 100 ms using RTC timing
    - Averaging 16 ADC readings per sample to reduce noise
    - Maintaining a circular buffer of the 20 most recent averaged values
    - Conditionally transmitting data over UART when brightness exceeds threshold
    - Entering standby sleep mode between operations to minimize power consumption
    - Keeping UART disabled outside transmit windows to reduce power
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "lab3.h"

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
       the main sampling loop. UART is disabled to conserve power.
    */
    
    /* System Initialization - Configure clocks, peripherals, and I/O pins */
    SYS_Initialize ( NULL );

    /* 1. Pin Configuration - Light sensor starts powered off */
    LightSensorVdd_Clear();

    /* 2. Register interrupt callbacks for RTC (100 ms), TC1 (10 ms), and ADC */
    RegisterCallbacks();

    /* 3. Enable ADC peripheral for light sensor measurements */
    ADC0_Enable();

    /* 4. Start RTC Timer and enable 100 ms compare interrupt for sampling intervals */
    RTC_Timer32Start();
    RTC_Timer32InterruptEnable(RTC_TIMER32_INT_MASK_CMP0);

    /* 5. Disable UART initially to save power - will be enabled only when transmitting data */
    SERCOM1_USART_Disable();

    /* 6. Configure voltage regulator to remain active during standby mode (ultra-low-power mode) */
    WaitForULP();

    

     /* =========================================================================
       MAIN APPLICATION LOOP
       =========================================================================
       Each iteration:
       1. Wait in standby for RTC 100 ms timeout
       2. Power sensor and wait 10 ms for warm-up
       3. Collect 16 ADC samples and average them
       4. Store result in a 20-sample circular buffer
       5. Transmit buffer over UART if brightness exceeds threshold
       6. Reduce CPU speed and return to standby
     */
    
    while ( true )
    {
        /* Poll system background tasks (required by Harmony framework) */
        SYS_Tasks ( );
        
        
        /* === STEP 1: Enter Standby and wait for RTC 100 ms timeout === */
        PM_StandbyModeEnter();                  /* Enter Standby until RTC fires */
        WAIT_FOR_100mS_RTC_TIMEOUT();           /* Wait for RTC interrupt, then clear flag */

        /* === STEP 2: Power up light sensor and begin warm-up === */
        LightSensorVdd_Set();                   /* Apply power to light sensor */
        timer10msOccured = false;
        TC1_TimerStart();                       /* Start 10 ms sensor warm-up timer */

        /* === STEP 3: Enter Standby during sensor warm-up === */
        PM_StandbyModeEnter();                  /* Standby during sensor warm-up */
        while (!timer10msOccured) { }           /* Wait for TC1 10 ms timeout */

        /* === STEP 4: Collect ADC samples from light sensor === */
        StartADCDataCollection();               /* Collect 16 ADC samples */
        LightSensorVdd_Clear();                 /* Power down sensor to save power */

        /* === STEP 5: Process and transmit data === */
        AverageLightSensorData();               /* Average 16 samples into single value */
        TransmitLightSensorDataToTerminal();    /* Send "Bright!" + 20 samples if > threshold */
        StoreLightSensorDataToBuffer();         /* Store averaged value in circular buffer */

        /* === STEP 6: Prepare for next cycle === */
        SERCOM1_USART_Disable();                /* Disable UART - ready for Standby */
    }

    return ( EXIT_FAILURE );
}

/*******************************************************************************
 End of File
*/