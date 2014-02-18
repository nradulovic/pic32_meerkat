/*
 * File:    bsp.c
 * Author:  nenad
 * Details: Board Support Package
 *
 * Created on February 8, 2014, 11:12 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>
#include "bsp.h"
#include "driver/intr.h"
#include "driver/clock.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi.h"
#include "driver/uart.h"
#include "driver/codec.h"
#include "driver/intr.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initBsp(
    void) {

    initClock();
    initIntr();
    initGpio();                                                                 /* Initialize GPIO module                                   */
    initSpi();
    initUart();
    initCodec();
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/