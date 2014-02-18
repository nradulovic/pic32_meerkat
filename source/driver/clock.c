/*
 * File:    clock.c
 * Author:  nenad
 * Details: Clock management
 *
 * Created on February 17, 2014, 1:13 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>
#include <xc.h>

#include "driver/clock.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_MAX_CPU_CLOCK            48000000ul

/*======================================================  LOCAL DATA TYPES  ==*/

struct systemClocks {
    uint32_t            cpu;
    uint32_t            peripheralBus;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static struct systemClocks GlobalSystemClocks;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initClock(
    void) {

    GlobalSystemClocks.cpu           = CONFIG_MAX_CPU_CLOCK;
    GlobalSystemClocks.peripheralBus = SYSTEMConfigPerformance(CONFIG_MAX_CPU_CLOCK);
}

uint32_t clockGetSystemClock(
    void) {

    return (GlobalSystemClocks.cpu);
}

uint32_t clockGetPeripheralClock (
    void) {

    return (GlobalSystemClocks.peripheralBus);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
