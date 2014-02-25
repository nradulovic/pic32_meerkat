/*
 * File:    epa_bt.c
 * Author:  nenad
 * Details: Event Processing Agent for Bluetooth driver
 *
 * Created on February 22, 2014, 7:22 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <string.h>

#include "epa_bt.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "vtimer/vtimer.h"
#include "arch/intr.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BT_DRV_CMD_SIZE(id, cmd, args)  char cmdSize ## id[sizeof(cmd) + args + sizeof("\r\n") - 1u];

#define BT_CMD_REPLY_BUFF_SIZE          100

#define BT_TIMEOUT                      5000

#define BT_R_CMD                        "CMD\r\n"

#define BT_CMD_GPIO_INIT()                                                      \
    do {                                                                        \
        *(CONFIG_BT_CMD_GPIO_PORT)->tris &= ~(0x1u << CONFIG_BT_CMD_GPIO_PIN);  \
    } while (0)

#define BT_CMD_GPIO_LOW()                                                       \
    do {                                                                        \
        *(CONFIG_BT_CMD_GPIO_PORT)->clr = 0x1u << CONFIG_BT_CMD_GPIO_PIN;       \
    } while (0)

#define BT_CMD_GPIO_HIGH()                                                      \
    do {                                                                        \
        *(CONFIG_BT_CMD_GPIO_PORT)->set = 0x1u << CONFIG_BT_CMD_GPIO_PIN;       \
    } while (0)

#define BT_DEF_GPIO_INIT_OUT()                                                  \
    do {                                                                        \
        *(CONFIG_BT_DEF_GPIO_PORT)->tris &= ~(0x1u << CONFIG_BT_DEF_GPIO_PIN);  \
    } while (0)

#define BT_DEF_GPIO_INIT_IN()                                                   \
    do {                                                                        \
        *(CONFIG_BT_DEF_GPIO_PORT)->tris |= (0x1u << CONFIG_BT_DEF_GPIO_PIN);   \
    } while (0)

#define BT_DEF_GPIO_LOW()                                                       \
    do {                                                                        \
        *(CONFIG_BT_DEF_GPIO_PORT)->clr = 0x1u << CONFIG_BT_DEF_GPIO_PIN;       \
    } while (0)

#define BT_DEF_GPIO_HIGH()                                                      \
    do {                                                                        \
        *(CONFIG_BT_DEF_GPIO_PORT)->set = 0x1u << CONFIG_BT_DEF_GPIO_PIN;       \
    } while (0)

#define BT_PWR_GPIO_INIT()                                                      \
    do {                                                                        \
        *(CONFIG_BT_PWR_GPIO_PORT)->tris &= ~(0x1u << CONFIG_BT_PWR_GPIO_PIN);  \
    } while (0)

#define BT_PWR_GPIO_LOW()                                                       \
    do {                                                                        \
        *(CONFIG_BT_PWR_GPIO_PORT)->clr = 0x1u << CONFIG_BT_PWR_GPIO_PIN;       \
    } while (0)

#define BT_PWR_GPIO_HIGH()                                                      \
    do {                                                                        \
        *(CONFIG_BT_PWR_GPIO_PORT)->set = 0x1u << CONFIG_BT_PWR_GPIO_PIN;       \
    } while (0)

#define BT_DRV_TABLE(entry)                                                     \
    entry(stateInit,            TOP)                                            \
    entry(stateIdle,            TOP)                                            \
    entry(stateCmdWaitBegin,    TOP)                                            \
    entry(stateCmdSend,         TOP)                                            \
    entry(stateCmdWaitEnd,      TOP)                                            \
    entry(stateDefToggle0,      TOP)                                            \
    entry(stateDefToggle1,      TOP)                                            \
    entry(stateDefToggle2,      TOP)                                            \
    entry(stateDefToggle3,      TOP)                                            \

/*======================================================  LOCAL DATA TYPES  ==*/

union BtDrvCmdSize {
    BT_DRV_CMD_TABLE(BT_DRV_CMD_SIZE)
};

enum btDrvStateId {
    ES_STATE_ID_INIT(BT_DRV_TABLE)
};

enum localEvent {
    EVT_BT_UART_RESPONSE = ES_EVENT_LOCAL_ID,
    EVT_BT_UART_ERROR,
    EVT_BT_TIMEOUT
};

