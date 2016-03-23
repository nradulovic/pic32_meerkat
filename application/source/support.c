/*
 * File:    support.c
 * Author:  nenad
 * Details: Various support functions
 *
 * Created on February 7, 2014, 12:26 AM
 */

#include <string.h>
#include <xc.h>

#include "base/debug.h"


static volatile unsigned int _excep_code;
static volatile unsigned int _excep_addr;
static volatile unsigned int _excep_vaddr;

/* NOTE: This function will be called when an error is detected within the
 *       eSolid packages. For details please see base/debug.h
 */
PORT_C_NORETURN
void hook_at_assert(
    const struct ncomponent_info * component_info,
    const char *     			fn,
    uint32_t                    line,
    const char *     			expr,
    const char *     			msg)
{
    (void)component_info;
    (void)fn;
    (void)line;
    (void)expr;
    (void)msg;
    
    for (;;);
}

void _general_exception_handler(void) 
{     
    volatile          int code;
    volatile          int intlev;

    _excep_vaddr = _CP0_GET_BADVADDR();
    _excep_code  = _CP0_GET_CAUSE();
    _excep_code  = (_excep_code & 0x0000007C) >> 2;

    /* Try to show what has happened. */
    code =  (_excep_code & 0x000000FF) >> 2;
    intlev =(_excep_code & 0x0000FF00) >> 10;

    while(1);               
}
