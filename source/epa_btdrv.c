/*
 * File:    epa_btdrv.c
 * Author:  nenad
 * Details: Event Processing Agent for Bluetooth driver
 *
 * Created on February 22, 2014, 7:22 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "events.h"
#include "arch/intr.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "vtimer/vtimer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BT_DRV_CMD_EXPAND_SIZE(id, cmd, args)                                   \
    char cmdSize ## id[sizeof(cmd) + args + sizeof("\r\n") - 1u];

#define BT_DRV_CMD_EXPAND_CMD(id, cmd, args)                                    \
    {cmd, sizeof(cmd) - 1u},

#define BT_CMD_REPLY_BUFF_SIZE          100

#define BT_CMD_BEGIN                    "CMD\r\n"
#define BT_CMD_END                      "END\r\n"
#define BT_CMD_VALID                    "AOK\r\n"
#define BT_CMD_REBOOT                   "Reboot!\r\n"

#define BT_CMD_INIT_OUT()                                                       \
    do {                                                                        \
        *(CONFIG_BT_GPIO_CMD_PORT)->tris &= ~(0x1u << CONFIG_BT_GPIO_CMD_PIN);  \
    } while (0)

#define BT_CMD_LOW()                                                            \
    do {                                                                        \
        *(CONFIG_BT_GPIO_CMD_PORT)->clr = 0x1u << CONFIG_BT_GPIO_CMD_PIN;       \
    } while (0)

#define BT_CMD_HIGH()                                                           \
    do {                                                                        \
        *(CONFIG_BT_GPIO_CMD_PORT)->set = 0x1u << CONFIG_BT_GPIO_CMD_PIN;       \
    } while (0)

#define BT_DEF_INIT_OUT()                                                       \
    do {                                                                        \
        *(CONFIG_BT_GPIO_DEF_PORT)->tris &= ~(0x1u << CONFIG_BT_GPIO_DEF_PIN);  \
    } while (0)

#define BT_DEF_INIT_IN()                                                        \
    do {                                                                        \
        *(CONFIG_BT_GPIO_DEF_PORT)->tris |= (0x1u << CONFIG_BT_GPIO_DEF_PIN);   \
    } while (0)

#define BT_DEF_LOW()                                                            \
    do {                                                                        \
        *(CONFIG_BT_GPIO_DEF_PORT)->clr = 0x1u << CONFIG_BT_GPIO_DEF_PIN;       \
    } while (0)

#define BT_DEF_HIGH()                                                           \
    do {                                                                        \
        *(CONFIG_BT_GPIO_DEF_PORT)->set = 0x1u << CONFIG_BT_GPIO_DEF_PIN;       \
    } while (0)

#define BT_PWR_INIT_OUT()                                                       \
    do {                                                                        \
        *(CONFIG_BT_GPIO_PWR_PORT)->tris &= ~(0x1u << CONFIG_BT_GPIO_PWR_PIN);  \
    } while (0)

#define BT_PWR_LOW()                                                            \
    do {                                                                        \
        *(CONFIG_BT_GPIO_PWR_PORT)->clr = 0x1u << CONFIG_BT_GPIO_PWR_PIN;       \
    } while (0)

#define BT_PWR_HIGH()                                                           \
    do {                                                                        \
        *(CONFIG_BT_GPIO_PWR_PORT)->set = 0x1u << CONFIG_BT_GPIO_PWR_PIN;       \
    } while (0)

#define BT_DRV_TABLE(entry)                                                     \
    entry(stateInit,            TOP)                                            \
    entry(stateIdle,            TOP)                                            \
    entry(stateCmdBegin,    TOP)                                            \
    entry(stateCmdIdle,         TOP)                                            \
    entry(stateCmdSend,         TOP)                                            \
    entry(stateCmdEnd,      TOP)                                            \
    entry(stateDefToggle0,      TOP)                                            \
    entry(stateDefToggle1,      TOP)                                            \
    entry(stateDefToggle2,      TOP)                                            \
    entry(stateDefToggle3,      TOP)                                            \

/*======================================================  LOCAL DATA TYPES  ==*/