struct btDrvComm {
    struct esEvent      header;
    size_t              size;
};

struct wspace {
    struct esEpa *      client;
    char                reqBuffer[sizeof(union BtDrvCmdSize)];
    size_t              reqSize;
    char                replyBuffer[BT_CMD_REPLY_BUFF_SIZE];
    struct uartHandle   uart;
    struct esVTimer     timer;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit      (struct wspace *, esEvent *);
static esAction stateIdle      (struct wspace *, esEvent *);
static esAction stateCmdWaitBegin(struct wspace *, esEvent *);
static esAction stateCmdSend   (struct wspace *, esEvent *);
static esAction stateCmdWaitEnd(struct wspace *, esEvent *);
static esAction stateDefToggle0(struct wspace *, esEvent *);
static esAction stateDefToggle1(struct wspace *, esEvent *);
static esAction stateDefToggle2(struct wspace *, esEvent *);
static esAction stateDefToggle3(struct wspace *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static size_t btReplyHandler(
    enum uartError, void *, size_t);

static void btTimeout(void *);

static void initBt(struct wspace *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("epa_bt", CONFIG_BT_DRV_NAME, "Nenad Radulovic");

static const esSmTable BtDrvTable[] = ES_STATE_TABLE_INIT(BT_DRV_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine BtDrvEpaDefine = ES_EPA_DEFINE(
    CONFIG_BT_DRV_NAME,
    CONFIG_BT_DRV_PRIORITY,
    CONFIG_BT_DRV_QUEUE_SIZE);

const struct esSmDefine BtDrvSmDefine = ES_SM_DEFINE(
    BtDrvTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *   BtDrv;

const struct BtDrvCmd BtDrvCmd[] = {
    BT_DRV_CMD_TABLE(BT_DRV_CMD_EXPAND_CMD)
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit (struct wspace * space, esEvent * event) {

    switch (event->id) {
        case ES_INIT : {
            initBt(space);
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(5000u),
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT : {

            return (ES_STATE_TRANSITION(stateIdle));
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
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT : {
            BT_DEF_GPIO_LOW();

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
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT : {
            BT_DEF_GPIO_HIGH();

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
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT : {
            BT_DEF_GPIO_LOW();

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
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT : {
            BT_DEF_GPIO_HIGH();

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle (struct wspace * space, esEvent * event) {

    switch (event->id) {
        case ES_ENTRY : {
            BT_CMD_GPIO_HIGH();
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(1000u),
                btTimeout,
                BtDrv);

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT: {
            esEvent * teste;
            esEventCreate(sizeof(struct BtDrvEvent), EVT_BT_DRV_REQ, &teste);
            ((struct BtDrvEvent *)teste)->string     = BtDrvCmd[BT_DRV_GET_DETAILS].cmd;
            ((struct BtDrvEvent *)teste)->stringSize = BtDrvCmd[BT_DRV_GET_DETAILS].size;
            esEpaSendEvent(BtDrv, teste);
            return (ES_STATE_HANDLED());
        }
        case EVT_BT_DRV_REQ : {
            space->client = event->producer;
            memcpy(
                space->reqBuffer,
                ((struct BtDrvEvent *)event)->string,
                ((struct BtDrvEvent *)event)->stringSize);
            space->reqSize = ((struct BtDrvEvent *)event)->stringSize;

            return (ES_STATE_TRANSITION(stateCmdWaitBegin));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdWaitBegin (struct wspace * space, esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            esVTimerStart(
                &space->timer,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_BT_UART_TIMEOUT_MS),
                btTimeout,
                BtDrv);
            uartReadStart(
                &space->uart,
                space->replyBuffer,
                sizeof(BT_R_CMD) - 1u);
            BT_CMD_GPIO_LOW();

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_TIMEOUT: {
            uartReadStop(&space->uart);
            
            return (ES_STATE_HANDLED());
        }
        case EVT_BT_UART_RESPONSE : {

            if (strcmp(space->replyBuffer, BT_R_CMD) == 0) {

                return (ES_STATE_TRANSITION(stateCmdSend));
            } else {
                esError error;

                ES_ENSURE(error = esEventCreate(
                    sizeof(struct BtDrvStatusEvent),
                    EVT_BT_DRV_STATUS,
                    &event));

                if (error == ES_ERROR_NONE) {
                    ((struct BtDrvStatusEvent *)event)->status = BT_DRV_ERR_FAILURE;
                    esEpaSendEvent(space->client, event);
                }

                return (ES_STATE_TRANSITION(stateIdle));
            }
        }
        case EVT_BT_UART_ERROR : {
            esError     error;

            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtDrvStatusEvent),
                EVT_BT_DRV_STATUS,
                &event));

            if (error == ES_ERROR_NONE) {
                ((struct BtDrvStatusEvent *)event)->status = BT_DRV_ERR_COMM;
                esEpaSendEvent(space->client, event);
            }

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdSend (struct wspace * space, esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            uartReadStart(&space->uart, &space->replyBuffer, sizeof(space->replyBuffer));
            uartWriteStart(&space->uart, space->reqBuffer, space->reqSize);
        }
        case EVT_BT_TIMEOUT: {
            uartReadStop(&space->uart);
            uartWriteStop(&space->uart);
            /*
             * Cancel UART RX which will generate EVT_BT_UART_RESPONSE or EVT_BT_UART_ERROR
             */
            return (ES_STATE_HANDLED());
        }
        case EVT_BT_UART_RESPONSE : {
            esError     error;
            size_t      stringSize;

            stringSize = ((struct btDrvComm *)event)->size;
            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtDrvEvent [1]) + stringSize,
                EVT_BT_DRV_REPLY,
                &event));

            if (error == ES_ERROR_NONE) {
                ((struct BtDrvEvent *)event)->string = (char *)(event + 1u);
                ((struct BtDrvEvent *)event)->stringSize = stringSize;
                memcpy(
                    ((struct BtDrvEvent *)event)->string,
                    space->replyBuffer,
                    stringSize);
                esEpaSendEvent(space->client, event);
            }

            return (ES_STATE_TRANSITION(stateIdle));

        }
        case EVT_BT_UART_ERROR : {
            esError     error;

            ES_ENSURE(error = esEventCreate(
                sizeof(struct BtDrvStatusEvent),
                EVT_BT_DRV_STATUS,
                &event));

            if (error == ES_ERROR_NONE) {
                ((struct BtDrvStatusEvent *)event)->status = BT_DRV_ERR_COMM;
                esEpaSendEvent(space->client, event);
            }

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {
            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdWaitEnd(struct wspace * space , esEvent * event) {
    switch (event->id) {
        default : {
            return (ES_STATE_IGNORED());
        }
    }
}

/*--  Support functions  -----------------------------------------------------*/

static size_t btReplyHandler(
    enum uartError      uartError,
    void *              buffer,
    size_t              size) {

    uint16_t            id;
    esEvent *           event;
    esError             error;

    if (uartError == UART_ERROR_NONE) {
        id = EVT_BT_UART_RESPONSE;
    } else {
        id = EVT_BT_UART_ERROR;
    }
    ES_ENSURE(error = esEventCreate(
        sizeof(struct btDrvComm),
        id,
        &event));

    if (error == ES_ERROR_NONE) {
        ((struct btDrvComm *)event)->size = size;
        esEpaSendEvent(BtDrv, event);
    }

    return (0u);
}

static void btTimeout(void *   arg) {
    struct esEpa *      epa;
    struct esEvent *    event;
    esError             error;

    epa = (struct esEpa *)arg;
    ES_ENSURE(error = esEventCreate(
        sizeof(struct esEvent),
        EVT_BT_TIMEOUT,
        &event));

    if (error == ES_ERROR_NONE) {
        esEpaSendEvent(epa, event);
    }
}

static void initBt(struct wspace * space) {

    struct uartConfig btUartConfig;

    /*--  Initialize CMD & DEF pins  -----------------------------------------*/
    BT_CMD_GPIO_INIT();
    BT_DEF_GPIO_INIT_IN();
    BT_PWR_GPIO_INIT();
    BT_CMD_GPIO_LOW();
    BT_DEF_GPIO_HIGH();
    BT_PWR_GPIO_HIGH();

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
    uartSetReader(&space->uart, btReplyHandler);

    /*--  Initialize timeout timer  ------------------------------------------*/
    esVTimerInit(&space->timer);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
