/*
 * File:    epa_notification.h
 * Author:  nenad
 * Details: Notification Event Processing Agent
 *
 * Created on May 28, 2014, 10:09 PM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_NOTIFICATION_H
#define EPA_NOTIFICATION_H

/*=========================================================  INCLUDE FILES  ==*/

#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_NOTIFICATION_PRIORITY    1
#define CONFIG_NOTIFICATION_QUEUE_SIZE  10
#define CONFIG_NOTIFICATION_EVENT_BASE  1400

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine NotificationEpa;
extern const struct esSmDefine  NotificationSm;
extern struct esEpa *   Notification;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_notification.h
 ******************************************************************************/
#endif /* EPA_NOTIFICATION_H */


