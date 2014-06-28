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
    EVT_LOCAL_TIMEOUT = ES_EVENT_LOCAL_ID,
    EVT_LOCAL_CMD_END_TIMEOUT
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

static esAction stateInit               (void *, const esEvent *);
static esAction stateIdle               (void *, const esEvent *);
static esAction stateCmdBegin           (void *, const esEvent *);
static esAction stateCmdApply           (void *, const esEvent *);
static esAction stateCmdEnd             (void *, const esEvent *);
static esAction stateSetAuth            (void *, const esEvent *);
static esAction stateSetName            (void *, const esEvent *);
static esAction stateSetAudio           (void *, const esEvent *);
static esAction stateSetVolume          (void *, const esEvent *);
static esAction stateSetAvrcp           (void *, const esEvent *);
static esAction stateSetConnectionMask  (void *, const esEvent *);
static esAction stateSetDiscoveryMask   (void *, const esEvent *);
static esAction stateSetDiscoverable    (void *, const esEvent *);
static esAction stateHartBeat           (void *, const esEvent *);
static esAction stateHartCmdBegin       (void *, const esEvent *);
static esAction stateHartQuery          (void *, const esEvent *);
static esAction stateHartCmdEnd         (void *, const esEvent *);
static esAction stateHartSendData       (void *, const esEvent *);

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

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
    switch (event->id) {
        case ES_INIT : {
            appTimerInit(&wspace->timeout);
            wspace->retry = 0;
            *(CONFIG_PTT_PORT)->tris |= (0x1 << CONFIG_PTT_PIN);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(void * space, const esEvent * event) {
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

static esAction stateCmdBegin(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
            wspace->retry = 1;

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdApply(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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

            if (((struct evtBtReply *)event)->status != BT_ERR_NONE) {
                wspace->retry = 1;
            }
            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdEnd(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(3000), EVT_LOCAL_CMD_END_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_CMD_END_TIMEOUT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_EXIT, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            esEvent *           request;
            esError             error;

            if (((struct evtBtReply *)event)->status == BT_ERR_NONE) {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_CODEC_ENABLE_AUDIO, &request));
                
                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Codec, request));
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_READY, &request));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Sync, request));
                        wspace->oldState = *(CONFIG_PTT_PORT)->port & (0x1 << CONFIG_PTT_PIN);

                        if (wspace->retry == 0) {
                            return (ES_STATE_TRANSITION(stateHartBeat));
                        }
                    }
                }
            }
            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAuth(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetName(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAudio(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetVolume(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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

                return (ES_STATE_TRANSITION(stateSetConnectionMask));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetAvrcp(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetConnectionMask(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetDiscoveryMask(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetDiscoverable(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartBeat(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Sync, request));
            }
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_HART_BEAT_PERIOD),
                EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
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
                    EVT_LOCAL_TIMEOUT);
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

static esAction stateHartCmdBegin(void * space, const esEvent * event) {
    (void)space;

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

static esAction stateHartQuery(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
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


static esAction stateHartCmdEnd(void * space, const esEvent * event) {
    struct wspace * wspace = space;

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

static esAction stateHartSendData(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            esEvent *           data;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtSend), EVT_BT_SEND_DATA, &data));

            if (!error) {
                if (wspace->notify == PTT_PRESSED) {
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