struct BtCmd {
    char *              cmd;
    size_t              size;
};

union BtDrvCmdSize {
    BT_DRV_CMD_TABLE(BT_DRV_CMD_EXPAND_SIZE)
};

enum btDrvStateId {
    ES_STATE_ID_INIT(BT_DRV_TABLE)
};

enum localEvents {
    EVT_UART_RESPONSE_ = ES_EVENT_LOCAL_ID,
    EVT_UART_ERROR_,
    EVT_TIMEOUT_
};

struct uartEvent_ {
    struct esEvent      header;
    size_t              size;
};

struct wspace {
    struct esEpa *      client;
    struct uartHandle   uart;
    struct esVTimer     timer;
    size_t              reqSize;
    char                reqBuffer[sizeof(union BtDrvCmdSize)];
    char                replyBuffer[BT_CMD_REPLY_BUFF_SIZE];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit      (struct wspace *, esEvent *);
static esAction stateIdle      (struct wspace *, esEvent *);
static esAction stateCmdBegin(struct wspace *, esEvent *);
static esAction stateCmdIdle   (struct wspace *, esEvent *);
static esAction stateCmdSend   (struct wspace *, esEvent *);
static esAction stateCmdEnd(struct wspace *, esEvent *);
static esAction stateDefToggle0(struct wspace *, esEvent *);
static esAction stateDefToggle1(struct wspace *, esEvent *);
static esAction stateDefToggle2(struct wspace *, esEvent *);
static esAction stateDefToggle3(struct wspace *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static size_t btUartReadHandler(
    enum uartError, void *, size_t);

static void btTimeoutHandler(void *);

static void initBtDrv(struct wspace *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("epa_bt", CONFIG_BT_DRV_NAME, "Nenad Radulovic");

static const esSmTable BtDrvTable[] = ES_STATE_TABLE_INIT(BT_DRV_TABLE);

static const struct BtCmd BtCmdTable[] = {
    BT_DRV_CMD_TABLE(BT_DRV_CMD_EXPAND_CMD)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine BtDrvEpa = ES_EPA_DEFINE(
    CONFIG_BT_DRV_NAME,
    CONFIG_BT_DRV_PRIORITY,
    CONFIG_BT_DRV_QUEUE_SIZE);

const struct esSmDefine BtDrvSm = ES_SM_DEFINE(
    BtDrvTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *   BtDrv;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit (struct wspace * space, esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            initBtDrv(space);
            BT_PWR_LOW();
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            BT_PWR_HIGH();

            return (ES_STATE_TRANSITION(stateDefToggle0));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle0 (struct wspace * space, esEvent * event) {
    switch(event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            BT_DEF_INIT_OUT();
            BT_DEF_LOW();

            return (ES_STATE_TRANSITION(stateDefToggle1));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle1 (struct wspace * space, esEvent * event) {
    switch(event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            BT_DEF_HIGH();

            return (ES_STATE_TRANSITION(stateDefToggle2));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle2 (struct wspace * space, esEvent * event) {
    switch(event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            BT_DEF_LOW();

            return (ES_STATE_TRANSITION(stateDefToggle3));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle3 (struct wspace * space, esEvent * event) {
    switch(event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            /*
             * TODO: Should broadcast here it is ready
             */
            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_BT_NOTIFY_READY,
                &event));
            esEpaSendEvent(BtMan, event);
            BT_DEF_HIGH();
            BT_DEF_INIT_IN();

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle (struct wspace * space, esEvent * event) {

    (void)space;

    switch (event->id) {
        case EVT_BT_CMD_MODE_ENTER : {
            space->client  = event->producer;

            return (ES_STATE_TRANSITION(stateCmdBegin));
        }
        case EVT_BT_RESTART : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeoutHandler,
                BtDrv);
            BT_PWR_LOW();

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_: {
            BT_PWR_HIGH();

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_SEND_DATA : {
            struct BtSendEvent * data;

            data = (struct BtSendEvent *)event;
            uartWriteStop(&space->uart);
            uartWriteStart(&space->uart, data->arg, data->argSize);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdBegin (struct wspace * space, esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_BT_UART_TIMEOUT_MS),
                btTimeoutHandler,
                BtDrv);
            uartReadStart(
                &space->uart,
                space->replyBuffer,
                sizeof(BT_CMD_BEGIN) - 1u);
            BT_CMD_LOW();

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_: {
            uartReadCancel(&space->uart);
            
            return (ES_STATE_HANDLED());
        }
        case EVT_UART_RESPONSE_ : {
            esVTimerCancel(&space->timer);
            uartReadStop(&space->uart);
            
            if (strcmp(space->replyBuffer, BT_CMD_BEGIN) == 0) {
                ES_ENSURE(esEventCreate(
                    sizeof(esEvent),
                    EVT_BT_NOTIFY_READY,
                    &event));
                esEpaSendEvent(space->client, event);

                return (ES_STATE_TRANSITION(stateCmdIdle));
            } else {
                esError error;
                struct BtReplyEvent * reply;

                ES_ENSURE(error = esEventCreate(
                    sizeof(struct BtReplyEvent),
                    EVT_BT_REPLY,
                    (esEvent **)&reply));

                if (error == ES_ERROR_NONE) {
                    reply->status  = BT_ERR_FAILURE;
                    reply->arg     = NULL;
                    reply->argSize = 0;
                    esEpaSendEvent(space->client, (esEvent *)reply);
                }

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        case EVT_UART_ERROR_ : {
            esError     error;
            struct BtReplyEvent * reply;
            
            esVTimerCancel(&space->timer);
            uartReadStop(&space->uart);
            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtReplyEvent),
                EVT_BT_REPLY,
                (esEvent **)&reply));

            if (error == ES_ERROR_NONE) {
                reply->status  = BT_ERR_COMM;
                reply->arg     = NULL;
                reply->argSize = 0;
                esEpaSendEvent(space->client, (esEvent *)reply);
            }

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdIdle (struct wspace * space, esEvent * event) {
    switch (event->id) {
        case EVT_BT_CMD_MODE_EXIT : {

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        case EVT_BT_REQ : {
            struct BtReqEvent * request;

            
            space->reqSize = 0u;
            request = (struct BtReqEvent *)event;
            memcpy(
                &space->reqBuffer[space->reqSize],
                BtCmdTable[request->cmd].cmd,
                BtCmdTable[request->cmd].size);
            space->reqSize += BtCmdTable[request->cmd].size;

            if (request->arg != NULL) {
                memcpy(
                    &space->reqBuffer[space->reqSize],
                    request->arg,
                    request->argSize);
                space->reqSize += request->argSize;
            }
            memcpy(
                &space->reqBuffer[space->reqSize],
                "\r\n",
                2u);
            space->reqSize += 2u;

            return (ES_STATE_TRANSITION(stateCmdSend));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdSend (struct wspace * space, esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_BT_UART_TIMEOUT_MS),
                btTimeoutHandler,
                BtDrv);
            uartReadStart(
                &space->uart,
                &space->replyBuffer,
                sizeof(space->replyBuffer));
            uartWriteStart(&space->uart, space->reqBuffer, space->reqSize);
            
            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_: {
            /* Cancel UART RX which will generate EVT_BT_UART_RESPONSE or
             * EVT_BT_UART_ERROR
             */
            uartReadCancel(&space->uart);
            uartWriteStop(&space->uart);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_RESPONSE_ : {
            esError     error;
            struct BtReplyEvent * reply;
            
            esVTimerCancel(&space->timer);
            uartReadStop(&space->uart);
            uartWriteStop(&space->uart);
            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtReplyEvent [1]) + ((struct uartEvent_ *)event)->size,
                EVT_BT_REPLY,
                (esEvent **)&reply));

            if (error == ES_ERROR_NONE) {

                if ((strcmp(space->replyBuffer, BT_CMD_VALID)  == 0) ||
                    (strcmp(space->replyBuffer, BT_CMD_REBOOT) == 0)) {
                    reply->status = BT_ERR_NONE;
                } else {
                    reply->status = BT_ERR_FAILURE;
                }
                reply->arg     = (char *)(reply + 1u);
                reply->argSize = ((struct uartEvent_ *)event)->size;
                memcpy(reply->arg, space->replyBuffer, reply->argSize);
                esEpaSendEvent(space->client, (esEvent *)reply);
            }

            return (ES_STATE_TRANSITION(stateCmdIdle));
        }
        case EVT_UART_ERROR_ : {
            esError     error;
            struct BtReplyEvent * reply;

            esVTimerCancel(&space->timer);
            uartReadStop(&space->uart);
            uartWriteStop(&space->uart);
            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtReplyEvent),
                EVT_BT_REPLY,
                (esEvent **)&reply));

            if (error == ES_ERROR_NONE) {
                reply->status  = BT_ERR_COMM;
                reply->arg     = NULL;
                reply->argSize = 0;
                esEpaSendEvent(space->client, (esEvent *)reply);
            }

            return (ES_STATE_TRANSITION(stateCmdIdle));
        }
        default : {
            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdEnd(struct wspace * space , esEvent * event) {

    (void)space;
    
    switch (event->id) {
        case ES_INIT : {
            BT_CMD_HIGH();

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {
            return (ES_STATE_IGNORED());
        }
    }
}

/*--  Support functions  -----------------------------------------------------*/

static size_t btUartReadHandler(
    enum uartError      uartError,
    void *              buffer,
    size_t              size) {

    uint16_t            id;
    struct uartEvent_ * reply;
    esError             error;

    (void)buffer;

    if ((uartError == UART_ERROR_NONE) || (uartError == UART_ERROR_CANCEL)) {
        id = EVT_UART_RESPONSE_;
    } else {
        id = EVT_UART_ERROR_;
    }
    ES_ENSURE(error = esEventCreate(
        sizeof(struct uartEvent_),
        id,
        (esEvent **)&reply));

    if (error == ES_ERROR_NONE) {
        reply->size = size;
        esEpaSendAheadEvent(BtDrv, (esEvent *)reply);
    }

    return (0u);
}

static void btTimeoutHandler(void *   arg) {
    struct esEpa *      epa;
    struct esEvent *    timeout;
    esError             error;

    epa = (struct esEpa *)arg;
    ES_ENSURE(error = esEventCreate(
        sizeof(struct esEvent),
        EVT_TIMEOUT_,
        &timeout));

    if (error == ES_ERROR_NONE) {
        esEpaSendAheadEvent(epa, timeout);
    }
}

static void initBtDrv(struct wspace * space) {

    struct uartConfig btUartConfig;

    /*--  Initialize CMD & DEF & PWR pins  -----------------------------------*/
    BT_CMD_INIT_OUT();
    BT_DEF_INIT_IN();
    BT_PWR_INIT_OUT();
    BT_CMD_HIGH();
    BT_DEF_HIGH();
    BT_PWR_HIGH();

    /*--  Initialize UART  ---------------------------------------------------*/
    btUartConfig.id          = &Uart1;
    btUartConfig.flags       = UART_TX_ENABLE   | UART_RX_ENABLE   |
                               UART_DATA_BITS_8 | UART_STOP_BITS_1 |
                               UART_PARITY_NONE;
    btUartConfig.speed       = CONFIG_BT_UART_SPEED;
    btUartConfig.isrPriority = ES_INTR_DEFAULT_ISR_PRIO;
    btUartConfig.remap.tx    = CONFIG_BT_UART_TX_PIN;
    btUartConfig.remap.rx    = CONFIG_BT_UART_RX_PIN;
    btUartConfig.remap.cts   = CONFIG_BT_UART_CTS_PIN;
    btUartConfig.remap.rts   = CONFIG_BT_UART_RTS_PIN;
    uartOpen(&space->uart, &btUartConfig);
    uartSetReader(&space->uart, btUartReadHandler);

    /*--  Initialize timeout timer  ------------------------------------------*/
    esVTimerInit(&space->timer);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
