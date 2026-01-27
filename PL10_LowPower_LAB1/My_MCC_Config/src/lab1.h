/*******************************************************************************
  Lab1 Header File

  Company:
    Microchip Technology Inc.

  File Name:
    lab1.h

  Summary:
    Interface definitions for low-power light sensor sampling application.

  Description:
    This header file provides function prototypes, constants, and macros for
    the Lab1 power-optimized light sensor sampling system. It defines the
    sampling parameters, threshold values, and interfaces for data collection,
    processing, transmission, and power management.
*******************************************************************************/

#ifndef LAB1_H
#define LAB1_H

/* =============================================================================
   Function Prototypes - Power Management
   ============================================================================= */

/**
 * @brief Configure voltage regulator for standby mode operation
 */
void WaitForULP(void);

#endif /* LAB3_H */