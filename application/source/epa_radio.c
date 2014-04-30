/*
 * File:   epa_radio.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "events.h"
#include "arch/intr.h"
#include "driver/uart.h"
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

#define RADIO_CMD_ECHO_ENABLE           "ATE1\r\n"
#define RADIO_RESPONSE_ECHO_ENABLE      RADIO_CMD_ECHO_ENABLE "\r\nOK\r\n"

#define RADIO_CMD_CONFIRM               "AT\r\n"
#define RADIO_RESPONSE_CONFIRM          RADIO_CMD_CONFIRM "\r\nOK\r\n"

#define RADIO_CMD_GET_STATE             "AT+CSQ?\r\n"
#define RADIO_RESPONSE_NETW             RADIO_CMD_GET_STATE "\r\n+CMS:"
#define RADIO_RESPONSE_NO_NETW          RADIO_CMD_GET_STATE "\r\n+CME ERROR: 30\r\n"

#define EVENT_NOTIFY_NO_NETW            (0x1u << 0)
#define EVENT_NOTIFY_NETW_GOOD          (0x1u << 1)
#define EVENT_NOTIFY_NETW_BAD           (0x1u << 2)
#define EVENT_NOTIFY_NO_DEVICE          (0x1u << 3)

#define RADIO_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \
    entry(stateEnableEcho,          TOP)                                        \
    entry(stateNoNetw,              TOP)                                        \
    entry(stateNetwLo,              TOP)                                        \
    entry(stateNetwHi,              TOP)                                        \
    entry(stateNoDev,               TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum radioStateId {
    ES_STATE_ID_INIT(RADIO_TABLE)
};

enum localEvents {
    EVT_UART_RESPONSE_ = ES_EVENT_LOCAL_ID,
    EVT_UART_ERROR_,
    EVT_ECHO_POLL_,
    EVT_NODEV_POLL_,
    EVT_NONETW_POLL_,
    EVT_NETWLO_POLL_,
    EVT_NETWHI_POLL_,
    EVT_IDLE_TIMEOUT_,
    EVT_UART_ECHO_TIMEOUT_,
    EVT_UART_NETWLO_TIMEOUT_,
    EVT_UART_NETWHI_TIMEOUT_,
    EVT_UART_NODEV_TIMEOUT_,
    EVT_UART_NONETW_TIMEOUT_
};

struct uartEvent_ {
    struct esEvent      header;
    size_t              size;
};

struct wspace {
    struct uartHandle   uart;
    struct appTimer     timeout;
    struct appTimer     timer;
    uint32_t            timerTicks;
    uint32_t            eventNotify;
    char                replyBuffer[40];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, const esEvent *);
static esAction stateEnableEcho     (struct wspace *, const esEvent *);
static esAction stateNoNetw         (struct wspace *, const esEvent *);
static esAction stateNetwLo         (struct wspace *, const esEvent *);
static esAction stateNetwHi         (struct wspace *, const esEvent *);
static esAction stateNoDev          (struct wspace *, const esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static size_t radioUartReadHandler(
    enum uartError      uartError,
    void *              buffer,
    size_t              size);
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
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_ECHO_TIMEOUT_);
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(
                &wspace->uart,
                &wspace->replyBuffer,
                sizeof(wspace->replyBuffer));
            uartWriteStart(
                &wspace->uart,
                RADIO_CMD_ECHO_ENABLE,
                sizeof(RADIO_CMD_ECHO_ENABLE) - 1u);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_ECHO_TIMEOUT_: {
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

            if (strncmp(wspace->replyBuffer, RADIO_RESPONSE_ECHO_ENABLE, sizeof(RADIO_RESPONSE_ECHO_ENABLE) - 1)  == 0) {
                
                return (ES_STATE_TRANSITION(stateNoDev));
            } else {
                appTimerStart(
                    &wspace->timer,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NODEV_PERIOD),
                    EVT_ECHO_POLL_);

                return (ES_STATE_HANDLED());
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

static esAction stateNoDev(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NODEV_TIMEOUT_);
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(
                &wspace->uart,
                &wspace->replyBuffer,
                sizeof(wspace->replyBuffer));
            uartWriteStart(
                &wspace->uart,
                RADIO_CMD_CONFIRM,
                sizeof(RADIO_CMD_CONFIRM) - 1u);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NODEV_TIMEOUT_: {
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

            if (strncmp(wspace->replyBuffer, RADIO_CMD_CONFIRM, sizeof(RADIO_CMD_CONFIRM) - 1)  == 0) {

                /*
                 * Generate notification event here: DEVICE_DETECTED
                 */
                return (ES_STATE_TRANSITION(stateNetwLo));
            } else {
                appTimerStart(
                    &wspace->timer,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NODEV_PERIOD),
                    EVT_NODEV_POLL_);
            
                return (ES_STATE_HANDLED());
            }
        }
        case EVT_NODEV_POLL_ : {

            return (ES_STATE_TRANSITION(stateNoDev));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNoNetw(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NONETW_TIMEOUT_);
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(
                &wspace->uart,
                &wspace->replyBuffer,
                sizeof(wspace->replyBuffer));
            uartWriteStart(
                &wspace->uart,
                RADIO_CMD_GET_STATE,
                sizeof(RADIO_CMD_GET_STATE) - 1u);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NONETW_TIMEOUT_: {
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

            if ((strncmp(wspace->replyBuffer, RADIO_RESPONSE_NO_NETW, sizeof(RADIO_RESPONSE_NO_NETW) - 1u)) != 0) {

                  return (ES_STATE_TRANSITION(stateNetwLo));
            } else {
                appTimerStart(
                    &wspace->timer,
                    ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NONETW_PERIOD),
                    EVT_NONETW_POLL_);

                return (ES_STATE_HANDLED());
            }
        }
        case EVT_NONETW_POLL_ : {

            return (ES_STATE_TRANSITION(stateNoNetw));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateNetwLo(struct wspace * wspace, const esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_UART_TIMEOUT),
                EVT_UART_NETWLO_TIMEOUT_);
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(
                &wspace->uart,
                &wspace->replyBuffer,
                sizeof(wspace->replyBuffer));
            uartWriteStart(
                &wspace->uart,
                RADIO_CMD_GET_STATE,
                sizeof(RADIO_CMD_GET_STATE) - 1u);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_NETWLO_TIMEOUT_: {
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

                if (strength > netwHighLimit) {
                    /*
                     * Generate notification event here
                     */
                    return (ES_STATE_TRANSITION(stateNetwHi));
                } else {
                    appTimerStart(
                        &wspace->timer,
                        ES_VTMR_TIME_TO_TICK_MS(CONFIG_RADIO_POLL_NOM_PERIOD),
                        EVT_NETWLO_POLL_);

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
                sizeof(wspace->replyBuffer));
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

static size_t radioUartReadHandler(
    enum uartError      uartError,
    void *              buffer,
    size_t              size) {

    uint16_t            id;
    esEvent *           reply;
    esError             error;

    (void)size;
    (void)buffer;

    if ((uartError == UART_ERROR_NONE) || (uartError == UART_ERROR_CANCEL)) {
        id = EVT_UART_RESPONSE_;
    } else {
        id = EVT_UART_ERROR_;
    }
    ES_ENSURE(error = esEventCreate(sizeof(esEvent), id, &reply));

    if (error == ES_ERROR_NONE) {
        esEpaSendAheadEvent(Radio, reply);
    }

    return (0u);
}

static void initRadio(struct wspace * wspace) {
    struct uartConfig radioUartConfig;

    /*--  Initialize UART  ---------------------------------------------------*/
    radioUartConfig.id          = CONFIG_RADIO_UART;
    radioUartConfig.flags       = UART_TX_ENABLE   | UART_RX_ENABLE   |
                                  UART_DATA_BITS_8 | UART_STOP_BITS_1 |
                                  UART_PARITY_NONE;
    radioUartConfig.speed       = 38400;                                        /* This is default baudrate for the radio                   */
    radioUartConfig.isrPriority = ES_INTR_DEFAULT_ISR_PRIO;
    radioUartConfig.remap.tx    = CONFIG_RADIO_UART_TX_PIN;
    radioUartConfig.remap.rx    = CONFIG_RADIO_UART_RX_PIN;
    radioUartConfig.remap.cts   = CONFIG_RADIO_UART_CTS_PIN;
    radioUartConfig.remap.rts   = CONFIG_RADIO_UART_RTS_PIN;
    uartOpen(&wspace->uart, &radioUartConfig);
    uartSetReader(&wspace->uart, radioUartReadHandler);

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