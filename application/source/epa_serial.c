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
    struct esEpa *      client;
    char                rx_buffer[4096];
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
            wspace->client = NULL;

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_OPEN : {
            uartOpen(&wspace->uart, &((struct evtSerialOpen *)event)->config);
            uartSetClient(&wspace->uart, esEdsGetCurrent());
            uartSetReader(&wspace->uart, reader);
            wspace->client = event->producer;

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
            while (uartReadStart(&wspace->uart, &wspace->rx_buffer,
                sizeof(wspace->rx_buffer), ES_VTMR_TIME_TO_TICK_MS(10)) == UART_ERROR_BUSY) ;

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {
            const struct evtSerialPacket * packet = (const struct evtSerialPacket *)event;

            while (uartWriteStart(&wspace->uart, packet->data, packet->size) == UART_ERROR_BUSY);

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static size_t reader(struct uartHandle * handle, enum uartError uartError, void * data, size_t size)
{
    struct wspace *             serial;

    serial = esEpaGetWorkspace(handle->epa);

    if (!((uartError == UART_ERROR_NONE) || (uartError == UART_ERROR_CANCEL))) {
        /* Force UART to continue working even after an error has been detected */
        uartReadStart(&serial->uart, &serial->rx_buffer,
                sizeof(serial->rx_buffer), ES_VTMR_TIME_TO_TICK_MS(10));
        
        return (0u); /* Ignored in this case */
    }
    
    if (serial->client != NULL) {
        esEvent *               packet;
        esError                 error;

        ES_ENSURE(error = esEventCreateI(sizeof(struct evtSerialPacket) + size,
            EVT_SERIAL_PACKET, &packet));

        if (!error) {
            struct evtSerialPacket * packet_;

            packet_                 = (struct evtSerialPacket *)packet;
            packet_->event.producer = handle->epa;
            packet_->data           = &packet_[1];
            packet_->size           = size;
            memcpy(packet_->data, data, size);
            ES_ENSURE(esEpaSendAheadEventI(serial->client, packet));
        }
    }
    
    return (sizeof(serial->rx_buffer));
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_serial.c
 ******************************************************************************/