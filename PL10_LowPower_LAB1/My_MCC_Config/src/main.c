/*******************************************************************************
  Main Source File - Lab 1: Standby Wake Demo

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    Basic low-power initialization and standby entry.

  Description:
    This application demonstrates the minimum low-power flow:
    - Initialize system clocks, peripherals, and I/O pins
    - Configure the voltage regulator for ultra-low-power standby operation
    - Enter standby mode and service Harmony tasks in the main loop
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
#include "lab1.h"

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
     Configure all peripherals and power settings before entering
     the main loop.
    */
    
    /* System Initialization - Configure clocks, peripherals, and I/O pins */
    SYS_Initialize ( NULL );

    /* Configure voltage regulator for ultra-low-power standby mode */
    WaitForULP();

    /* Enter standby sleep mode to conserve power */
    PM_StandbyModeEnter ();

    /* =========================================================================
       MAIN APPLICATION LOOP
       =========================================================================
       Maintain Harmony tasks after wake-up events.
    */

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

