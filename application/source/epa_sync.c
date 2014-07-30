
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
#include "eds/queue.h"

#define CONFIG_BLOCK_TIMEOUT                100

#define SYNC_TABLE(entry)                                                                           \
    entry(stateInit,            TOP)                                                                \
    entry(stateIdle,            TOP)                                                                \
    entry(stateClient,          TOP)                                                                \
    entry(stateOther,           TOP)                                                                

enum syncStateId {
    ES_STATE_ID_INIT(SYNC_TABLE)
};

enum evtLocalId {
    EVT_LOCAL_TIMEOUT = ES_EVENT_USER_ID,
    EVT_LOCAL_FLUSH
};

struct wspace {
    struct appTimer     timeout;
    struct esEventQ     wait;
    void *              waitStorage[CONFIG_SYNC_QUEUE_SIZE];
    struct syncRoute    route;
    volatile int        protector;
};

static esAction stateInit           (void *, const esEvent *);
static esAction stateIdle           (void *, const esEvent *);
static esAction stateClient         (void *, const esEvent *);
static esAction stateOther          (void *, const esEvent *);

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

struct esEpa *          SyncBt;
struct esEpa *          SyncRadio;


static esAction stateInit(void * space, const esEvent * event) {
    struct wspace *     wspace = space;
    
    switch (event->id) {
        case ES_INIT : {
            appTimerInit(&wspace->timeout);
            wspace->protector = 0xdeaddead;
            esQueueInit(&wspace->wait, &wspace->waitStorage[0], CONFIG_SYNC_QUEUE_SIZE);

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_REGISTER : {
            struct eventSyncRegister * eventRegister =
                (struct eventSyncRegister *)event;

            wspace->route.client = eventRegister->route.client;
            wspace->route.common = eventRegister->route.common;
            wspace->route.other  = eventRegister->route.other;

            return (ES_STATE_TRANSITION(stateIdle));
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
            if (esQueueFlush(&wspace->wait) != ES_ERROR_NONE) {
                appTimerStart(&wspace->timeout,
                    ES_VTMR_TIME_TO_TICK_MS(20), EVT_LOCAL_FLUSH);
            }

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_FLUSH : {
            
            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_SYNC_REGISTER :
        case EVT_SYNC_DONE     : {

            return (ES_STATE_HANDLED());
        }
        case EVT_SYNC_REQUEST : {
            if (event->producer == wspace->route.client) {
                esError         error;
                esEvent *       response;

                ES_ENSURE(error = esEventCreate(sizeof(esEvent),
                    EVT_SYNC_GRANTED, &response));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(wspace->route.client, response));

                    return (ES_STATE_TRANSITION(stateClient));
                }
            }

            return (ES_STATE_HANDLED());
        }
        default : {
            if (event->producer == wspace->route.client) {
                esEpaSendEvent(wspace->route.common, (esEvent *)event);

                return (ES_STATE_TRANSITION(stateClient));
            } else if (event->producer == wspace->route.common) {
                esEpaSendEvent(wspace->route.other, (esEvent *)event);

                return (ES_STATE_TRANSITION(stateOther));
            } else if (event->producer == wspace->route.other) {
                esEpaSendEvent(wspace->route.common, (esEvent *)event);

                return (ES_STATE_TRANSITION(stateOther));
            }

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateOther(void * space, const esEvent * event) {
    struct wspace *     wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(CONFIG_BLOCK_TIMEOUT), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            
            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_SYNC_REGISTER : {

            return (ES_STATE_HANDLED());
        }
        default : {
            if (event->producer == wspace->route.client) {
                esQueuePut(&wspace->wait, (esEvent *)event);

                return (ES_STATE_HANDLED());
            } else if (event->producer == wspace->route.common) {
                esEpaSendEvent(wspace->route.other, (esEvent *)event);
                
                return (ES_STATE_TRANSITION(stateOther));
            } else if (event->producer == wspace->route.other) {
                esEpaSendEvent(wspace->route.common, (esEvent *)event);

                return (ES_STATE_TRANSITION(stateOther));
            }

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateClient(void * space, const esEvent * event) {
    struct wspace *     wspace = space;

    switch (event->id) {
        case EVT_SYNC_DONE : {

            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_SYNC_REGISTER : {

            return (ES_STATE_HANDLED());
        }
        default : {
            if (event->producer == wspace->route.client) {
                esEpaSendEvent(wspace->route.common, (esEvent *)event);

                return (ES_STATE_HANDLED());
            } else if (event->producer == wspace->route.common) {
                esEpaSendEvent(wspace->route.client, (esEvent *)event);

                return (ES_STATE_HANDLED());
            } else if (event->producer == wspace->route.other) {
                esQueuePut(&wspace->wait, (esEvent *)event);
                
                return (ES_STATE_HANDLED());
            }

            return (ES_STATE_IGNORED());
        }
    }
}
