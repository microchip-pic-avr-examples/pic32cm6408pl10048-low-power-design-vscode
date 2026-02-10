/*******************************************************************************
  Main Source File - Lab 2: RTC-Timed Light Sensor

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Power-optimized light sensor sampling with RTC-timed intervals.

  Description:
    This application demonstrates low-power operation by:
    - Sampling a light sensor every 100 ms using RTC timing
    - Averaging 16 ADC readings per sample to reduce noise
    - Maintaining a circular buffer of the 20 most recent averaged values
    - Conditionally transmitting data over UART when brightness exceeds threshold
    - Minimizing power consumption through selective peripheral enabling
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
#include "lab2.h"

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
       Configure all peripherals, callbacks, and I/O pins before entering
       the main sampling loop. UART is disabled initially to conserve power.
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
    
     /* =========================================================================
       MAIN APPLICATION LOOP
       =========================================================================
       Each iteration:
       1. Wait for RTC 100 ms timeout
       2. Power up light sensor and wait 10 ms for warm-up
       3. Collect 16 ADC samples and power down sensor
       4. Average the samples and store the value in a 20-sample buffer
       5. Transmit the buffer over UART if brightness exceeds threshold
       6. Optionally reduce CPU speed during idle periods
     */
    
    while ( true )
    {
        /* Poll system tasks */
        SYS_Tasks ( );
        
        
        /* === STEP 1: Wait for RTC 100 ms compare interrupt === */
        WAIT_FOR_100mS_RTC_TIMEOUT();   /* Wait for RTC interrupt, then clear flag */
        
        
        /* === OPTIONAL: Restore CPU to full speed for active sampling === */
        /* Uncomment to enable dynamic clock speed management for power savings */
        //SetCPUSpeedToNormal();


        /* === STEP 2: Power up light sensor and wait 10 ms for stabilization === */
        LightSensorVdd_Set();           /* Apply power to sensor */
        TC1_10mS_TIMER_START();         /* Wait 10ms for sensor warm-up */
        
        
        /* === STEP 3: Collect 16 ADC samples and power down sensor === */
        StartADCDataCollection();       /* Sample ADC 16 times from AIN26 */
        LightSensorVdd_Clear();         /* Remove power from sensor */
        
        
        /* === STEP 4: Process data and conditionally transmit === */
        AverageLightSensorData();               /* Calculate mean of 16 samples */
        TransmitLightSensorDataToTerminal();    /* If > 1500, send "Bright!" + 20 values */
        StoreLightSensorDataToBuffer();         /* Advance circular buffer index */


        /* === OPTIONAL: Reduce CPU speed during idle period === */
        /* Uncomment to enable dynamic clock speed management for power savings */
        //ReduceCPUSpeed();
    }

    return ( EXIT_FAILURE );
}

/*******************************************************************************
 End of File
*/