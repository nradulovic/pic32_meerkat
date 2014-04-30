/*
 * File:    support.c
 * Author:  nenad
 * Details: Various support functions
 *
 * Created on February 7, 2014, 12:26 AM
 */

#include <string.h>

#include "app_timer.h"
#include "base/debug.h"

static char file[100];
static char function[100];
static char expression[100];

/* NOTE: This function will be called when an error is detected within the
 *       eSolid packages. For details please see base/debug.h
 */
void userAssert(
    const struct esDebugReport * dbgReport) {

    strcpy(file,       dbgReport->modFile);
    strcpy(function,   dbgReport->fnName);
    strcpy(expression, dbgReport->expr);
}

