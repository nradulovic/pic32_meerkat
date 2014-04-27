/* 
 * File:   epa_radio.h
 * Author: nenad
 *
 * Created on April 27, 2014, 1:12 PM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_RADIO_H
#define EPA_RADIO_H

/*=========================================================  INCLUDE FILES  ==*/

#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_RADIO_PRIORITY           28
#define CONFIG_RADIO_QUEUE_SIZE         10
#define CONFIG_RADIO_EVENT_BASE         1300

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum radioEvents {
    EVT_RADIO_NO_DEVICE = CONFIG_RADIO_EVENT_BASE,
    EVT_RADIO_NO_NETW,
    EVT_RADIO_NETW_STRENGTH
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine RadioEpa;
extern const struct esSmDefine  RadioSm;
extern struct esEpa *   Radio;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_radio.h
 ******************************************************************************/
#endif /* EPA_RADIO_H */

