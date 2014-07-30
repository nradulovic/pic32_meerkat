/*
 * File:   epa_radio.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "events.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_RADIO_WAIT_PERIOD        400

#define RADIO_CMD_ECHO_DISABLE          "ATE0\r\n"
#define RADIO_RESPONSE_ECHO_DISABLE_E   "ATE0\r\r\nOK\r\n"
#define RADIO_RESPONSE_ECHO_DISABLE     "\r\nOK\r\n"

#define RADIO_CMD_GET_STATE             "AT+CSQ?\r\n"
#define RADIO_RESPONSE_NETW             "\r\n+CSQ:"
#define RADIO_RESPONSE_NO_NETW          "\r\n+CME ERROR: 30\r\n"
#define RADIO_RESPONSE_NOT_DATA         "\r\n+CME ERROR: 36\r\n"

#define RADIO_TABLE(entry)                                                      \
    entry(stateInit,            TOP)                                            \
    entry(stateDisableEcho,     TOP)                                            \
    entry(statePoll,            TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum radioStateId {
    ES_STATE_ID_INIT(RADIO_TABLE)
};

enum localEvents {
    EVT_LOCAL_DISABLE_ECHO_WAIT = ES_EVENT_LOCAL_ID,
    EVT_LOCAL_POLL_WAIT
};

struct wspace {
    struct appTimer     timeout;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit          (void *, const esEvent *);
static esAction stateDisableEcho   (void *, const esEvent *);
static esAction statePoll          (void *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Radio", "Radio manager", "Nenad Radulovic");

static const esSmTable RadioTable[] = ES_STATE_TABLE_INIT(RADIO_TABLE);

static int32_t          netwLowLimit;
static int32_t          netwHighLimit;

/*======================================================  GLOBAL VARIABLES  ==*/


const struct esEpaDefine RadioEpa = ES_EPA_DEFINE(
    "Radio",
    CONFIG_RADIO_PRIORITY,
    CONFIG_RADIO_QUEUE_SIZE);

const struct esSmDefine RadioSm = ES_SM_DEFINE(
    RadioTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *          Radio;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            esEvent *           request;
            esError             error;

            netwLowLimit  = -90;
            netwHighLimit = -87;
            appTimerInit(&wspace->timeout);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_REGISTER, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncBt, request));
            }

            return (ES_STATE_TRANSITION(stateDisableEcho));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDisableEcho(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
    switch (event->id) {
        case ES_EXIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncBt, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_TICK : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_WAIT_PERIOD),
                EVT_LOCAL_DISABLE_ECHO_WAIT);
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET,
                &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_ECHO_DISABLE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_ECHO_DISABLE) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_DISABLE_ECHO_WAIT : {
            esEvent *           notify;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_DEVICE, &notify));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Notification, notify));
            }

            return (ES_STATE_TRANSITION(stateDisableEcho));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if ((strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE,   sizeof(RADIO_RESPONSE_ECHO_DISABLE)   - 1)  == 0) ||
                (strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE_E, sizeof(RADIO_RESPONSE_ECHO_DISABLE_E) - 1)  == 0)){
                
                return (ES_STATE_TRANSITION(statePoll));
            } else {

                return (ES_STATE_TRANSITION(stateDisableEcho));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction statePoll(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_EXIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncBt, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_TICK : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_WAIT_PERIOD),
                EVT_LOCAL_POLL_WAIT);
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET,
                &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_GET_STATE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_GET_STATE) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_POLL_WAIT: {
            esError             error;
            esEvent *           notify;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_DEVICE, &notify));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Notification, notify));
            }

            return (ES_STATE_TRANSITION(stateDisableEcho));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;
            esError             error;
            esEvent *           notify;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if (strncmp(packet->data, RADIO_RESPONSE_NETW, sizeof(RADIO_RESPONSE_NETW) - 1)  == 0) {
                int32_t strength;

                strength = atoi(&((char *)(packet->data))[sizeof(RADIO_RESPONSE_NETW)]);

                if (strength > netwHighLimit) {
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NET_HIGH, &notify));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Notification, notify));
                    }
                } else if (strength < netwLowLimit) {
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NET_LOW, &notify));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Notification, notify));
                    }
                }
            } else {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_NETW, &notify));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Notification, notify));
                }
            }
            
            return (ES_STATE_TRANSITION(statePoll));
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
