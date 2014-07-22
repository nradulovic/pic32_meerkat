/*
 * File:   epa_radio.c
 * Author: nenad
 *
 * Created on April 27, 2014, 1:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "epa_serial.h"
#include "driver/uart.h"
#include "vtimer/vtimer.h"

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
    EVT_UART_TIMEOUT_,
    EVT_UART_ERROR_
};

struct wspace {
    struct uartHandle   uart;
    struct esEpa *      epa;
    char                replyBuffer[2048];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (void *, const esEvent *);
static esAction stateIdle           (void *, const esEvent *);

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

static esAction stateInit(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            wspace->epa = NULL;

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

static esAction stateIdle(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
    switch (event->id) {
        case ES_ENTRY : {
            memset(&wspace->replyBuffer, 0, sizeof(wspace->replyBuffer));
            uartReadStart(&wspace->uart, &wspace->replyBuffer, sizeof(wspace->replyBuffer),
                ES_VTMR_TIME_TO_TICK_MS(10));

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_CLIENT : {
            const struct evtSerialClient * client = (const struct evtSerialClient *)event;

            wspace->epa = client->epa;

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet = (const struct evtSerialPacket *)event;

            if (event->producer == wspace->epa) {
                while (uartWriteStart(&wspace->uart, packet->data, packet->size) == UART_ERROR_BUSY);
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_UART_RX_RESPONSE_ : {

            if (wspace->epa != NULL) {
                esEvent *           packet;
                esError             error;

                ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket) + event->size,
                    EVT_SERIAL_PACKET, &packet));

                if (!error) {
                    struct evtSerialPacket * packet_;

                    packet_       = (struct evtSerialPacket *)packet;
                    packet_->data = &packet_[1];
                    packet_->size = event->size;
                    memcpy(packet_->data, &wspace->replyBuffer, packet_->size);
                    ES_ENSURE(esEpaSendEvent(wspace->epa, packet));
                }
            }
            uartReadStart(&wspace->uart, &wspace->replyBuffer, sizeof(wspace->replyBuffer),
                ES_VTMR_TIME_TO_TICK_MS(10));

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static size_t reader(struct uartHandle * handle, enum uartError uartError, void * data, size_t size) {
    static esEvent             uartEvt;

    (void)data;

    if ((uartError == UART_ERROR_NONE) || (uartError == UART_ERROR_CANCEL)) {
        uartEvt.id = EVT_UART_RX_RESPONSE_;
    } else {
        uartEvt.id = EVT_UART_ERROR_;
    }
    uartEvt.attrib    = ES_EVENT_CONST_Msk;
    uartEvt.mem       = NULL;
    uartEvt.producer  = NULL;
    uartEvt.size      = size;
    uartEvt.timestamp = 0;
#if (CONFIG_API_VALIDATION == 1)
    uartEvt.signature = ES_EVENT_SIGNATURE;
#endif
    ES_ENSURE(esEpaSendAheadEventI(handle->epa, &uartEvt));

    return (0u);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_serial.c
 ******************************************************************************/