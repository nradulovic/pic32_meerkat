/*
 * File:   spi.c
 * Author: nenad
 *
 * Created on February 6, 2014, 7:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>

#include "vtimer/vtimer.h"
#include "driver/uart.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define UART_INACTIVE                   0
#define UART_RX_ACTIVE                  (0x1u << 0)
#define UART_TX_ACTIVE                  (0x1u << 1)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void uartTimeout(
    void *              arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void uartTimeout(
    void *              arg) {

    *(volatile esAtomic *)arg = true;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void uartInit(
    struct uartHandle *        handle,
    const struct uartConfig *  config,
    const struct uartId *      id) {

    handle->id = id;
    handle->id->open(config);
    handle->state = UART_INACTIVE;
}

enum uartError uartTerm(
    struct uartHandle * handle) {

    volatile esAtomic   timerIsFinished;
    esVTimer            timerTimeout;

    timerIsFinished = true;
    esVTimerInit(&timerTimeout);

    if (CONFIG_UART_CLOSE_WAIT_TICKS > 1) {
        esVTimerStart(
            &timerTimeout,
            CONFIG_UART_CLOSE_WAIT_TICKS,
            uartTimeout,
            (void *)&timerIsFinished);
        timerIsFinished = false;
    }
    while ((handle->state != UART_INACTIVE) && (timerIsFinished == false));

    if (handle->state != UART_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (UART_ERROR_BUSY);
    }
    esVTimerTerm(&timerTimeout);
    handle->state = UART_INACTIVE;
    (* handle->id->close)();

    return (UART_ERROR_NONE);
}

enum uartError uartRead(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

enum uartError uartWrite(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements) {

    size_t              transmitted;
    volatile esAtomic   timerIsFinished;
    enum uartError      error;
    esVTimer            timerTimeout;

    timerIsFinished = true;
    esVTimerInit(&timerTimeout);

    if (handle->config->relTimeout != 0) {
        esVTimerStart(
            &timerTimeout,
            handle->config->relTimeout * nElements,
            uartTimeout,
            (void *)&timerIsFinished);
        timerIsFinished = false;
    }
    while ((handle->state != UART_INACTIVE) && (timerIsFinished == false));

    if (handle->state != UART_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (UART_ERROR_BUSY);
    }
    handle->state |= UART_TX_ACTIVE;
    transmitted = 0u;

    switch (1 /* TODO: change this*/) {
        case 0: {
            uint8_t *  buffer_ = (uint8_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        case 1 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
    }
    esVTimerTerm(&timerTimeout);

    if ((timerIsFinished == false)) {
        error = UART_ERROR_NONE;
    } else {
        error = UART_ERROR_TIMEOUT;
    }
    handle->state &= ~UART_TX_ACTIVE;

    return (error);
}

void uartSetReader(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t)) {

    handle->reader = notify;
}

void uartSetWriter(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t)) {

    handle->writer = notify;
}

void uartReadStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartReadStop(
    struct uartHandle * handle);

void uartWriteStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartWriteStop(
    struct uartHandle * handle);

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/

