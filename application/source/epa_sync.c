
#include "arch/intr.h"
#include "epa_sync.h"
#include "app_timer.h"
#include "driver/uart.h"
#include "epa_serial.h"
#include "config/config_project.h"
#include "config/config_pins.h"
#include "epa_btdrv.h"
#include "epa_radio.h"

#define SYNC_TABLE(entry)                                                                           \
    entry(stateInit,            TOP)

enum syncStateId {
    ES_STATE_ID_INIT(SYNC_TABLE)
};

struct wspace {
    struct appTimer             timeout;
};

static esAction stateInit           (struct wspace *, const esEvent *);

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


static esAction stateInit(struct wspace * space, const esEvent * event) {
    (void)space;
    
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
            ((struct evtSerialClient *)request)->client = BtDrv;
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
            ((struct evtSerialClient *)request)->client = Radio;
            ES_ENSURE(esEpaSendEvent(SerialRadio, request));

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}