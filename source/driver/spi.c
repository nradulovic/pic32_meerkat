/*
 * File:   spi.c
 * Author: nenad
 *
 * Created on February 6, 2014, 7:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "vtimer/vtimer.h"
#include "driver/spi.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SPI_DATA_Msk                    (0x3 << 10)

/*======================================================  LOCAL DATA TYPES  ==*/


#define SPI_INACTIVE                    0
#define SPI_RX_ACTIVE                   (0x1u << 0)
#define SPI_TX_ACTIVE                   (0x1u << 1)

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void spiTimeout(
    void *              arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void spiTimeout(void * arg) {

    *(volatile esAtomic *)arg = true;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void spiOpen(
    struct spiHandle *        handle,
    const struct spiConfig *  config) {

    handle->id     = config->id;
    handle->config = config;
    handle->id->open(config);
    handle->state  = SPI_INACTIVE;
}


enum spiError spiClose(
    struct spiHandle * handle) {

    volatile esAtomic   timerIsFinished;
    esVTimer            timerTimeout;

    timerIsFinished = true;
    esVTimerInit(&timerTimeout);

    if (CONFIG_SPI_CLOSE_WAIT_TICKS > 1) {
        esVTimerStart(
            &timerTimeout,
            CONFIG_SPI_CLOSE_WAIT_TICKS,
            spiTimeout,
            (void *)&timerIsFinished);
        timerIsFinished = false;
    }
    while ((handle->state != SPI_INACTIVE) && (timerIsFinished == false));

    if (handle->state != SPI_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (SPI_ERROR_BUSY);
    }
    esVTimerTerm(&timerTimeout);
    handle->state = SPI_INACTIVE;
    (* handle->id->close)();

    return (SPI_ERROR_NONE);
}

enum spiError spiRead(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements) {

    size_t              received;
    volatile esAtomic   timerIsFinished;
    enum spiError       error;
    esVTimer            timerTimeout;

    timerIsFinished = true;
    esVTimerInit(&timerTimeout);

    if (handle->config->relTimeout != 0) {
        esVTimerStart(
            &timerTimeout,
            handle->config->relTimeout * nElements,
            spiTimeout,
            (void *)&timerIsFinished);
        timerIsFinished = false;
    }
    while ((handle->state != SPI_INACTIVE) && (timerIsFinished == false));

    if (handle->state != SPI_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (SPI_ERROR_BUSY);
    }
    handle->state |= SPI_RX_ACTIVE;
    received = 0u;

    switch (handle->config->flags & SPI_DATA_Msk) {
        case SPI_DATA_8: {
            uint8_t *  buffer_ = (uint8_t *)buffer;

            while ((timerIsFinished == false) && (received < nElements)) {
                if (handle->id->isReadBuffEmpty() != true) {
                    buffer_[received] = (uint8_t)handle->id->read();
                    received++;
                }
            }
            break;
        }
        case SPI_DATA_16 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while ((timerIsFinished == false) && (received < nElements)) {
                if (handle->id->isReadBuffEmpty() != true) {
                    buffer_[received] = (uint16_t)handle->id->read();
                    received++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while ((timerIsFinished == false) && (received < nElements)) {
                if (handle->id->isReadBuffEmpty() != true) {
                    buffer_[received] = handle->id->read();
                    received++;
                }
            }
            break;
        }
    }
    esVTimerTerm(&timerTimeout);

    if ((timerIsFinished == false)) {
        error = SPI_ERROR_NONE;
    } else {
        error = SPI_ERROR_TIMEOUT;
    }
    handle->state &= ~SPI_RX_ACTIVE;

    return (error);
}

enum spiError spiWrite(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements) {

    size_t              transmitted;
    volatile esAtomic   timerIsFinished;
    enum spiError       error;
    esVTimer            timerTimeout = ES_VTIMER_INITIALIZER();

    timerIsFinished = false;
    esVTimerInit(&timerTimeout);

    if (handle->config->relTimeout != 0) {
        esVTimerStart(
            &timerTimeout,
            handle->config->relTimeout * nElements,
            spiTimeout,
            (void *)&timerIsFinished);
    }
    while ((handle->state != SPI_INACTIVE) && (timerIsFinished == false));

    if (handle->state != SPI_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (SPI_ERROR_BUSY);
    }
    handle->state |= SPI_TX_ACTIVE;
    transmitted = 0u;

    switch (handle->config->flags & SPI_DATA_Msk) {
        case SPI_DATA_8: {
            uint8_t *  buffer_ = (uint8_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    buffer_[transmitted] = handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        case SPI_DATA_16 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    buffer_[transmitted] = handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while ((timerIsFinished == false) && (transmitted < nElements)) {
                if (handle->id->isWriteBuffFull() != true) {
                    buffer_[transmitted] = handle->id->write(buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
    }
    esVTimerTerm(&timerTimeout);

    if ((timerIsFinished == false)) {
        error = SPI_ERROR_NONE;
    } else {
        error = SPI_ERROR_TIMEOUT;
    }
    handle->state &= ~SPI_TX_ACTIVE;

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
