/* 
 * File:    main.c
 * Author:  nenad
 * Details: Main loop
 *
 * Created on February 7, 2014, 10:06 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stddef.h>
#include <stdint.h>

#include "config/config_mcu.h"
#include "config/config_project.h"

#include "bsp.h"
#include "events.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

static struct nheap 			g_event_mem;
static uint8_t					g_event_mem_storage[16384];

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

int main(void) 
{
    bsp_init();
	
    /*--  Initialize core port system  ---------------------------------------*/
    ncore_init();
    
	/*--  Enable virtual timers  ---------------------------------------------*/
	ncore_timer_enable();

    /*--  Initialize heap memory allocator  ----------------------------------*/
	nheap_init(&g_event_mem, g_event_mem_storage, sizeof(g_event_mem_storage));
    
    /*--  Register a memory to use for events  -------------------------------*/
	nevent_register_mem(&g_event_mem.mem_class);
    
    /*--  Set application idle routine  --------------------------------------*/
	neds_set_idle(NULL);

    /*-- Create all required EPAs  -------------------------------------------*/


    /*--  Start EPA execution  -----------------------------------------------*/
	neds_run();

    return (0);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of main.c
 ******************************************************************************/
