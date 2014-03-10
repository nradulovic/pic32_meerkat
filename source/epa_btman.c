/*
 * File:    epa_btman.c
 * Author:  nenad
 * Details: Bluetooth Manager
 *
 * Created on February 27, 2014, 10:07 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "events.h"
#include "vtimer/vtimer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BT_MAN_TABLE(entry)                                                     \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)                                        \
    entry(stateCmdBegin,            TOP)                                        \
    entry(stateCmdApply,            TOP)                                        \
    entry(stateCmdEnd,              TOP)                                        \
    entry(stateSetAuth,             TOP)                                        \
    entry(stateSetName,             TOP)                                        \
    entry(stateSetDiscoveryMask,    TOP)                                        \
    entry(stateSetDiscoverable,     TOP)                                        \
    entry(stateSetAudio,            TOP)                                        \
    entry(stateHartBeat,            TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum BtManStateId {
    ES_STATE_ID_INIT(BT_MAN_TABLE)
};

enum localEvents {
    EVT_TIMEOUT_ = ES_EVENT_LOCAL_ID
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (void *, esEvent *);
static esAction stateIdle           (void *, esEvent *);
static esAction stateCmdBegin       (void *, esEvent *);
static esAction stateCmdApply       (void *, esEvent *);
static esAction stateCmdEnd         (void *, esEvent *);
static esAction stateSetAuth        (void *, esEvent *);
static esAction stateSetName        (void *, esEvent *);
static esAction stateSetAudio       (void *, esEvent *);
static esAction stateSetDiscoveryMask   (void *, esEvent *);
static esAction stateSetDiscoverable    (void *, esEvent *);
static esAction stateHartBeat       (void *, esEvent *);

static void btTimeoutHandler(void *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("BtMan", "Bluetooth manager", "Nenad Radulovic");

static const esSmTable BtManTable[] = ES_STATE_TABLE_INIT(BT_MAN_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine BtManEpa = ES_EPA_DEFINE(
    "BtMan",
    CONFIG_BT_MAN_PRIORITY,
    CONFIG_BT_MAN_QUEUE_SIZE);

const struct esSmDefine BtManSm = ES_SM_DEFINE(
    BtManTable,
    0,
    stateInit);

struct esEpa *   BtMan;

static esVTimer GlobalHartBeatTimer;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(void * space, esEvent * event) {

    (void)space;
    
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
    (void)space;

    switch (event->id) {
        case EVT_BT_NOTIFY_READY : {

            return (ES_STATE_TRANSITION(stateCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdBegin(void * space, esEvent * event) {

    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_ENTER,
                &event));
            esEpaSendEvent(BtDrv, event);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_NOTIFY_READY : {

            return (ES_STATE_TRANSITION(stateSetAuth));
        }
        case EVT_BT_REPLY : {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdApply(void * space, esEvent * event) {

        (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_REBOOT;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {
                ES_ENSURE(esEventCreate(
                    sizeof(esEvent),
                    EVT_BT_CMD_MODE_EXIT,
                    &event));
                esEpaSendEvent(BtDrv, event);

                return (ES_STATE_TRANSITION(stateHartBeat));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdEnd(void * space, esEvent * event) {

        (void)space;

    switch (event->id) {
        case ES_INIT : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_EXIT,
                &event));
            esEpaSendEvent(BtDrv, event);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAuth(void * space, esEvent * event) {

    (void)space;
    
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_AUTH_NONE;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetName));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetName(void * space, esEvent * event) {

    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DEVICE_NAME;
            ((struct BtReqEvent *)req)->arg = CONFIG_BT_MODULE_NAME;
            ((struct BtReqEvent *)req)->argSize = sizeof(CONFIG_BT_MODULE_NAME) - 1u;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetAudio));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAudio(void * space, esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_AUDIO_I2S;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetDiscoveryMask));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetDiscoveryMask(void * space, esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DISCOVERY_MASK;
            ((struct BtReqEvent *)req)->arg = CONFIG_BT_PROFILE;
            ((struct BtReqEvent *)req)->argSize = sizeof(CONFIG_BT_PROFILE) - 1u;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetDiscoverable));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetDiscoverable(void * space, esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DISCOVERABLE;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            esEpaSendEvent(BtDrv, req);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateCmdApply));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}


static esAction stateHartBeat(void * space, esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   enableAudio;
            
            esVTimerStart(
                &GlobalHartBeatTimer,
                ES_VTMR_TIME_TO_TICK_MS(10000u),
                btTimeoutHandler,
                NULL);

            esEventCreate(sizeof(esEvent), EVT_CODEC_ENABLE_AUDIO, &enableAudio);
            esEpaSendEvent(Codec, enableAudio);
            
            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            struct BtSendEvent * data;

            ES_ENSURE(esEventCreate(
                sizeof(struct BtSendEvent),
                EVT_BT_SEND_DATA,
                (esEvent **)&data));
            data->arg = "I am alive";
            data->argSize = sizeof("I am alive");
            esEpaSendEvent(BtDrv, (esEvent *)data);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static void btTimeoutHandler(void * arg) {
    struct esEvent *    timeout;
    esError             error;

    (void)arg;
    esVTimerStart(
        &GlobalHartBeatTimer,
        ES_VTMR_TIME_TO_TICK_MS(10000u),
        btTimeoutHandler,
        NULL);
    ES_ENSURE(error = esEventCreate(
        sizeof(struct esEvent),
        EVT_TIMEOUT_,
        &timeout));

    if (error == ES_ERROR_NONE) {
        esEpaSendAheadEvent(BtMan, timeout);
    }
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_bt_man.c
 ******************************************************************************/
