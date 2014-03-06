/*
 * File:    codec.c
 * Author:  nenad
 * Details: Audio codec driver
 *
 * Created on February 15, 2014, 11:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "vtimer/vtimer.h"
#include "driver/codec.h"
#include "driver/clock.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

struct spiHandle *  GlobalSpi;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initCodec(
    void) {
    /*
     * NOTE: This is the place to do general and system wide initializaion
     */
}

enum codecError codecOpen(
    struct spiHandle *  spi) {

    GlobalSpi = spi;
    cpumpEnable();                                                              /* We need 5V for output analog switch                      */
    clockSetOutput(CLOCK_OUT_1, CLOCK_OUT_SOURCE_SYSCLK, CLOCK_OUT_DIV_6);
    
    return (CODEC_ERROR_NONE);
}

void codecClose(
    void) {

    cpumpDisable();                                                             /* Disable 5V generation as we don't need it anymore        */
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
