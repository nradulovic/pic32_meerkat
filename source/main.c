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

#include "test/test_spi.h"
#include "test/test_uart.h"
#include "bsp.h"
#include "events.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Execute enabled tests
 * @details     To enable/disable tests see @ref config_project.h and
 *              @ref config_test.h
 */
static void execTests(
    void);

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

static void execTests(
    void) {

#if (CONFIG_PROJ_TEST_SPI == 1) ||                                              \
    (CONFIG_PROJ_TEST_UART == 1) ||                                             \
    (CONFIG_PROJ_TEST_GPIO)
    PORTBbits.RB10 = 0;
    TRISBbits.TRISB10 = 0;
    PORTBbits.RB10 = 0;
    PORTBbits.RB10 = 1;
    PORTBbits.RB10 = 0;
    PORTBbits.RB10 = 1;
    for (;;) {
        execTestSpi();
        execTestUart();
    }
#endif
}

static void processEvents(
    void) {
    void *              heapBuff;

    /*--  Initialize PIC32 port  ---------------------------------------------*/
    /* NOTE: This will setup interrupt policy, PIC32 core timer, etc
     */
    esBaseInit();

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
    ES_ENSURE(esEpaKernelInit());

    /*--  Set application idle routine  --------------------------------------*/
    ES_ENSURE(esEpaKernelSetIdle(idle));

    /*-- Create all required EPAs  -------------------------------------------*/
    ES_ENSURE(esEpaCreate(&BtDrvEpa, &BtDrvSm, &StaticMem, &BtDrv));
    ES_ENSURE(esEpaCreate(&BtManEpa, &BtManSm, &StaticMem, &BtMan));
    ES_ENSURE(esEpaCreate(&CodecEpa, &CodecSm, &StaticMem, &Codec));

    /*--  Start EPA execution  -----------------------------------------------*/
    ES_ENSURE(esEpaKernelStart());
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
    execTests();                                                                /* Execute enabled tests                                    */
    processEvents();

    return (EXIT_SUCCESS);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
