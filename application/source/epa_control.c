

#include "epa_control.h"
#include "epa_radio.h"
#include "app_timer.h"
#include "epa_codec.h"
#include "app_pdetector.h"
#include "epa_btman.h"
#include "driver/gpio.h"


#define CONFIG_PTT_PRESSED            "+PTT=P\r\n"
#define CONFIG_PTT_RELEASED           "+PTT=R\r\n"

#define CONTROL_TABLE(entry)                                                                        \
    entry(stateInit,            TOP)                                                                \
    entry(stateRadioUsable,     TOP)                                                                \
    entry(stateRadioNotUsable,  TOP)                                                                \

enum controlStateId {
    ES_STATE_ID_INIT(CONTROL_TABLE)
};

enum evtLocalId {
    EVT_LOCAL_TIMEOUT = ES_EVENT_USER_ID
};

static esAction stateInit           (void *, const esEvent *);
static esAction stateRadioUsable    (void *, const esEvent *);
static esAction stateRadioNotUsable (void *, const esEvent *);

static const ES_MODULE_INFO_CREATE("Control", "Control ", "Nenad Radulovic");

static const esSmTable      ControlTable[] = ES_STATE_TABLE_INIT(CONTROL_TABLE);

const struct esEpaDefine    ControlEpa = ES_EPA_DEFINE(
    "Control",
    CONFIG_CONTROL_PRIORITY,
    CONFIG_CONTROL_QUEUE_SIZE);

const struct esSmDefine ControlSm = ES_SM_DEFINE(
    ControlTable,
    0,
    stateInit);

struct esEpa *          Control;


static esAction stateInit(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_INIT : {
            gpioSetAsOutputPullUp(CONFIG_RADIO_PTT_PORT, CONFIG_RADIO_PTT_PIN);

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NET_LOW :
        case EVT_RADIO_NO_NETW:
        case EVT_RADIO_NO_DEVICE: {

            return (ES_STATE_TRANSITION(stateRadioNotUsable));
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateRadioUsable));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateRadioNotUsable(void * space, const esEvent * event) {
    (void)space;

    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent),
                EVT_CODEC_ENABLE_AUDIO, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ES_ENSURE(esEpaSendEvent(Codec, request));

            return (ES_STATE_HANDLED());
        }
        case EVT_PDETECT_PRESS : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct eventBtManSend),
                EVT_BT_MAN_SEND, &request));

            if (!error) {
                struct eventBtManSend * request_ =
                    (struct eventBtManSend *)request;
                request_->data = CONFIG_PTT_PRESSED;
                request_->size = sizeof(CONFIG_PTT_PRESSED) - 1;

                ES_ENSURE(esEpaSendEvent(BtMan, request));
            }
            return (ES_STATE_HANDLED());
        }
        case EVT_PDETECT_RELEASE: {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(struct eventBtManSend),
                EVT_BT_MAN_SEND, &request));

            if (!error) {
                struct eventBtManSend * request_ =
                    (struct eventBtManSend *)request;
                request_->data = CONFIG_PTT_RELEASED;
                request_->size = sizeof(CONFIG_PTT_RELEASED) - 1;

                ES_ENSURE(esEpaSendEvent(BtMan, request));
            }
            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NET_HIGH : {

            return (ES_STATE_TRANSITION(stateRadioUsable));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateRadioUsable(void * space, const esEvent * event) {
    (void)space;
    
    switch (event->id) {
        case ES_ENTRY : {
            esEvent *           request;
            esError             error;

            ES_ENSURE(error = esEventCreate(sizeof(esEvent),
                EVT_CODEC_DISABLE_AUDIO, &request));

            if (error) {
                return (ES_STATE_HANDLED());
            }
            ES_ENSURE(esEpaSendEvent(Codec, request));

            return (ES_STATE_HANDLED());

        }
        case ES_EXIT : {
            *(CONFIG_RADIO_PTT_PORT)->set = (uint32_t)0x1u << CONFIG_RADIO_PTT_PIN;

            return (ES_STATE_HANDLED());
        }
        case EVT_RADIO_NET_LOW :
        case EVT_RADIO_NO_NETW:
        case EVT_RADIO_NO_DEVICE: {

            return (ES_STATE_TRANSITION(stateRadioNotUsable));
        }
        case EVT_PDETECT_PRESS : {
            *(CONFIG_RADIO_PTT_PORT)->clr = (uint32_t)0x1u << CONFIG_RADIO_PTT_PIN;

            return (ES_STATE_HANDLED());
        }
        case EVT_PDETECT_RELEASE: {
            *(CONFIG_RADIO_PTT_PORT)->set = (uint32_t)0x1u << CONFIG_RADIO_PTT_PIN;
            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

