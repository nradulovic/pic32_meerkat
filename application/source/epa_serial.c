/*
 * File:   epa_radio.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "epa_serial.h"
#include "driver/uart.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SERIAL_TABLE(entry)                                                     \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum serialStateId {
    ES_STATE_ID_INIT(SERIAL_TABLE)
};

enum localEvents {
    EVT_UART_RX_RESPONSE_ = ES_EVENT_LOCAL_ID,
    EVT_UART_ERROR_
};

struct uartEvent_ {
    struct esEvent      header;
    size_t              size;
};

struct wspace {
    struct uartHandle   uart;
    struct esEpa *      client;
    char                replyBuffer[256];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, const esEvent *);
static esAction stateIdle           (struct wspace *, const esEvent *);

static size_t reader(struct uartHandle *, enum uartError, void *, size_t);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Serial", "Serial management", "Nenad Radulovic");

static const esSmTable SerialTable[] = ES_STATE_TABLE_INIT(SERIAL_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine SerialEpa = ES_EPA_DEFINE(
    "Serial",
    CONFIG_SERIAL_PRIORITY,
    CONFIG_SERIAL_QUEUE_SIZE);

const struct esSmDefine SerialSm = ES_SM_DEFINE(
    SerialTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *          SerialBt;
struct esEpa *          SerialRadio;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_INIT : {
            wspace->client = NULL;

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_OPEN : {
            uartOpen(&wspace->uart, &((struct evtSerialOpen *)event)->config);
            uartSetClient(&wspace->uart, esEdsGetCurrent());
            uartSetReader(&wspace->uart, reader);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * wspace, const esEvent * event) {
    switch (event->id) {
        case ES_ENTRY : {
            uartReadStart(&wspace->uart, &wspace->replyBuffer, sizeof(wspace->replyBuffer),
                ES_VTMR_TIME_TO_TICK_MS(20));

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_CLIENT : {
            wspace->client = ((const struct evtSerialClient *)event)->client;

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {

            while (uartWriteStart(&wspace->uart, ((const struct evtSerialPacket *)event)->data,
                ((const struct evtSerialPacket *)event)->size) == UART_ERROR_BUSY);

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_RX_RESPONSE_ : {

            if (wspace->client != NULL) {
                struct esEvent *    packet;
                esError             error;

                error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET, &packet);

                if (!error) {
                    ((struct evtSerialPacket *)packet)->data = &wspace->replyBuffer;
                    ((struct evtSerialPacket *)packet)->size =
                        ((const struct uartEvent_ *)event)->size;
                     esEpaSendEvent(wspace->client, packet);
                }
            }
            uartReadStart(&wspace->uart, &wspace->replyBuffer, sizeof(wspace->replyBuffer),
                ES_VTMR_TIME_TO_TICK_MS(20));

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static size_t reader(struct uartHandle * handle, enum uartError uartError, void * data, size_t size) {
    esEvent *           uartEvt;
    uint16_t            id;
    esError             error;

    (void)data;
    
    if ((uartError == UART_ERROR_NONE) || (uartError == UART_ERROR_TIMEOUT)) {
        id = EVT_UART_RX_RESPONSE_;
    } else {
        id = EVT_UART_ERROR_;
    }
    ES_ENSURE(error = esEventCreateI(sizeof(struct uartEvent_), id, &uartEvt));

    if (!error) {
        ((struct uartEvent_ *)uartEvt)->size = size;
        ES_ENSURE(esEpaSendAheadEventI(handle->epa, uartEvt));
    }

    return (0u);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_serial.c
 ******************************************************************************/