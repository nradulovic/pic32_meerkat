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

#define CONFIG_SYSTEM_CLOCK             48000000ul

/*======================================================  LOCAL DATA TYPES  ==*/

struct clocks {
    uint32_t            system;
    uint32_t            peripheralBus;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static struct clocks GlobalClocks;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initClock(
    void) {

    GlobalClocks.system        = CONFIG_SYSTEM_CLOCK;
    GlobalClocks.peripheralBus = SYSTEMConfigPerformance(CONFIG_SYSTEM_CLOCK);
}

uint32_t clockGetSystemClock(
    void) {

    return (GlobalClocks.system);
}

uint32_t clockGetPeripheralClock (
    void) {

    return (GlobalClocks.peripheralBus);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
