/*
 * File:    epa_btman.c
 * Author:  nenad
 * Details: Bluetooth Manager
 *
 * Created on February 27, 2014, 10:07 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "neon_eds.h"
#include "events.h"
#include "conv.h"
#include "driver/gpio.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/

enum local_events 
{
    EVT_LOCAL_TIMEOUT = NEVENT_USER_ID,
    EVT_LOCAL_RESET_SETTINGS_TIMEOUT,
    EVT_LOCAL_RESET_CMD_TIMEOUT,
    EVT_LOCAL_SET_CMD_TIMEOUT,
    EVT_LOCAL_ACTION_CMD_TIMEOUT,
    EVT_LOCAL_CMD_END_TIMEOUT
};

/**@brief       Epa workspace
 */
struct wspace 
{
	struct netimer		timeout;
    uint32_t            retry;
    uint32_t            connected_profiles;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static naction state_init(struct nsm *, const struct nevent *);
static naction state_idle(struct nsm *, const struct nevent *);
static naction state_reset_cmd_begin(struct nsm *, const struct nevent *);
static naction state_reset_settings(struct nsm *, const struct nevent *);
static naction state_reset_cmd_end(struct nsm *, const struct nevent *);
static naction state_set_cmd_begin(struct nsm *, const struct nevent *);
static naction state_set_auth(struct nsm *, const struct nevent *);
static naction state_set_name(struct nsm *, const struct nevent *);
static naction state_set_audio(struct nsm *, const struct nevent *);
static naction state_set_connection_mask(struct nsm *, const struct nevent *);
static naction state_set_discovery_mask(struct nsm *, const struct nevent *);
static naction state_set_speaker_volume(struct nsm *, const struct nevent *);
static naction state_set_cmd_end(struct nsm *, const struct nevent *);
static naction state_action_cmd_begin(struct nsm *, const struct nevent *);
static naction state_action_discoverable(struct nsm *, const struct nevent *);
static naction state_action_volume(struct nsm *, const struct nevent *);
static naction state_action_avrcp(struct nsm *, const struct nevent *);
static naction state_action_cmd_end(struct nsm *, const struct nevent *);
static naction state_heart_beat(struct nsm *, const struct nevent *);
static naction state_heart_cmd_begin(struct nsm *, const struct nevent *);

static esAction stateInit               (void *, const esEvent *);
static esAction stateIdle               (void *, const esEvent *);
static esAction stateResetCmdBegin      (void *, const esEvent *);
static esAction stateResetSettings      (void *, const esEvent *);
static esAction stateResetCmdEnd        (void *, const esEvent *);
static esAction stateSetCmdBegin        (void *, const esEvent *);
static esAction stateSetAuth            (void *, const esEvent *);
static esAction stateSetName            (void *, const esEvent *);
static esAction stateSetAudio           (void *, const esEvent *);
static esAction stateSetConnectionMask  (void *, const esEvent *);
static esAction stateSetDiscoveryMask   (void *, const esEvent *);
static esAction stateSetSpeakerVolume   (void *, const esEvent *);
static esAction stateSetCmdEnd          (void *, const esEvent *);
static esAction stateActionCmdBegin     (void *, const esEvent *);
static esAction stateActionDiscoverable (void *, const esEvent *);
static esAction stateActionVolume       (void *, const esEvent *);
static esAction stateActionAvrcp        (void *, const esEvent *);
static esAction stateActionCmdEnd       (void *, const esEvent *);
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

            return (ES_STATE_TRANSITION(stateResetCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateResetCmdBegin(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            wspace->retry = 0;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_ENTER, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateResetSettings));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateResetCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateResetSettings(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_DEFAULTS;
                ((struct evtBtReq *)request)->arg = NULL;
                ((struct evtBtReq *)request)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_RESET_SETTINGS_TIMEOUT : {

            return (ES_STATE_TRANSITION(stateResetCmdEnd));
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {
                appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(30),
                    EVT_LOCAL_RESET_SETTINGS_TIMEOUT);

                return (ES_STATE_HANDLED());
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateResetCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateResetCmdEnd(void * space, const esEvent * event) {
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {
                if (wspace->retry == 0) {
                    appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(100),
                        EVT_LOCAL_RESET_CMD_TIMEOUT);

                    return (ES_STATE_HANDLED());
                }
            }
            
            return (ES_STATE_TRANSITION(stateResetCmdBegin));             /* Retry from beginning */
        }
        case EVT_LOCAL_RESET_CMD_TIMEOUT : {

            return (ES_STATE_TRANSITION(stateSetCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetCmdBegin(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            wspace->retry = 0;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_ENTER, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetAuth));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateSetCmdEnd));
            }
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetName));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateSetCmdEnd));
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetAudio));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateSetCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

