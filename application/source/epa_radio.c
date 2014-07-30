/*
 * File:   epa_radio.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "events.h"
#include "arch/intr.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CONFIG_RADIO_WAIT_PERIOD        400
#define CONFIG_RADIO_PAUSE_PERIOD       1100

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
    entry(stateDisableEchoPause,TOP)                                            \
    entry(statePoll,            TOP)                                            \
    entry(statePollPause,       TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum radioStateId {
    ES_STATE_ID_INIT(RADIO_TABLE)
};

enum localEvents {
    EVT_LOCAL_DISABLE_ECHO_WAIT = ES_EVENT_LOCAL_ID,
    EVT_LOCAL_DISABLE_ECHO_PAUSE,
    EVT_LOCAL_POLL_WAIT,
    EVT_LOCAL_POLL_PAUSE
};

struct wspace {
    struct appTimer     timeout;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (void *, const esEvent *);
static esAction stateDisableEcho        (void *, const esEvent *);
static esAction stateDisableEchoPause   (void *, const esEvent *);
static esAction statePoll               (void *, const esEvent *);
static esAction statePollPause          (void *, const esEvent *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Radio", "Radio manager", "Nenad Radulovic");

static const esSmTable RadioTable[] = ES_STATE_TABLE_INIT(RADIO_TABLE);

static int32_t          g_netwLowLimit;
static int32_t          g_netwHighLimit;

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

#define CONFIG_LOW_LIMIT                    -90
#define CONFIG_HI_LIMIT                     -87

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            struct eventSyncRegister * sync_register;
            esEvent *           request;
            esError             error;

            /* Set up SyncSerial EPA (Serial UART switcher) ------------------*/
            ES_ENSURE(error = esEventCreate(sizeof(struct eventSyncRegister),
                EVT_SYNC_REGISTER, &request));

            if (error) {
                return (ES_STATE_TRANSITION(stateInit));
            }
            sync_register = (struct eventSyncRegister *)request;
            sync_register->route.client = esEdsGetCurrent();
            sync_register->route.common = SerialRadio;
            sync_register->route.other  = SyncBt;
            ES_ENSURE(esEpaSendEvent(SyncRadio, request));

            /* Set up SerialRadio (Radio UART driver) ------------------------*/
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialOpen),
                EVT_SERIAL_OPEN, &request));

            if (error) {
                return (ES_STATE_TRANSITION(stateInit));
            }
            ((struct evtSerialOpen *)request)->client       = SyncRadio;
            ((struct evtSerialOpen *)request)->config.id    = CONFIG_RADIO_UART;
            ((struct evtSerialOpen *)request)->config.flags = UART_TX_ENABLE |
                                                              UART_RX_ENABLE |
                                                              UART_DATA_BITS_8 |
                                                              UART_STOP_BITS_1 |
                                                              UART_PARITY_NONE;
            ((struct evtSerialOpen *)request)->config.speed     = 38400;
            ((struct evtSerialOpen *)request)->config.isrPriority = ES_INTR_DEFAULT_ISR_PRIO;
            ((struct evtSerialOpen *)request)->config.remap.tx  = CONFIG_RADIO_UART_TX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rx  = CONFIG_RADIO_UART_RX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.cts = CONFIG_RADIO_UART_CTS_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rts = CONFIG_RADIO_UART_RTS_PIN;
            ES_ENSURE(esEpaSendEvent(SyncRadio, request));

            /* Finished COMM with SyncRadio ----------------------------------*/
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_DONE,
                &request));

            if (error) {
                return (ES_STATE_TRANSITION(stateInit));
            }
            ES_ENSURE(esEpaSendEvent(SyncRadio, request));

            /* Initialize this EPA -------------------------------------------*/
            g_netwLowLimit  = CONFIG_LOW_LIMIT;
            g_netwHighLimit = CONFIG_HI_LIMIT;
            appTimerInit(&wspace->timeout);

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
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_REQUEST,
                &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_GRANTED : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_WAIT_PERIOD),
                EVT_LOCAL_DISABLE_ECHO_WAIT);
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket),
                EVT_SERIAL_PACKET,
                &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_ECHO_DISABLE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_ECHO_DISABLE) - 1;
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
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

            return (ES_STATE_TRANSITION(stateDisableEchoPause));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet =
                (const struct evtSerialPacket *)event;

            appTimerCancel(&wspace->timeout);

            if ((strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE,   sizeof(RADIO_RESPONSE_ECHO_DISABLE)   - 1)  == 0) ||
                (strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE_E, sizeof(RADIO_RESPONSE_ECHO_DISABLE_E) - 1)  == 0)) {
                
                return (ES_STATE_TRANSITION(statePoll));
            } else {

                return (ES_STATE_TRANSITION(stateDisableEchoPause));
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDisableEchoPause(void * space, const esEvent * event)
{
    struct wspace *             wspace = space;
    
    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_PAUSE_PERIOD),
                EVT_LOCAL_DISABLE_ECHO_PAUSE);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_DISABLE_ECHO_PAUSE : {

            return (ES_STATE_TRANSITION(stateDisableEcho));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction statePoll(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_REQUEST,
                &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_GRANTED : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_WAIT_PERIOD),
                EVT_LOCAL_POLL_WAIT);
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET,
                &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_GET_STATE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_GET_STATE) - 1;
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_DONE, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(SyncRadio, request));
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

                if (strength > g_netwHighLimit) {
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NET_HIGH, &notify));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Notification, notify));
                    }
                } else if (strength < g_netwLowLimit) {
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
            
            return (ES_STATE_TRANSITION(statePollPause));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction statePollPause(void * space, const esEvent* event) {
    struct wspace *             wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_PAUSE_PERIOD),
                EVT_LOCAL_POLL_PAUSE);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_POLL_PAUSE : {

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
