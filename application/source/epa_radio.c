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

/**@brief       Nominal polling period for network state
 */
#define CONFIG_RADIO_POLL_NOM_PERIOD    1000

/**@brief       Polling period when no network is detected
 */
#define CONFIG_RADIO_POLL_NONETW_PERIOD 2000

#define CONFIG_RADIO_POLL_NODEV_PERIOD  4000

#define CONFIG_RADIO_UART_TIMEOUT       20

#define RADIO_CMD_ECHO_DISABLE          "ATE0\r\n"
#define RADIO_RESPONSE_ECHO_DISABLE_E   "ATE0\r\r\nOK\r\n"
#define RADIO_RESPONSE_ECHO_DISABLE     "\r\nOK\r\n"

#define RADIO_CMD_CONFIRM               "AT\r\n"
#define RADIO_RESPONSE_CONFIRM          "\r\nOK\r\n"

#define RADIO_CMD_GET_STATE             "AT+CSQ?\r\n"
#define RADIO_RESPONSE_NETW             "\r\n+CSQ:"
#define RADIO_RESPONSE_NO_NETW          "\r\n+CME ERROR: 30\r\n"
#define RADIO_RESPONSE_NOT_DATA         "\r\n+CME ERROR: 36\r\n"

#define EVENT_NOTIFY_NO_NETW            (0x1u << 0)
#define EVENT_NOTIFY_NETW_GOOD          (0x1u << 1)
#define EVENT_NOTIFY_NETW_BAD           (0x1u << 2)
#define EVENT_NOTIFY_NO_DEVICE          (0x1u << 3)

