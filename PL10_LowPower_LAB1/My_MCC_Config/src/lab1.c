#include "definitions.h"                // SYS function prototypes
#include "lab1.h"
void WaitForULP(void) {
    /* Enable ULP voltage regulator for Standby mode */
    SUPC_REGS->SUPC_VREG |= SUPC_VREG_RUNSTDBY_Msk;

    while(!(SUPC_REGS->SUPC_STATUS & SUPC_VREG_RUNSTDBY_Msk));
    
    /* Note: SUPC_STATUS_ULPVREFRDY_Msk does not exist for this device.
     * The VREG is configured and will be active in standby mode.
     * Add delay if needed for voltage regulator stabilization. */
}