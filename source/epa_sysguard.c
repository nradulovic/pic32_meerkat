/*
 * File:    epa_sysguard.h
 * Author:  nenad
 * Details: Event Processing Agent for SysGuard
 *
 * Created on February 27, 2014, 10:07 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "events.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SYSGUARD_TABLE(entry)                                                   \
    entry(stateInit,            TOP)                                            \
    entry(stateIdle,            TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum SysGuardStateId {
    ES_STATE_ID_INIT(SYSGUARD_TABLE)
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit      (void *, esEvent *);
static esAction stateIdle      (void *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("SysGuard", "System guard", "Nenad Radulovic");

static const esSmTable SysGuardTable[] = ES_STATE_TABLE_INIT(SYSGUARD_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine SysGuardEpa = ES_EPA_DEFINE(
    "SysGuard",
    31,
    CONFIG_SYSGUARD_QUEUE_SIZE);

const struct esSmDefine SysGuardSm = ES_SM_DEFINE(
    SysGuardTable,
    0,
    stateInit);

struct esEpa *   SysGuard;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(void * space, esEvent * event) {
    switch (event->id) {
        case ES_INIT : {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(void * space, esEvent * event) {
    switch (event->id) {
        case EVT_SYSGUARD_NOTIFY_READY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtEvent), EVT_BT_DRV_REQ, &req));
            ((struct BtEvent *)req)->cmd = BT_DRV_SET_AUTH_NONE;
            ((struct BtEvent *)req)->arg = NULL;
            ((struct BtEvent *)req)->argSize = 0;
            esEpaSendEvent(event->producer, req);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
