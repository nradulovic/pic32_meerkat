/*
 * File:    spi.c
 * Author:  nenad
 * Details: Generic UART driver
 *
 * Created on February 6, 2014, 7:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "vtimer/vtimer.h"
#include "driver/uart.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define UART_INACTIVE                   0
#define UART_RX_ACTIVE                  (0x1u << 0)
#define UART_TX_ACTIVE                  (0x1u << 1)

#define UART_TIMER_COUNTING             0u
#define UART_TIMER_FIRED                1u

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void uartTimeout(
    void *              arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void uartTimeout(
    void *              arg) {

    *(volatile esAtomic *)arg = UART_TIMER_FIRED;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initUartDriver(
    void) {
    /*
     * NOTE: This is the place to do general and system wide initializaion
     */
}

void uartOpen(
    struct uartHandle *        handle,
    const struct uartConfig *  config) {

    handle->id     = config->id;
    handle->flags  = config->flags;
    handle->reader = NULL;
    handle->writer = NULL;
    handle->id->open(config, handle);
    handle->state  = UART_INACTIVE;
}

enum uartError uartClose(
    struct uartHandle * handle) {

    if (handle->state != UART_INACTIVE) {

        return (UART_ERROR_BUSY);
    }
    handle->state = UART_INACTIVE;
    handle->id->close(handle);

    return (UART_ERROR_NONE);
}

enum uartError uartRead(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout) {

    size_t              received;
    volatile esAtomic   timerState;
    enum uartError      error;
    esVTimer            timerTimeout = ES_VTIMER_INITIALIZER();

    timerState = UART_TIMER_COUNTING;
    esVTimerInit(&timerTimeout);

    if (timeout != 0) {
        esVTimerStart(
            &timerTimeout,
            timeout,
            uartTimeout,
            (void *)&timerState);
    }
    while (((handle->state & UART_RX_ACTIVE) != 0u) && (timerState == UART_TIMER_COUNTING));

    if ((handle->state & UART_RX_ACTIVE) != 0u) {
        esVTimerCancel(&timerTimeout);

        return (UART_ERROR_BUSY);
    }
    handle->state |= UART_RX_ACTIVE;
    received = 0u;

    if (handle->flags & UART_DATA_BITS_9) {
        uint16_t *  buffer_ = (uint16_t *)buffer;

        while ((timerState == UART_TIMER_COUNTING) && (received < nElements)) {
            if (handle->id->isReadBuffEmpty(handle) != true) {
                buffer_[received] = handle->id->read(handle);
                received++;
            }
        }
    } else {
        uint8_t *  buffer_ = (uint8_t *)buffer;

        while ((timerState == UART_TIMER_COUNTING) && (received < nElements)) {
            if (handle->id->isReadBuffEmpty(handle) != true) {
                buffer_[received] = handle->id->read(handle);
                received++;
            }
        }
    }
    esVTimerCancel(&timerTimeout);

    if ((timerState == UART_TIMER_COUNTING)) {
        error = UART_ERROR_NONE;
    } else {
        error = UART_ERROR_TIMEOUT;
    }
    handle->state &= ~UART_RX_ACTIVE;

    return (error);
}

enum uartError uartWrite(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout) {

    size_t              transmitted;
    volatile esAtomic   timerState;
    enum uartError      error;
    esVTimer            timerTimeout = ES_VTIMER_INITIALIZER();

    timerState = UART_TIMER_COUNTING;
    esVTimerInit(&timerTimeout);

    if (timeout != 0) {
        esVTimerStart(
            &timerTimeout,
            timeout,
            uartTimeout,
            (void *)&timerState);
    }
    while (((handle->state & UART_TX_ACTIVE) != 0u) && (timerState == UART_TIMER_COUNTING));

    if ((handle->state & UART_TX_ACTIVE) != 0u) {
        esVTimerCancel(&timerTimeout);

        return (UART_ERROR_BUSY);
    }
    handle->state |= UART_TX_ACTIVE;
    transmitted = 0u;

    if (handle->flags & UART_DATA_BITS_9) {
        uint16_t *  buffer_ = (uint16_t *)buffer;

        while ((timerState == UART_TIMER_COUNTING) && (transmitted < nElements)) {
            if (handle->id->isWriteBuffFull(handle) != true) {
                handle->id->write(handle, buffer_[transmitted]);
                transmitted++;
            }
        }
    } else {
        uint8_t *  buffer_ = (uint8_t *)buffer;

        while ((timerState == UART_TIMER_COUNTING) && (transmitted < nElements)) {
            if (handle->id->isWriteBuffFull(handle) != true) {
                handle->id->write(handle, buffer_[transmitted]);
                transmitted++;
            }
        }
    }
    esVTimerCancel(&timerTimeout);

    if ((timerState == UART_TIMER_COUNTING)) {
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
    size_t           (* notify)(enum uartError, const void *, size_t)) {

    handle->writer = notify;
}

enum uartError uartReadStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements) {

    if ((handle->state & UART_RX_ACTIVE) != 0u) {

        return (UART_ERROR_BUSY);
    }
    handle->state     |= UART_RX_ACTIVE;
    handle->readBuffer = buffer;
    handle->readSize   = nElements;
    handle->id->readStart(handle);

    return (UART_ERROR_NONE);
}

void uartReadCancel(
    struct uartHandle * handle) {

    if (handle->state &   UART_RX_ACTIVE) {
        handle->state &= ~UART_RX_ACTIVE;
        handle->id->readCancel(handle);
    }
}

void uartReadStop(
    struct uartHandle * handle) {

    handle->id->readStop(handle);
    handle->state &= ~UART_RX_ACTIVE;
}

enum uartError uartWriteStart(
    struct uartHandle * handle,
    const void *        buffer,
    size_t              nElements) {

    if ((handle->state & UART_TX_ACTIVE) != 0u) {

        return (UART_ERROR_BUSY);
    }
    handle->state      |= UART_TX_ACTIVE;
    handle->writeBuffer = buffer;
    handle->writeSize   = nElements;
    handle->id->writeStart(handle);

    return (UART_ERROR_NONE);
}

void uartWriteStop(
    struct uartHandle * handle) {

    if (handle->state &   UART_TX_ACTIVE) {
        handle->state &= ~UART_TX_ACTIVE;
        handle->id->writeStop(handle);
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of uart.c
 ******************************************************************************/

