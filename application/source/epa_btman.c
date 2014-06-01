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
#include "conv.h"
#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BT_MAN_TABLE(entry)                                                     \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)                                        \
    entry(stateCmdBegin,            TOP)                                        \
    entry(stateCmdApply,            TOP)                                        \
    entry(stateCmdEnd,              TOP)                                        \
    entry(stateSetAuth,             TOP)                                        \
    entry(stateSetName,             TOP)                                        \
    entry(stateSetConnectionMask,   TOP)                                        \
    entry(stateSetDiscoveryMask,    TOP)                                        \
    entry(stateSetDiscoverable,     TOP)                                        \
    entry(stateSetAudio,            TOP)                                        \
    entry(stateSetVolume,           TOP)                                        \
    entry(stateHartCmdBegin,        TOP)                                        \
    entry(stateHartQuery,           TOP)                                        \
    entry(stateHartCmdEnd,          TOP)                                        \
    entry(stateHartSendData,        TOP)                                        \
    entry(stateHartBeat,            TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum BtManStateId {
    ES_STATE_ID_INIT(BT_MAN_TABLE)
};

enum localEvents {
    EVT_TIMEOUT_ = ES_EVENT_LOCAL_ID
};

enum pttNotify {
    PTT_PRESSED,
    PTT_RELEASED
}; 

/**@brief       Epa workspace
 */
struct wspace {
    struct esVTimer     timeout;
    uint32_t            retry;
    uint32_t            connectedProfiles;
    uint32_t            oldState;
    enum pttNotify      notify;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, esEvent *);
static esAction stateIdle           (struct wspace *, esEvent *);
static esAction stateCmdBegin       (struct wspace *, esEvent *);
static esAction stateCmdApply       (struct wspace *, esEvent *);
static esAction stateCmdEnd         (struct wspace *, esEvent *);
static esAction stateSetAuth        (struct wspace *, esEvent *);
static esAction stateSetName        (struct wspace *, esEvent *);
static esAction stateSetAudio       (struct wspace *, esEvent *);
static esAction stateSetVolume      (struct wspace *, esEvent *);
static esAction stateSetConnectionMask  (struct wspace *, esEvent *);
static esAction stateSetDiscoveryMask   (struct wspace *, esEvent *);
static esAction stateSetDiscoverable    (struct wspace *, esEvent *);
static esAction stateHartBeat       (struct wspace *, esEvent *);
static esAction stateHartCmdBegin   (struct wspace *, esEvent *);
static esAction stateHartQuery      (struct wspace *, esEvent *);
static esAction stateHartCmdEnd     (struct wspace *, esEvent *);
static esAction stateHartSendData   (struct wspace *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

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
    sizeof(struct wspace),
    stateInit);

struct esEpa *   BtMan;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * wspace, esEvent * event) {

    (void)wspace;
    