#define RADIO_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \
    entry(stateEnableEcho,          TOP)                                        \
    entry(stateEnableEchoAt,        TOP)                                        \
    entry(stateNoNetw,              TOP)                                        \
    entry(stateNetwLo,              TOP)                                        \
    entry(stateNetwHi,              TOP)                                        \
    entry(stateNoDev,               TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum radioStateId {
    ES_STATE_ID_INIT(RADIO_TABLE)
};

enum localEvents {
    EVT_ECHO_POLL_ = ES_EVENT_LOCAL_ID,
    EVT_NODEV_POLL_,
    EVT_NETWLO_POLL_,
    EVT_NETWHI_POLL_,
    EVT_IDLE_TIMEOUT_,
    EVT_UART_ECHO_TIMEOUT_,
    EVT_UART_ECHO_AT_TIMEOUT_,
    EVT_UART_NETWLO_TIMEOUT_,
    EVT_UART_NETWHI_TIMEOUT_,
    EVT_UART_NODEV_TIMEOUT_,
    EVT_UART_NONETW_TIMEOUT_
};

struct wspace {
    struct appTimer     timeout;
    struct appTimer     timer;
    uint32_t            timerTicks;
    uint32_t            eventNotify;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, const esEvent *);
static esAction stateEnableEcho     (struct wspace *, const esEvent *);
static esAction stateEnableEchoAt   (struct wspace *, const esEvent *);
static esAction stateNoNetw         (struct wspace *, const esEvent *);
static esAction stateNetwLo         (struct wspace *, const esEvent *);
static esAction stateNetwHi         (struct wspace *, const esEvent *);
static esAction stateNoDev          (struct wspace *, const esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static void initRadio(struct wspace *);

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

static esAction stateInit(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            netwLowLimit  = -90;
            netwHighLimit = -87;
            initRadio(wspace);

            return (ES_STATE_TRANSITION(stateEnableEcho));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateEnableEcho(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_ECHO_TIMEOUT_);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SERIAL_PACKET, &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_ECHO_DISABLE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_ECHO_DISABLE) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_ECHO_TIMEOUT_: {
            esEvent *           notify;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_DEVICE, &notify));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Notification, notify));
            }

            return (ES_STATE_TRANSITION(stateEnableEcho));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;
            esError             error;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if ((strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE,   sizeof(RADIO_RESPONSE_ECHO_DISABLE)   - 1)  == 0) ||
                (strncmp(packet->data, RADIO_RESPONSE_ECHO_DISABLE_E, sizeof(RADIO_RESPONSE_ECHO_DISABLE_E) - 1)  == 0)){
                
                return (ES_STATE_TRANSITION(stateNoDev));
            } else {
                esEvent *       notify;

                if (packet->size == 0) {
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_DEVICE, &notify));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Notification, notify));
                    }
                    appTimerStart(&wspace->timer,
                        ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NODEV_PERIOD), EVT_ECHO_POLL_);

                    return (ES_STATE_HANDLED());
                } else {

                    return (ES_STATE_TRANSITION(stateEnableEchoAt));
                }
            }
        }
        case EVT_ECHO_POLL_ : {

            return (ES_STATE_TRANSITION(stateEnableEcho));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateEnableEchoAt(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
         case ES_ENTRY : {
            esEvent *           request;
            esError             error;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_ECHO_AT_TIMEOUT_);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SERIAL_PACKET, &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_CONFIRM;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_CONFIRM) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_ECHO_AT_TIMEOUT_: {
            
            return (ES_STATE_TRANSITION(stateEnableEcho));
        }
        case EVT_SERIAL_PACKET : {

            appTimerCancel(&wspace->timeout);

            return (ES_STATE_TRANSITION(stateEnableEcho));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNoDev(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NODEV_TIMEOUT_);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SERIAL_PACKET, &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_CONFIRM;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_CONFIRM) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NODEV_TIMEOUT_: {
            appTimerStart(&wspace->timer, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NODEV_PERIOD),
                EVT_NODEV_POLL_);

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;
            esError             error;
            esEvent *           notify;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if (strncmp(packet->data, RADIO_RESPONSE_CONFIRM, sizeof(RADIO_RESPONSE_CONFIRM) - 1) == 0) {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_DETECTED, &notify));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Notification, notify));
                }

                return (ES_STATE_TRANSITION(stateNetwLo));
            } else {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_DEVICE, &notify));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Notification, notify));
                }
                appTimerStart(&wspace->timer, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NODEV_PERIOD),
                    EVT_UART_NODEV_TIMEOUT_);
            
                return (ES_STATE_HANDLED());
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNoNetw(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NONETW_TIMEOUT_);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SERIAL_PACKET, &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_GET_STATE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_GET_STATE) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NONETW_TIMEOUT_: {
            appTimerStart(&wspace->timer, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NONETW_PERIOD),
                EVT_UART_NONETW_TIMEOUT_);

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;
            esError             error;
            esEvent *           notify;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if ((strncmp(packet->data, RADIO_RESPONSE_NO_NETW, sizeof(RADIO_RESPONSE_NO_NETW) - 1u)) != 0) {

                  return (ES_STATE_TRANSITION(stateNetwLo));
            } else {
                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NO_NETW, &notify));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(Notification, notify));
                }
                appTimerStart(&wspace->timer,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NONETW_PERIOD),
                    EVT_UART_NONETW_TIMEOUT_);

                return (ES_STATE_HANDLED());
            }
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNetwLo(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NETWLO_TIMEOUT_);
            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SERIAL_PACKET, &request));

            if (!error) {
                ((struct evtSerialPacket *)request)->data = RADIO_CMD_GET_STATE;
                ((struct evtSerialPacket *)request)->size = sizeof(RADIO_CMD_GET_STATE) - 1;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NETWLO_TIMEOUT_: {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NOM_PERIOD),
                EVT_UART_NETWLO_TIMEOUT_);

            return (ES_STATE_TRANSITION(stateNetwLo));
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet;
            esError             error;
            esEvent *           notify;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            if (strncmp(packet->data, RADIO_RESPONSE_NETW, sizeof(RADIO_RESPONSE_NETW) - 1)  == 0) {
                int32_t strength;

                strength = atoi(&((uint8_t *)(packet->data))[sizeof(RADIO_RESPONSE_NETW)]);

                if (strength > netwHighLimit) {

                    return (ES_STATE_TRANSITION(stateNetwHi));
                } else {
                    ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_RADIO_NET_LOW, &notify));

                    if (!error) {
                        ES_ENSURE(esEpaSendEvent(Notification, notify));
                    }
                    appTimerStart(
                        &wspace->timer,
                        ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NOM_PERIOD),
                        EVT_NETWLO_POLL_);

                    return (ES_STATE_HANDLED());
                }
            } else if (strncmp(packet->data, RADIO_RESPONSE_NO_NETW, sizeof(RADIO_RESPONSE_NO_NETW) - 1u) == 0) {
                
                /*
                 * Generate notification event here
                 */
                return (ES_STATE_TRANSITION(stateNoNetw));
            } else {
                /*
                 * Generate notification event here
                 */
                
                return (ES_STATE_TRANSITION(stateNoDev));
            }
        }
        case EVT_NETWLO_POLL_ : {

            return (ES_STATE_TRANSITION(stateNetwLo));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNetwHi(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NETWHI_TIMEOUT_);
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(
                &wspace->uart,
                &wspace->replyBuffer,
                sizeof(wspace->replyBuffer),
                (uint32_t)-1);
            uartWriteStart(
                &wspace->uart,
                RADIO_CMD_GET_STATE,
                sizeof(RADIO_CMD_GET_STATE) - 1u);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NETWHI_TIMEOUT_: {
            /* Cancel UART RX which will generate EVT_BT_UART_RESPONSE or
             * EVT_BT_UART_ERROR
             */
            uartReadCancel(&wspace->uart);
            uartWriteStop(&wspace->uart);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_ERROR_    :
        case EVT_UART_RESPONSE_ : {
            appTimerCancel(&wspace->timeout);
            uartReadStop(&wspace->uart);
            uartWriteStop(&wspace->uart);

            if (strncmp(wspace->replyBuffer, RADIO_RESPONSE_NETW, sizeof(RADIO_RESPONSE_NETW) - 1)  == 0) {
                int32_t strength;

                strength = atoi(&wspace->replyBuffer[sizeof(RADIO_RESPONSE_NETW)]);

                if (strength < netwLowLimit) {
                    /*
                     * Generate notification event here
                     */
                    return (ES_STATE_TRANSITION(stateNetwLo));
                } else {
                    esEvent * event;

                    ES_ENSURE(esEventCreate(sizeof(esEvent), EVT_RADIO_NET_HIGH, &event));
                    ES_ENSURE(esEpaSendEvent(Notification, event));
                    appTimerStart(
                        &wspace->timer,
                        ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NOM_PERIOD),
                        EVT_NETWHI_POLL_);

                    return (ES_STATE_HANDLED());
                }
            } else if (strncmp(wspace->replyBuffer, RADIO_RESPONSE_NO_NETW, sizeof(RADIO_RESPONSE_NO_NETW) - 1u) == 0) {

                /*
                 * Generate notification event here
                 */
                return (ES_STATE_TRANSITION(stateNoNetw));
            } else {
                /*
                 * Generate notification event here
                 */

                return (ES_STATE_TRANSITION(stateNoDev));
            }
        }
        case EVT_NETWHI_POLL_ : {

            return (ES_STATE_TRANSITION(stateNetwHi));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*--  Support functions  -----------------------------------------------------*/

static void initRadio(struct wspace * wspace) {
    /*--  Initialize timeout timer  ------------------------------------------*/
    appTimerInit(&wspace->timeout);

    /*--  Initialize poll/retry timer  ---------------------------------------*/
    appTimerInit(&wspace->timer);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_radio.c
 ******************************************************************************/