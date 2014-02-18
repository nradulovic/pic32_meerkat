/*
 * File:    spi.c
 * Author:  nenad
 * Details: Generic SPI driver
 *
 * Created on February 6, 2014, 7:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "vtimer/vtimer.h"
#include "driver/spi.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SPI_DATA_Msk                    (0x3 << 10)

/*======================================================  LOCAL DATA TYPES  ==*/

#define SPI_INACTIVE                    0u
#define SPI_ACTIVE                      1u

#define SPI_TIMER_COUNTING              0u
#define SPI_TIMER_FIRED                 1u

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void spiTimeout(
    void *              arg);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void spiTimeout(void * arg) {

    *(volatile esAtomic *)arg = SPI_TIMER_FIRED;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initSpi(
    void) {
    /*
     * NOTE: This is the place to do general and system wide initializaion
     */
}

void spiOpen(
    struct spiHandle *        handle,
    const struct spiConfig *  config) {

    handle->id     = config->id;
    handle->config = config;
    handle->id->open(handle);
    handle->state  = SPI_INACTIVE;
}


enum spiError spiClose(
    struct spiHandle * handle) {

    volatile esAtomic   timerState;
    esVTimer            timerTimeout;

    timerState = SPI_TIMER_COUNTING;
    esVTimerInit(&timerTimeout);

    if (CONFIG_SPI_CLOSE_WAIT_TICKS > 1) {
        esVTimerStart(
            &timerTimeout,
            CONFIG_SPI_CLOSE_WAIT_TICKS,
            spiTimeout,
            (void *)&timerState);
    }
    while ((handle->state != SPI_INACTIVE) && (timerState == SPI_TIMER_COUNTING));

    if (handle->state != SPI_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (SPI_ERROR_BUSY);
    }
    esVTimerTerm(&timerTimeout);
    handle->state = SPI_INACTIVE;
    handle->id->close(handle);

    return (SPI_ERROR_NONE);
}

enum spiError spiExchange(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout) {

    size_t              transmitted;
    volatile esAtomic   timerState;
    enum spiError       error;
    esVTimer            timerTimeout = ES_VTIMER_INITIALIZER();

    timerState = SPI_TIMER_COUNTING;
    esVTimerInit(&timerTimeout);

    if (timeout != 0) {
        esVTimerStart(
            &timerTimeout,
            timeout,
            spiTimeout,
            (void *)&timerState);
    }
    while ((handle->state == SPI_INACTIVE) && (timerState == SPI_TIMER_COUNTING));

    if (handle->state != SPI_INACTIVE) {
        esVTimerTerm(&timerTimeout);

        return (SPI_ERROR_BUSY);
    }
    handle->state = SPI_ACTIVE;
    transmitted = 0u;

    switch (handle->config->flags & SPI_DATA_Msk) {
        case SPI_DATA_8: {
            uint8_t *  buffer_ = (uint8_t *)buffer;

            while ((timerState == SPI_TIMER_COUNTING) && (transmitted < nElements)) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        case SPI_DATA_16 : {
            uint16_t *  buffer_ = (uint16_t *)buffer;

            while ((timerState == SPI_TIMER_COUNTING) && (transmitted < nElements)) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
        default : {
            uint32_t *  buffer_ = (uint32_t *)buffer;

            while ((timerState == SPI_TIMER_COUNTING) && (transmitted < nElements)) {
                if (handle->id->isBuffFull(handle) != true) {
                    buffer_[transmitted] = handle->id->exchange(handle, buffer_[transmitted]);
                    transmitted++;
                }
            }
            break;
        }
    }
    esVTimerTerm(&timerTimeout);

    if ((timerState == SPI_TIMER_COUNTING)) {
        error = SPI_ERROR_NONE;
    } else {
        error = SPI_ERROR_TIMEOUT;
    }
    handle->state = SPI_INACTIVE;

    return (error);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
