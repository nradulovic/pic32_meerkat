/*
 * File:    spi.c
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 17, 2014, 1:41 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>

#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

const struct gpio GlobalGpioA = {
    &PORTA, &TRISA, &LATA,  &LATASET,  &LATACLR,  &PORTAINV
};

const struct gpio GlobalGpioB = {
    &PORTB, &TRISB, &LATB,  &LATBSET,  &LATBCLR,  &PORTBINV
};

const struct gpio GlobalGpioC = {
    &PORTC, &TRISC, &LATC,  &LATCSET,  &LATCCLR,  &PORTCINV
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGpio(
    void) {
#if (__PIC32_FEATURE_SET == 150)
    /*
     * Setup all pins as digital IO with push-pull mode
     */
    ANSELA = 0u;
    ODCA   = 0u;
    ANSELB = 0u;
    ODCB   = 0u;
    ANSELC = 0u;
    ODCC   = 0u;
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/

