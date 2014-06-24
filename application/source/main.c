/* 
 * File:    main.c
 * Author:  nenad
 * Details: Main loop
 *
 * Created on February 7, 2014, 10:06 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "config/config_mcu.h"
#include "config/config_project.h"

#include <stdio.h>

#include "base/debug.h"
#include "eds/epa.h"
#include "vtimer/vtimer.h"

#include "bsp.h"
#include "events.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void processEvents(
    void);

/**@brief       Idle routine
 */
static void idle(
    void);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("main", "main loop", "Nenad Radulovic");

static uint8_t          StaticMemBuff[10240];

/*======================================================  GLOBAL VARIABLES  ==*/

esMem                   StaticMem = ES_MEM_INITIALIZER();
esMem                   HeapMem   = ES_MEM_INITIALIZER();

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void processEvents(
    void) {
    void *              heapBuff;

    /*--  Initialize virtual timers  -----------------------------------------*/
    esModuleVTimerInit();

    /*--  Initialize static memory allocator  --------------------------------*/
    ES_ENSURE(esMemInit(
        &esGlobalStaticMemClass,
        &StaticMem,
        StaticMemBuff,
        sizeof(StaticMemBuff),
        0));

    /*--  Initialize heap memory allocator  ----------------------------------*/
    ES_ENSURE(esMemAlloc(&StaticMem, 4096, &heapBuff));
    ES_ENSURE(esMemInit(
        &esGlobalHeapMemClass,
        &HeapMem,
        heapBuff,
        4096,
        0));

    /*--  Register a memory to use for events  -------------------------------*/
    ES_ENSURE(esEventRegisterMem(&HeapMem));

    /*--  Initialize EDS kernel  ---------------------------------------------*/
    ES_ENSURE(esEdsInit());

    /*--  Set application idle routine  --------------------------------------*/
    ES_ENSURE(esEdsSetIdle(idle));

    /*-- Create all required EPAs  -------------------------------------------*/
    ES_ENSURE(esEpaCreate(&BtDrvEpa,        &BtDrvSm,        &StaticMem, &BtDrv));
    ES_ENSURE(esEpaCreate(&BtManEpa,        &BtManSm,        &StaticMem, &BtMan));
    ES_ENSURE(esEpaCreate(&CodecEpa,        &CodecSm,        &StaticMem, &Codec));
    ES_ENSURE(esEpaCreate(&RadioEpa,        &RadioSm,        &StaticMem, &Radio));
    ES_ENSURE(esEpaCreate(&NotificationEpa, &NotificationSm, &StaticMem, &Notification));
    ES_ENSURE(esEpaCreate(&SerialEpa,       &SerialSm,       &StaticMem, &Serial1));
    ES_ENSURE(esEpaCreate(&SerialEpa,       &SerialSm,       &StaticMem, &Serial2));

    /*--  Start EPA execution  -----------------------------------------------*/
    ES_ENSURE(esEdsStart());
    ES_ENSURE(esMemTerm(&HeapMem));
}

static void idle(
    void) {
    /*
     * TODO: Put the sleep instruction here
     */
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

int main(
    int                 argc,
    char**              argv) {

    (void)argc;
    (void)argv;
    
    initBsp();                                                                  /* Initialize Board Support Package module                  */
    processEvents();

    return (EXIT_SUCCESS);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
