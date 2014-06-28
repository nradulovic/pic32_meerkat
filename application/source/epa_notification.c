/*
 * File:   epa_notification.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */





/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

#include "events.h"
#include "bsp.h"
#include "app_timer.h"
#include "epa_notification.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define NOTIFICATION_TABLE(entry)                                               \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)                                        \
    entry(stateNoDev,               TOP)                                        \
    entry(stateNoNetw,              TOP)                                        \
    entry(stateNetHi,               TOP)                                        \
    entry(stateNetLo,               TOP)                                        \

/*======================================================  LOCAL DATA TYPES  ==*/

enum radioStateId {
    ES_STATE_ID_INIT(NOTIFICATION_TABLE)
};

struct wspace {
    struct appTimer     timeout;
    bool                toggle;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (void *, const esEvent *);
static esAction stateIdle           (void *, const esEvent *);
static esAction stateNoDev          (void *, const esEvent *);
static esAction stateNoNetw         (void *, const esEvent *);
static esAction stateNetHi          (void *, const esEvent *);
static esAction stateNetLo          (void *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Notification", "Notification LED", "Nenad Radulovic");

static const esSmTable NotificationTable[] = ES_STATE_TABLE_INIT(NOTIFICATION_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/


const struct esEpaDefine NotificationEpa = ES_EPA_DEFINE(
    "Notification",
    CONFIG_NOTIFICATION_PRIORITY,
    CONFIG_NOTIFICATION_QUEUE_SIZE);

const struct esSmDefine NotificationSm = ES_SM_DEFINE(
    NotificationTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *          Notification;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            appTimerInit(&wspace->timeout);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

enum localEventId {
    STATE_NO_DEV_ = ES_EVENT_LOCAL_ID,
    STATE_NO_NETW_
};

static esAction stateIdle(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case EVT_RADIO_NO_DEVICE : {

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        case EVT_RADIO_NO_NETW : {

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        case EVT_RADIO_NET_LOW : {

            return (ES_STATE_TRANSITION(stateNetLo));
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateNetHi));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNoDev(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(300), STATE_NO_DEV_);
            wspace->toggle = false;
            notifyLedOff();

            return (ES_STATE_HANDLED());
        }
        case STATE_NO_DEV_ : {
            if (wspace->toggle) {
                notifyLedRed();
            } else {
                notifyLedOff();
            }
            wspace->toggle = !wspace->toggle;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(300), STATE_NO_DEV_);

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NO_NETW : {

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        case EVT_RADIO_NET_LOW : {

            return (ES_STATE_TRANSITION(stateNetLo));
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateNetHi));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNoNetw(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(200), STATE_NO_NETW_);
            wspace->toggle = false;
            notifyLedOff();

            return (ES_STATE_HANDLED());
        }
        case STATE_NO_NETW_ : {
            if (wspace->toggle) {
                notifyLedGreen();
            } else {
                notifyLedOff();
            }
            wspace->toggle = !wspace->toggle;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(200), STATE_NO_NETW_);

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NO_DEVICE : {

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        case EVT_RADIO_NET_LOW : {

            return (ES_STATE_TRANSITION(stateNetLo));
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateNetHi));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNetHi(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            notifyLedGreen();

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NO_DEVICE : {

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        case EVT_RADIO_NO_NETW : {

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        case EVT_RADIO_NET_LOW : {

            return (ES_STATE_TRANSITION(stateNetLo));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNetLo(void * space, const esEvent * event) {
    (void)space;
    
    switch (event->id) {
        case ES_ENTRY : {
            notifyLedRed();

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NO_DEVICE : {

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        case EVT_RADIO_NO_NETW : {

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateNetHi));
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
 * END of epa_radio.c
 ******************************************************************************/