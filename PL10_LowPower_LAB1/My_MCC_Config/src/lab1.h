/*******************************************************************************
  Lab 1 Header File

  Company:
    Microchip Technology Inc.

  File Name:
    lab1.h

  Summary:
    Interface definitions for low-power standby helpers.

  Description:
    This header file provides function prototypes for Lab 1 helper routines
    related to ultra-low-power standby operation.
*******************************************************************************/

#ifndef LAB1_H
#define LAB1_H

// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes - Power Management
// *****************************************************************************
// *****************************************************************************

/**
 * @brief Configure voltage regulator for standby mode operation
 */
void WaitForULP(void);

#endif /* LAB1_H */