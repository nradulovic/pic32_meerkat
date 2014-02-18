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

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
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


    return (CODEC_ERROR_NONE);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
