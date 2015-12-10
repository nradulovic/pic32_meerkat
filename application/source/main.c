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

static void process_events(
    void);

/**@brief       Idle routine
 */
static void idle(
    void);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

static nheap 					g_event_mem;
static uint8_t					g_event_mem_storage[12288];

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void process_events(void) 
{
	ncore_init();
    
	/*--  Initialize virtual timers  -----------------------------------------*/
	ncore_timer_enable();
	nsched_init();
    /*--  Initialize heap memory allocator  ----------------------------------*/
	nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));
    /*--  Register a memory to use for events  -------------------------------*/
	nevent_registerm_mem(&g_event_mem.mem_class);
    /*--  Set application idle routine  --------------------------------------*/
	neds_set_idle(NULL);

    /*-- Create all required EPAs  -------------------------------------------*/
	nepa_init(&g_bt_drv_epa, 		&g_bt_drv_define);
	nepa_init(&g_bt_man_epa, 		&g_bt_man_define);
	nepa_init(&g_codec_epa,  		&g_codec_define);
	nepa_init(&g_radio_epa,  		&g_radio_define);
	nepa_init(&g_notify_epa, 		&g_notify_define);
	nepa_init(&g_serial_bt_epa, 	&g_serial_bt_define);
	nepa_init(&g_serial_radio_epa, 	&g_serial_radio_define);
	nepa_init(&g_sync_bt_epa,       &g_sync_bt_define);
	nepa_init(&g_sync_radio_epa,    &g_sync_radio_define);
	nepa_init(&g_sync_radio_epa,    &g_sync_radio_define);

    /*--  Start EPA execution  -----------------------------------------------*/
	neds_run();
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
    
    bsp_init();
	process_events();

    return (EXIT_SUCCESS);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
