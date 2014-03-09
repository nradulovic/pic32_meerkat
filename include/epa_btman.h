/* 
 * File:    epa_sysguard.h
 * Author:  nenad
 * Details: Event Processing Agent for SysGuard
 *
 * Created on February 27, 2014, 10:05 PM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_BT_MAN_H
#define EPA_BT_MAN_H

/*=========================================================  INCLUDE FILES  ==*/

#include "config/config_pins.h"
#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_BT_MAN_PRIORITY          29
#define CONFIG_BT_MAN_QUEUE_SIZE        10
#define CONFIG_BT_MAN_EVENT_BASE        1100

/**@brief       BT module name
 */
#define CONFIG_BT_MODULE_NAME           "Meerkat"

/**@brief       BT module device name
 */
#define CONFIG_BT_DEVICE_NAME           "meerkat"

/**@brief       BT profiles
 * @details     See documentation about available options for BT profile
 */
#define CONFIG_BT_PROFILE               "06"

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum BtManEvents {
    EVT_BT_MAN_NOTIFY_READY = CONFIG_BT_MAN_EVENT_BASE
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine BtManEpa;
extern const struct esSmDefine  BtManSm;
extern struct esEpa *   BtMan;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_bt_man.h
 ******************************************************************************/
#endif /* EPA_BT_MAN_H */



