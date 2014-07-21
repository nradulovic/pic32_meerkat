
#include "arch/intr.h"
#include "epa_sync.h"
#include "app_timer.h"
#include "driver/uart.h"
#include "epa_serial.h"
#include "config/config_project.h"
#include "config/config_pins.h"
#include "epa_btdrv.h"
#include "epa_radio.h"
#include "epa_btman.h"

#define CONFIG_POLL_PERIOD                  4000

#define SYNC_TABLE(entry)                                                                           \
    entry(stateInit,            TOP)                                                                \
    entry(stateIdle,            TOP)                                                                \

enum syncStateId {
    ES_STATE_ID_INIT(SYNC_TABLE)
};

enum evtLocalId {
    EVT_LOCAL_POLL = ES_EVENT_USER_ID
};

struct wspace {
    struct appTimer     timeout;
    bool                isBtReady;
    bool                isRadioReady;
};

static esAction stateInit           (void *, const esEvent *);
static esAction stateIdle           (void *, const esEvent *);

static const ES_MODULE_INFO_CREATE("Sync", "Sync manager", "Nenad Radulovic");

static const esSmTable SyncTable[] = ES_STATE_TABLE_INIT(SYNC_TABLE);

const struct esEpaDefine SyncEpa = ES_EPA_DEFINE(
    "Sync",
    CONFIG_SYNC_PRIORITY,
    CONFIG_SYNC_QUEUE_SIZE);

const struct esSmDefine SyncSm = ES_SM_DEFINE(
    SyncTable,
    sizeof(struct wspace),
    stateInit);

struct esEpa *          Sync;


static esAction stateInit(void * space, const esEvent * event) {
    struct wspace *     wspace = space;
    
    switch (event->id) {
        case ES_INIT : {
            esError             error;
            esEvent *           request;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialOpen), EVT_SERIAL_OPEN, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialOpen *)request)->config.id          = CONFIG_BT_UART;
            ((struct evtSerialOpen *)request)->config.flags       = UART_TX_ENABLE   |
                                                                    UART_RX_ENABLE   |
                                                                    UART_DATA_BITS_8 |
                                                                    UART_STOP_BITS_1 |
                                                                    UART_PARITY_NONE;
            ((struct evtSerialOpen *)request)->config.speed       = CONFIG_BT_UART_SPEED;
            ((struct evtSerialOpen *)request)->config.isrPriority = ES_INTR_DEFAULT_ISR_PRIO;
            ((struct evtSerialOpen *)request)->config.remap.tx    = CONFIG_BT_UART_TX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rx    = CONFIG_BT_UART_RX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.cts   = CONFIG_BT_UART_CTS_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rts   = CONFIG_BT_UART_RTS_PIN;
            ES_ENSURE(esEpaSendEvent(SerialBt, request));
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT,
                &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialClient *)request)->epa = BtDrv;
            ES_ENSURE(esEpaSendEvent(SerialBt, request));
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialOpen), EVT_SERIAL_OPEN, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialOpen *)request)->config.id          = CONFIG_RADIO_UART;
            ((struct evtSerialOpen *)request)->config.flags       = UART_TX_ENABLE   |
                                                                    UART_RX_ENABLE   |
                                                                    UART_DATA_BITS_8 |
                                                                    UART_STOP_BITS_1 |
                                                                    UART_PARITY_NONE;
            ((struct evtSerialOpen *)request)->config.speed       = 38400;
            ((struct evtSerialOpen *)request)->config.isrPriority = ES_INTR_DEFAULT_ISR_PRIO;
            ((struct evtSerialOpen *)request)->config.remap.tx    = CONFIG_RADIO_UART_TX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rx    = CONFIG_RADIO_UART_RX_PIN;
            ((struct evtSerialOpen *)request)->config.remap.cts   = CONFIG_RADIO_UART_CTS_PIN;
            ((struct evtSerialOpen *)request)->config.remap.rts   = CONFIG_RADIO_UART_RTS_PIN;
            ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT,
                &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialClient *)request)->epa = Radio;
            ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            appTimerInit(&wspace->timeout);
            wspace->isBtReady    = false;
            wspace->isRadioReady = false;

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_READY : {
            if (event->producer == BtMan) {
                wspace->isBtReady = true;
            } else if (event->producer == Radio) {
                wspace->isRadioReady = true;
            }

            if (wspace->isBtReady && wspace->isRadioReady) {
                return (ES_STATE_TRANSITION(stateIdle));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(void * space, const esEvent * event) {
    struct wspace *     wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            wspace->isBtReady    = false;
            wspace->isRadioReady = false;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_POLL_PERIOD), EVT_LOCAL_POLL);

            return (ES_STATE_HANDLED());
        }
        case ES_EXIT : {
            appTimerCancel(&wspace->timeout);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_POLL : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialClient *)request)->epa = Radio;
            ES_ENSURE(esEpaSendEvent(SerialRadio, request));
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ((struct evtSerialClient *)request)->epa = BtDrv;
            ES_ENSURE(esEpaSendEvent(SerialBt, request));

            ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_SYNC_TICK, &request));

            if (!error) {
                ES_ENSURE(esEpaSendEvent(Radio, request));
                ES_ENSURE(esEpaSendEvent(BtMan, request));
            }
            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_DONE : {
            esEvent *           request;
            esError             error;

            if (event->producer == BtMan) {
                wspace->isBtReady = true;
            } else if (event->producer == Radio) {
                wspace->isRadioReady = true;
            }

            if (wspace->isBtReady && wspace->isRadioReady) {
                ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT, &request));

                if (error) {
                    return (ES_STATE_TRANSITION(stateIdle));
                }
                ((struct evtSerialClient *)request)->epa = SerialBt;
                ES_ENSURE(esEpaSendEvent(SerialRadio, request));
                ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialClient), EVT_SERIAL_CLIENT, &request));

                if (error) {
                    return (ES_STATE_TRANSITION(stateIdle));
                }
                ((struct evtSerialClient *)request)->epa = SerialRadio;
                ES_ENSURE(esEpaSendEvent(SerialBt, request));

                return (ES_STATE_TRANSITION(stateIdle));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}
