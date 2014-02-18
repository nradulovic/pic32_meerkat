/* 
 * File:    main.c
 * Author:  nenad
 * Details: Main loop
 *
 * Created on February 7, 2014, 10:06 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "../config/config_mcu.h"
#include "../config/config_project.h"

#include <stdio.h>

#include "base/base.h"
#include "base/debug.h"
#include "eds/epa.h"

#include "test/test_spi.h"
#include "test/test_uart.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

/**@brief       Execute enabled tests
 * @details     To enable/disable tests see @ref config_project.h and
 *              @ref config_test.h
 */
static void execTests(
    void);

/**@brief       Idle routine
 */
static void idle(
    void);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("main", "main loop", "Nenad Radulovic");

static uint8_t          GlobalStaticBuff[10240];
static esMem            GlobalStatic = ES_MEM_INITIALIZER();
static esMem            GlobalHeap   = ES_MEM_INITIALIZER();

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void execTests(
    void) {

#if (CONFIG_PROJ_TEST_SPI == 1) ||                                              \
    (CONFIG_PROJ_TEST_UART == 1)
    for (;;) {
        PORTCSET = (0x1 << 1);
        LATCSET  = (0x1 << 1);
        TRISCCLR = (0x1 << 1);
        execTestSpi();
        execTestUart();
    }
#endif
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

    void *              heapBuff;

    (void)argc;
    (void)argv;

    initBsp();                                                                  /* Initialize Board Support Package module                  */
    execTests();                                                                /* Execute enabled tests                                    */

    ES_API_ENSURE(
        esMemInit(
            &esGlobalStaticMemClass,
            &GlobalStatic,
            GlobalStaticBuff,
            sizeof(GlobalStaticBuff),
            0));
    ES_API_ENSURE(esMemAlloc(&GlobalStatic, 4096, &heapBuff));
    ES_API_ENSURE(
        esMemInit(
            &esGlobalHeapMemClass,
            &GlobalHeap,
            heapBuff,
            4096,
            0));
    ES_API_ENSURE(esEventRegisterMem(&GlobalHeap));
    ES_API_ENSURE(esEpaKernelInit());
    ES_API_ENSURE(esEpaKernelSetIdle(idle));
    ES_API_ENSURE(esEpaKernelStart());
    ES_API_ENSURE(esMemTerm(&GlobalHeap));

    return (EXIT_SUCCESS);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
