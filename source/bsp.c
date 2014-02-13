/*
 * File:   bsp.c
 * Author: nenad
 *
 * Created on February 8, 2014, 11:12 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_MAX_CPU_CLOCK            50000000ul

/*======================================================  LOCAL DATA TYPES  ==*/

struct systemInfo {
    struct systemClocks {
        uint32_t            cpu;
        uint32_t            peripheralBus;
    }                   clock;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/

static struct systemInfo GlobalSystemInfo;

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void bspInit(
    void) {

    GlobalSystemInfo.clock.cpu           = CONFIG_MAX_CPU_CLOCK;
    GlobalSystemInfo.clock.peripheralBus = SYSTEMConfigPerformance(CONFIG_MAX_CPU_CLOCK);

}

uint32_t bspGetPBfclock (
    void) {

    return (GlobalSystemInfo.clock.peripheralBus);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/