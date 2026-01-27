/*******************************************************************************
  Lab1 Source File

  Company:
    Microchip Technology Inc.

  File Name:
    lab1.c

  Summary:
    Low-power light sensor sampling and data transmission application.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/
 
#include "definitions.h"                // SYS function prototypes
#include "lab1.h"
#include <stdio.h>
#include <string.h>

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