/*
 * File:   epa_radio.h
 * Author: nenad
 *
 * Created on April 27, 2014, 1:12 PM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_SERIAL_H
#define EPA_SERIAL_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "eds/epa.h"
#include "driver/uart.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_SERIAL_PRIORITY           30
#define CONFIG_SERIAL_QUEUE_SIZE         10
#define CONFIG_SERIAL_EVENT_BASE         1500

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum serialEvents {
    EVT_SERIAL_OPEN = CONFIG_SERIAL_EVENT_BASE,
    EVT_SERIAL_CLIENT,
    EVT_SERIAL_PACKET,
    EVT_SERIAL_RECV,
    EVT_SERIAL_SEND_CANCEL,
    EVT_SERIAL_RECV_CANCEL
};

struct evtSerialOpen {
    struct esEvent      event;
    struct uartConfig   config;
};

struct evtSerialClient {
    struct esEvent      event;
    esEpa *             client;
};

struct evtSerialPacket {
    struct esEvent      event;
    void *              data;
    size_t              size;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine SerialEpa;
extern const struct esSmDefine  SerialSm;
extern struct esEpa *   SerialBt;
extern struct esEpa *   SerialRadio;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_serial.h
 ******************************************************************************/
#endif /* EPA_SERIAL_H */