    switch (event->id) {
        case ES_INIT : {
            esVTimerInit(&wspace->timeout);
            *(CONFIG_PTT_PORT)->tris |= (0x1 << CONFIG_PTT_PIN);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case EVT_BT_NOTIFY_READY : {

            return (ES_STATE_TRANSITION(stateCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdBegin(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_ENTER,
                &event));
            ES_ENSURE(esEpaSendEvent(BtDrv, event));

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

static esAction stateCmdApply(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_REBOOT;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {
                ES_ENSURE(esEventCreate(
                    sizeof(esEvent),
                    EVT_BT_CMD_MODE_EXIT,
                    &event));
                ES_ENSURE(esEpaSendEvent(BtDrv, event));
                ES_ENSURE(esEventCreate(
                    sizeof(esEvent),
                    EVT_CODEC_ENABLE_AUDIO,
                    &event));
                ES_ENSURE(esEpaSendEvent(Codec, event));
                wspace->oldState = *(CONFIG_PTT_PORT)->port & (0x1 << CONFIG_PTT_PIN);

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

static esAction stateCmdEnd(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_INIT : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_EXIT,
                &event));
            ES_ENSURE(esEpaSendEvent(BtDrv, event));

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAuth(struct wspace * wspace, esEvent * event) {

    (void)wspace;
    
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_AUTH_NONE;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

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

static esAction stateSetName(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DEVICE_NAME;
            ((struct BtReqEvent *)req)->arg = CONFIG_BT_MODULE_NAME;
            ((struct BtReqEvent *)req)->argSize = sizeof(CONFIG_BT_MODULE_NAME) - 1u;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

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

static esAction stateSetAudio(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_AUDIO_I2S;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetVolume));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetVolume(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_MUTE_OFF;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((struct BtReplyEvent *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetConnectionMask));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetConnectionMask(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_CONNECTION_MASK;
            ((struct BtReqEvent *)req)->arg = CONFIG_BT_PROFILE;
            ((struct BtReqEvent *)req)->argSize = sizeof(CONFIG_BT_PROFILE) - 1u;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

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

static esAction stateSetDiscoveryMask(struct wspace * wspace, esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DISCOVERY_MASK;
            ((struct BtReqEvent *)req)->arg = CONFIG_BT_PROFILE;
            ((struct BtReqEvent *)req)->argSize = sizeof(CONFIG_BT_PROFILE) - 1u;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

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

static esAction stateSetDiscoverable(struct wspace * wspace, esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_SET_DISCOVERABLE;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

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

static esAction stateHartBeat(struct wspace * wspace, esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esVTimerCancel(&wspace->timeout);
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_HART_BEAT_PERIOD),
                btTimeoutHandler,
                NULL);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            uint32_t        newState;
            
            newState = *(CONFIG_PTT_PORT)->port & (0x1 << CONFIG_PTT_PIN);

            if ((wspace->oldState != 0) && (newState == 0)) {
                wspace->oldState = newState;
                wspace->notify   = PTT_PRESSED;

                return (ES_STATE_TRANSITION(stateHartCmdBegin));
            } else if ((wspace->oldState == 0) && (newState != 0)) {
                wspace->oldState = newState;
                wspace->notify   = PTT_RELEASED;

                return (ES_STATE_TRANSITION(stateHartCmdBegin));
            } else {
                wspace->oldState = newState;
                esVTimerCancel(&wspace->timeout);
                esVTimerStart(
                    &wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_HART_BEAT_PERIOD),
                    btTimeoutHandler,
                    NULL);

                return (ES_STATE_HANDLED());
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartCmdBegin(struct wspace * wspace, esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_ENTER,
                &event));
            ES_ENSURE(esEpaSendEvent(BtDrv, event));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_NOTIFY_READY : {

            return (ES_STATE_TRANSITION(stateHartQuery));
        }
        case EVT_BT_REPLY : {

            return (ES_STATE_TRANSITION(stateHartBeat));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartQuery(struct wspace * wspace, esEvent * event) {

    (void)wspace;
    
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *   req;

            ES_ENSURE(esEventCreate(sizeof(struct BtReqEvent), EVT_BT_REQ, &req));
            ((struct BtReqEvent *)req)->cmd = BT_QUERY;
            ((struct BtReqEvent *)req)->arg = NULL;
            ((struct BtReqEvent *)req)->argSize = 0;
            ES_ENSURE(esEpaSendEvent(BtDrv, req));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            struct BtReplyEvent * reply;

            reply = (struct BtReplyEvent *)event;

            if (reply->argSize == 0u) {

                return (ES_STATE_TRANSITION(stateHartQuery));
            } else {
                wspace->connectedProfiles = convAsciiToHex(reply->arg[1]);

                return (ES_STATE_TRANSITION(stateHartCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

#define CONFIG_PTT_PRESSED            "+PTT=P\r\n"
#define CONFIG_PTT_RELEASED           "+PTT=R\r\n"

static esAction stateHartCmdEnd(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_INIT : {
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_CMD_MODE_EXIT,
                &event));
            ES_ENSURE(esEpaSendEvent(BtDrv, event));

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_NOTIFY_READY : {
            
            if (wspace->connectedProfiles & BT_DRV_PROFILE_SPP_Msk) {

                return (ES_STATE_TRANSITION(stateHartSendData));
            } else {

                return (ES_STATE_TRANSITION(stateHartBeat));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartSendData(struct wspace * space, esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_INIT : {
            struct BtSendEvent * data;

            ES_ENSURE(esEventCreate(
                sizeof(struct BtSendEvent),
                EVT_BT_SEND_DATA,
                (esEvent **)&data));

            if (space->notify == PTT_PRESSED) {
                data->arg = CONFIG_PTT_PRESSED;
                data->argSize = sizeof(CONFIG_PTT_PRESSED);
            } else {
                data->arg = CONFIG_PTT_RELEASED;
                data->argSize = sizeof(CONFIG_PTT_RELEASED);
            }
            
            ES_ENSURE(esEpaSendEvent(BtDrv, (esEvent *)data));

            return (ES_STATE_TRANSITION(stateHartBeat));
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
