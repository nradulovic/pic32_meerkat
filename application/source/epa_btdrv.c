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
#include "vtimer/vtimer.h"
#include "app_timer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define BT_DRV_CMD_EXPAND_SIZE(id, cmd, args)                                   \
    char cmdSize ## id[sizeof(cmd) + args + sizeof("\r\n") - 1u];

#define BT_DRV_CMD_EXPAND_CMD(id, cmd, args)                                    \
    {cmd, sizeof(cmd) - 1u},

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
    entry(stateCmdBegin,        TOP)                                            \
    entry(stateCmdIdle,         TOP)                                            \
    entry(stateCmdSend,         TOP)                                            \
    entry(stateCmdEnd,          TOP)                                            \
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
    EVT_LOCAL_TIMEOUT = ES_EVENT_LOCAL_ID,
    EVT_LOCAL_INIT_TIMEOUT,
    EVT_LOCAL_CMD_BEGIN_TIMEOUT,
    EVT_LOCAL_CMD_SEND_TIMEOUT,
    EVT_LOCAL_CMD_END_TIMEOUT
};

struct wspace {
    struct esEpa *      client;
    struct appTimer     timeout;
    size_t              reqSize;
    char                reqBuffer[sizeof(union BtDrvCmdSize)];
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit               (void *, const esEvent *);
static esAction stateIdle               (void *, const esEvent *);
static esAction stateCmdBegin           (void *, const esEvent *);
static esAction stateCmdIdle            (void *, const esEvent *);
static esAction stateCmdSend            (void *, const esEvent *);
static esAction stateCmdEnd             (void *, const esEvent *);
static esAction stateDefToggle0         (void *, const esEvent *);
static esAction stateDefToggle1         (void *, const esEvent *);
static esAction stateDefToggle2         (void *, const esEvent *);
static esAction stateDefToggle3         (void *, const esEvent *);

/*--  Support functions  -----------------------------------------------------*/

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

static esAction stateInit (void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            initBtDrv(space);
            BT_PWR_LOW();
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            BT_PWR_HIGH();

            return (ES_STATE_TRANSITION(stateDefToggle3));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle0(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch(event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            BT_DEF_INIT_OUT();
            BT_DEF_LOW();

            return (ES_STATE_TRANSITION(stateDefToggle1));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle1(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch(event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            BT_DEF_HIGH();

            return (ES_STATE_TRANSITION(stateDefToggle2));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle2(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch(event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            BT_DEF_LOW();

            return (ES_STATE_TRANSITION(stateDefToggle3));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateDefToggle3(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch(event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_INIT_TIMEOUT);
            BT_DEF_HIGH();
            //BT_DEF_INIT_IN();

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_INIT_TIMEOUT : {
            esEvent *           notify;
            /*
             * TODO: Should broadcast here it is ready
             */
            ES_ENSURE(esEventCreate(sizeof(esEvent), EVT_BT_NOTIFY_READY, &notify));
            ES_ENSURE(esEpaSendEvent(BtMan, notify));

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
        case EVT_BT_CMD_MODE_ENTER : {
            wspace->client  = event->producer;

            return (ES_STATE_TRANSITION(stateCmdBegin));
        }
        case EVT_BT_RESTART : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(1000u), EVT_LOCAL_TIMEOUT);
            BT_PWR_LOW();

            return (ES_STATE_HANDLED());
        }
        case EVT_LOCAL_TIMEOUT: {
            BT_PWR_HIGH();

            return (ES_STATE_HANDLED());
        }
        case EVT_BT_SEND_DATA : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET, 
                &request));
            
            if (!error) {
                ((struct evtSerialPacket *)request)->data = ((struct evtBtSend *)event)->arg;
                ((struct evtSerialPacket *)request)->size = ((struct evtBtSend *)event)->argSize;
                ES_ENSURE(esEpaSendEvent(SerialBt, request));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdBegin(void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_BT_TIMEOUT_MS),
                EVT_LOCAL_CMD_BEGIN_TIMEOUT);
            BT_CMD_LOW();

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {
            esError             error;
            const struct evtSerialPacket * packet;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;
            
            if ((packet->size >= sizeof(BT_CMD_BEGIN) - 1u) &&
                (strncmp(packet->data, BT_CMD_BEGIN, sizeof(BT_CMD_BEGIN) - 1u) == 0)) {
                esEvent *       notify;

                ES_ENSURE(error = esEventCreate(sizeof(esEvent), EVT_BT_NOTIFY_READY, &notify));

                if (!error) {
                    ES_ENSURE(esEpaSendEvent(wspace->client, notify));

                    return (ES_STATE_TRANSITION(stateCmdIdle));
                } else {

                    return (ES_STATE_TRANSITION(stateCmdEnd));
                }
            } else {
                esEvent *       reply;

                ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

                if (!error) {
                    ((struct evtBtReply *)reply)->status  = BT_ERR_FAILURE;
                    ((struct evtBtReply *)reply)->arg     = NULL;
                    ((struct evtBtReply *)reply)->argSize = 0;
                    ES_ENSURE(esEpaSendEvent(wspace->client, reply));
                }

                return (ES_STATE_TRANSITION(stateCmdEnd));
            }
        }
        case EVT_LOCAL_CMD_BEGIN_TIMEOUT : {
            esError             error;
            esEvent *           reply;
            
            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_COMM;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        case EVT_BT_CMD_MODE_ENTER : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_FAILURE;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateCmdIdle (void * space, const esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case EVT_BT_CMD_MODE_EXIT : {

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        case EVT_BT_REQ : {
            const struct evtBtReq * request;
            
            wspace->reqSize = 0u;
            request = (const struct evtBtReq *)event;
            memcpy(&wspace->reqBuffer[wspace->reqSize], BtCmdTable[request->cmd].cmd,
                BtCmdTable[request->cmd].size);
            wspace->reqSize += BtCmdTable[request->cmd].size;

            if (request->arg != NULL) {
                memcpy(&wspace->reqBuffer[wspace->reqSize], request->arg, request->argSize);
                wspace->reqSize += request->argSize;
            }
            memcpy(&wspace->reqBuffer[wspace->reqSize], "\r\n", 2u);
            wspace->reqSize += 2u;

            return (ES_STATE_TRANSITION(stateCmdSend));
        }
        case EVT_BT_CMD_MODE_ENTER : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_FAILURE;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}
static volatile char buffer[200];
static esAction stateCmdSend (void * space, const  esEvent * event) {
    struct wspace * wspace = space;

    switch (event->id) {
        case ES_INIT : {
            esEvent *           packet;
            esError             error;
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(CONFIG_BT_TIMEOUT_MS),
                EVT_LOCAL_CMD_SEND_TIMEOUT);
            ES_ENSURE(error = esEventCreate(sizeof(struct evtSerialPacket), EVT_SERIAL_PACKET,
                &packet));

            if (error) {
                return (ES_STATE_TRANSITION(stateCmdIdle));
            }
            ((struct evtSerialPacket *)packet)->data = wspace->reqBuffer;
            ((struct evtSerialPacket *)packet)->size = wspace->reqSize;
            ES_ENSURE(esEpaSendEvent(SerialBt, packet));
            
            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {
            esError             error;
            esEvent *           reply;
            const struct evtSerialPacket * packet;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;
            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply [1]) + packet->size,
                EVT_BT_REPLY, &reply));
memcpy(buffer, packet->data, packet->size);
            if (!error) {

                if ((strncmp(packet->data, BT_CMD_VALID,  sizeof(BT_CMD_VALID)  - 1u) == 0) ||
                    (strncmp(packet->data, BT_CMD_REBOOT, sizeof(BT_CMD_REBOOT) - 1u) == 0)) {
                    ((struct evtBtReply *)reply)->status = BT_ERR_NONE;
                } else {
                    ((struct evtBtReply *)reply)->status = BT_ERR_FAILURE;
                }
                ((struct evtBtReply *)reply)->arg     = (char *)&((struct evtBtReply *)reply)[1];
                ((struct evtBtReply *)reply)->argSize = packet->size;
                memcpy(((struct evtBtReply *)reply)->arg, packet->data, packet->size);
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_TRANSITION(stateCmdIdle));
        }
        case EVT_LOCAL_CMD_SEND_TIMEOUT : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_COMM;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_TRANSITION(stateCmdIdle));
        }
        case EVT_BT_CMD_MODE_EXIT : {

            return (ES_STATE_TRANSITION(stateCmdEnd));
        }
        case EVT_BT_CMD_MODE_ENTER : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_FAILURE;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_HANDLED());
        }
        default : {
            return (ES_STATE_IGNORED());
        }
    }
}



static esAction stateCmdEnd(void * space, const esEvent * event) {
    struct wspace * wspace = space;
    
    switch (event->id) {
        case ES_ENTRY : {
            appTimerStart(&wspace->timeout, ES_VTMR_TIME_TO_TICK_MS(2000),
                EVT_LOCAL_CMD_END_TIMEOUT);
            BT_CMD_HIGH();

            return (ES_STATE_HANDLED());
        }
        case EVT_SERIAL_PACKET : {
            
            const struct evtSerialPacket * packet;
            esError         error;
            esEvent *       reply;

            appTimerCancel(&wspace->timeout);
            packet = (const struct evtSerialPacket *)event;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));
            memcpy(buffer, packet->data, packet->size);

            if (!error) {
                if (strncmp(packet->data, BT_CMD_END, sizeof(BT_CMD_END) - 1u) == 0) {
                    ((struct evtBtReply *)reply)->status  = BT_ERR_NONE;
                } else {
                    ((struct evtBtReply *)reply)->status  = BT_ERR_COMM;
                }
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }
            

            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_LOCAL_CMD_END_TIMEOUT : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_COMM;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_TRANSITION(stateIdle));
        }
        case EVT_BT_CMD_MODE_ENTER : {
            esError             error;
            esEvent *           reply;

            ES_ENSURE(error = esEventCreate(sizeof(struct evtBtReply), EVT_BT_REPLY, &reply));

            if (!error) {
                ((struct evtBtReply *)reply)->status  = BT_ERR_FAILURE;
                ((struct evtBtReply *)reply)->arg     = NULL;
                ((struct evtBtReply *)reply)->argSize = 0;
                ES_ENSURE(esEpaSendEvent(wspace->client, reply));
            }

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*--  Support functions  -----------------------------------------------------*/

static void initBtDrv(struct wspace * space) {

    /*--  Initialize CMD & DEF & PWR pins  -----------------------------------*/
    BT_CMD_INIT_OUT();
    BT_DEF_INIT_IN();
    BT_PWR_INIT_OUT();
    BT_CMD_HIGH();
    BT_DEF_HIGH();
    BT_PWR_HIGH();

    /*--  Initialize timeout timer  ------------------------------------------*/
    appTimerInit(&space->timeout);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/
