/* 
 * File:   main.c
 * Author: nenad
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
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void execTests(
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

#if (CONFIG_PROJ_TEST_SPI == 1)
    execTestSpi();
#endif /* (CONFIG_PROJ_TEST_SPI == 1) */

}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

int main(
    int                 argc,
    char**              argv) {

    void *              heapBuff;

    (void)argc;
    (void)argv;

    bspInit();

    execTests();

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

    return (EXIT_SUCCESS);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
