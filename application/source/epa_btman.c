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
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_PTT_PRESSED            "+PTT=P\r\n"
#define CONFIG_PTT_RELEASED           "+PTT=R\r\n"

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
    entry(stateSetAvrcp,            TOP)                                        \
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
    struct appTimer     timeout;
    uint32_t            retry;
    uint32_t            connectedProfiles;
    uint32_t            oldState;
    enum pttNotify      notify;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (struct wspace *, const esEvent *);
static esAction stateIdle               (struct wspace *, const esEvent *);
static esAction stateCmdBegin           (struct wspace *, const esEvent *);
static esAction stateCmdApply           (struct wspace *, const esEvent *);
static esAction stateCmdEnd             (struct wspace *, const esEvent *);
static esAction stateSetAuth            (struct wspace *, const esEvent *);
static esAction stateSetName            (struct wspace *, const esEvent *);
static esAction stateSetAudio           (struct wspace *, const esEvent *);
static esAction stateSetVolume          (struct wspace *, const esEvent *);
static esAction stateSetAvrcp           (struct wspace *, const esEvent *);
static esAction stateSetConnectionMask  (struct wspace *, const esEvent *);
static esAction stateSetDiscoveryMask   (struct wspace *, const esEvent *);
static esAction stateSetDiscoverable    (struct wspace *, const esEvent *);
static esAction stateHartBeat           (struct wspace *, const esEvent *);
static esAction stateHartCmdBegin       (struct wspace *, const esEvent *);
static esAction stateHartQuery          (struct wspace *, const esEvent *);
static esAction stateHartCmdEnd         (struct wspace *, const esEvent *);
static esAction stateHartSendData       (struct wspace *, const esEvent *);

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

static esAction stateInit(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            appTimerInit(&wspace->timeout);
            *(CONFIG_PTT_PORT)->tris |= (0x1 << CONFIG_PTT_PIN);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * wspace, const esEvent * event) {

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

static esAction stateCmdBegin(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_ENTER, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

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

static esAction stateCmdApply(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_REBOOT;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            esEvent *           request;
            esError             error;

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_EXIT, &request));

                if (error) {
                    return (ES_STATE_TRANSITION(stateCmdEnd));
                }
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_CODEC_ENABLE_AUDIO, &request));

                if (error) {
                    return (ES_STATE_TRANSITION(stateCmdEnd));
                }
                ES_ENSURE(esEpaSendEvent(Codec, request));
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

static esAction stateCmdEnd(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_INIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_EXIT, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAuth(struct wspace * wspace, const esEvent * event) {

    (void)wspace;
    
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_AUTH_NONE;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetName(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_DEVICE_NAME;
                ((struct evtBtReq *)request)->arg = CONFIG_BT_MODULE_NAME;
                ((struct evtBtReq *)request)->argSize = sizeof(CONFIG_BT_MODULE_NAME) - 1u;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetAudio(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_AUDIO_I2S;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetVolume(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_MUTE_OFF;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetAvrcp));
            } else {

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAvrcp(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));
            
            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_AVRCP;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetConnectionMask(struct wspace * wspace, const esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_CONNECTION_MASK;
                ((struct evtBtReq *)request)->arg = CONFIG_BT_PROFILE;
                ((struct evtBtReq *)request)->argSize = sizeof(CONFIG_BT_PROFILE) - 1u;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetDiscoveryMask(struct wspace * wspace, const esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_DISCOVERY_MASK;
                ((struct evtBtReq *)request)->arg = CONFIG_BT_PROFILE;
                ((struct evtBtReq *)request)->argSize = sizeof(CONFIG_BT_PROFILE) - 1u;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateSetDiscoverable(struct wspace * wspace, const esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_DISCOVERABLE;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {

            if (((const struct evtBtReply *)event)->status == BT_ERR_NONE) {

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

static esAction stateHartBeat(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Sync, request));
            }
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_HART_BEAT_PERIOD),
                EVT_TIMEOUT_);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            uint32_t        newState;
            
            newState = *(CONFIG_PTT_PORT)->port & (0x1 << CONFIG_PTT_PIN);

            if ((wspace->oldState != 0) && (newState == 0)) {
                esEvent *           request;
                esError             error;

                wspace->oldState = newState;
                wspace->notify   = PTT_PRESSED;

                ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_SYNC_REQUEST_TICK,
                    &request));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Sync, request));
                }
            } else if ((wspace->oldState == 0) && (newState != 0)) {
                esEvent *           request;
                esError             error;

                wspace->oldState = newState;
                wspace->notify   = PTT_RELEASED;

                ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_SYNC_REQUEST_TICK,
                    &request));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Sync, request));
                }
            } else {
                wspace->oldState = newState;
                appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_HART_BEAT_PERIOD),
                    EVT_TIMEOUT_);
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_TICK : {
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_TRANSITION(stateHartCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartCmdBegin(struct wspace * wspace, const esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_ENTER, &request));
            
            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

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

static esAction stateHartQuery(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_QUERY;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply;

            reply = (const struct evtBtReply *)event;

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


static esAction stateHartCmdEnd(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_EXIT, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

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

static esAction stateHartSendData(struct wspace * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_INIT : {
            esEvent *           data;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtSend), EVT_BT_SEND_DATA, &data));

            if (!error) {
                if (space->notify == PTT_PRESSED) {
                    ((struct evtBtSend *)data)->arg     =        CONFIG_PTT_PRESSED;
                    ((struct evtBtSend *)data)->argSize = sizeof(CONFIG_PTT_PRESSED);
                } else {
                    ((struct evtBtSend *)data)->arg     =        CONFIG_PTT_RELEASED;
                    ((struct evtBtSend *)data)->argSize = sizeof(CONFIG_PTT_RELEASED);
                }
                ES_ENSURE(esEpaSendEvent(BtDrv, data));
            }

            return (ES_STATE_TRANSITION(stateHartBeat));
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
 * END of epa_bt_man.c
 ******************************************************************************/
