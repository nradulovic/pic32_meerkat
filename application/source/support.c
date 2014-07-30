/*
 * File:    support.c
 * Author:  nenad
 * Details: Various support functions
 *
 * Created on February 7, 2014, 12:26 AM
 */

#include <string.h>
#include <xc.h>

#include "app_timer.h"
#include "base/debug.h"

static char file[100];
static char function[100];
static char expression[100];

static volatile unsigned int _excep_code;
static volatile unsigned int _excep_addr;
static volatile unsigned int _excep_vaddr;

/* NOTE: This function will be called when an error is detected within the
 *       eSolid packages. For details please see base/debug.h
 */
void userAssert(
    const struct esDebugReport * dbgReport) {

    strcpy(file,       dbgReport->modFile);
    strcpy(function,   dbgReport->fnName);
    strcpy(expression, dbgReport->expr);

    while (true);
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
