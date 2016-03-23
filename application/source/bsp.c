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
#include "driver/clock.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void bsp_init(
    void) {

    /*--  Initialize CPU peripherals  ----------------------------------------*/
    initClockDriver();
}



/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/