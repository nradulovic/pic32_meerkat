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
    &PORTA, &TRISA, &LATA,  &PORTASET,  &PORTACLR,  &PORTAINV
};

const struct gpio GlobalGpioB = {
    &PORTB, &TRISB, &LATB,  &PORTBSET,  &PORTBCLR,  &PORTBINV
};

const struct gpio GlobalGpioC = {
    &PORTC, &TRISC, &LATC,  &PORTCSET,  &PORTCCLR,  &PORTCINV
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initGpio(
    void) {
#if (__PIC32_FEATURE_SET == 150)
    ANSELA = 0u;
    ANSELB = 0u;
    ANSELC = 0u;
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/