#define CONFIG_AUDIO_PATH               "0102"

static esAction stateSetAudio(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_AUDIO_PATH;
                ((struct evtBtReq *)request)->arg = CONFIG_AUDIO_PATH;
                ((struct evtBtReq *)request)->argSize = sizeof(CONFIG_AUDIO_PATH) - 1u;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetConnectionMask));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateSetCmdEnd));
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateSetDiscoveryMask));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateSetCmdEnd));
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status != BT_ERR_NONE) {
                wspace->retry = 1;
            }

            return (ES_STATE_TRANSITION(stateSetCmdEnd));
            /* BT does not recognize this command */
#if 0
            return (ES_STATE_TRANSITION(stateSetSpeakerVolume));
#endif
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

#define CONFIG_SPEAKER_VOLUME           "0A"

static esAction stateSetSpeakerVolume(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReq), EVT_BT_REQ, &request));

            if (!error) {
                ((struct evtBtReq *)request)->cmd = BT_SET_SPEAKER_VOLUME;
                ((struct evtBtReq *)request)->arg = CONFIG_SPEAKER_VOLUME;
                ((struct evtBtReq *)request)->argSize = sizeof(CONFIG_SPEAKER_VOLUME) - 1u;
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status != BT_ERR_NONE) {
                wspace->retry = 1;
            }

            return (ES_STATE_TRANSITION(stateSetCmdEnd));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateSetCmdEnd(void * space, const esEvent * event) {
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {
                if (wspace->retry == 0) {
                    appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(100),
                        EVT_LOCAL_SET_CMD_TIMEOUT);

                    return (ES_STATE_HANDLED());
                }
            }

            return (ES_STATE_TRANSITION(stateSetCmdBegin));
        }
        case EVT_LOCAL_SET_CMD_TIMEOUT : {

            return (ES_STATE_TRANSITION(stateActionCmdBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateActionCmdBegin(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            wspace->retry = 0;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_ENTER, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateActionDiscoverable));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateActionCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateActionDiscoverable(void * space, const esEvent * event) {
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateActionVolume));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateActionCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateActionVolume(void * space, const esEvent * event) {
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateActionAvrcp));
            } else {
                wspace->retry = 1;

                return (ES_STATE_TRANSITION(stateActionCmdEnd));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateActionAvrcp(void * space, const esEvent * event) {
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
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status != BT_ERR_NONE) {
                wspace->retry = 1;
            }
            
            return (ES_STATE_TRANSITION(stateActionCmdEnd));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateActionCmdEnd(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_CMD_MODE_EXIT, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(BtDrv, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {
                if (wspace->retry == 0) {
                    appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(100),
                        EVT_LOCAL_ACTION_CMD_TIMEOUT);

                    return (ES_STATE_HANDLED());
                }
            }

            return (ES_STATE_TRANSITION(stateActionCmdBegin));            /* Retry from beginning */
        }
        case EVT_LOCAL_ACTION_CMD_TIMEOUT : {
            esEvent *           request;
            esError             error;
            
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_CODEC_ENABLE_AUDIO,
                &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Codec, request));

                return (ES_STATE_TRANSITION(stateHartBeat));
            }

            return (ES_STATE_TRANSITION(stateActionCmdBegin));            /* Retry from beginning */
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateHartBeat(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case EVT_BT_MAN_SEND : {
            struct eventBtManSend * send = (struct eventBtManSend *)event;
            esEvent *           data;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtSend),
                EVT_BT_SEND_DATA, &data));

            if (!error) {
                struct evtBtSend * data_ = (struct evtBtSend *)data;
                data_->arg               = (char *)send->data;
                data_->argSize           = send->size;
                ES_ENSURE(esEpaSendEvent(BtDrv, data));
            }
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
        case EVT_BT_REPLY : {
            const struct evtBtReply * reply = (const struct evtBtReply *)event;

            if (reply->status == BT_ERR_NONE) {

                return (ES_STATE_TRANSITION(stateHartQuery));
            } else {

                return (ES_STATE_TRANSITION(stateHartCmdBegin));
            }
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
        case EVT_BT_REPLY : {
           
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
    (void)space;

    switch (event->id) {
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
