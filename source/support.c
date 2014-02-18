/*
 * File:    support.c
 * Author:  nenad
 * Details: Various support functions
 *
 * Created on February 7, 2014, 12:26 AM
 */

#include "base/debug.h"

/* NOTE: This function will be called when an error is detected within the
 *       eSolid packages. For details please see base/debug.h
 */
void userAssert(
    const struct esDebugReport * dbgReport) {

    /*
     * TODO: Report an error here
     */
    (void)dbgReport;
}