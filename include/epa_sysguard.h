/* 
 * File:    epa_sysguard.h
 * Author:  nenad
 * Details: Event Processing Agent for SysGuard
 *
 * Created on February 27, 2014, 10:05 PM
 */

#ifndef EPA_SYSGUARD_H
#define EPA_SYSGUARD_H

/*=========================================================  INCLUDE FILES  ==*/

#include "config/config_pins.h"
#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_SYSGUARD_QUEUE_SIZE      10
#define CONFIG_SYSGUARD_EVENT_BASE      10000

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum SysGuardEvents {
    EVT_SYSGUARD_NOTIFY_READY = CONFIG_SYSGUARD_EVENT_BASE
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine SysGuardEpa;
extern const struct esSmDefine  SysGuardSm;
extern struct esEpa *   SysGuard;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_sysguard.h
 ******************************************************************************/
#endif /* EPA_SYSGUARD_H */



