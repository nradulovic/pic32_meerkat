/* 
 * File:   config_mcu.h
 * Author: nenad
 *
 * Created on February 15, 2014, 11:02 PM
 */

#ifndef CONFIG_MCU_H_
#define	CONFIG_MCU_H_

#ifdef	__cplusplus
extern "C" {
#endif

#if (__PIC32_FEATURE_SET__ == 150)
#pragma config FPLLIDIV = DIV_2                                                 /* PLL Input Divider (2x Divider)                           */
#pragma config FPLLMUL = MUL_24                                                 /* PLL Multiplier (24x Multiplier)                          */
#pragma config FPLLODIV = DIV_2                                                 /* System PLL Output Clock Divider (PLL Divide by 2)        */

#pragma config FNOSC = FRCPLL                                                   /* Oscillator Selection Bits (Fast RC Osc with PLL)         */
#pragma config FSOSCEN = OFF                                                    /* Secondary Oscillator Enable (Disabled)                   */
#pragma config IESO = OFF                                                       /* Internal/External Switch Over (Disabled)                 */
#pragma config POSCMOD = OFF                                                    /* Primary Oscillator Configuration (Primary osc disabled)  */
#pragma config OSCIOFNC = OFF                                                   /* CLKO Output Signal Active on the OSCO Pin (Disabled)     */
#pragma config FPBDIV = DIV_1                                                   /* Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)           */
#pragma config FCKSM = CSECMD                                                   /* Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled) */
#pragma config WDTPS = PS1048576                                                /* Watchdog Timer Postscaler (1:1048576)                    */
#pragma config WINDIS = OFF                                                     /* Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode) */
#pragma config FWDTEN = OFF                                                     /* Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls)) */
#pragma config FWDTWINSZ = WISZ_25                                              /* Watchdog Timer Window Size (Window Size is 25%)          */

#pragma config JTAGEN = ON                                                      /* JTAG Enable (JTAG Port Enabled)                          */
#pragma config ICESEL = ICS_PGx1                                                /* ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1) */
#pragma config PWP = OFF                                                        /* Program Flash Write Protect (Disable)                    */
#pragma config BWP = OFF                                                        /* Boot Flash Write Protect bit (Protection Disabled)       */
#pragma config CP = OFF                                                         /* Code Protect (Protection Disabled)                       */
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_MCU_H_ */

