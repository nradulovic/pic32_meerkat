/*
 * File:    lld_spis.c
 * Author:  nenad
 * Details: Low Level Driver for Software SPI
 *
 * Created on February 14, 2014, 8:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <xc.h>

#include "driver/spi.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define SPIS_GPIO(id, bit, port, tris, clr, set)                                \
    {(0x01u << bit), port, tris, clr, set},

/*======================================================  LOCAL DATA TYPES  ==*/

struct spisGpio {
    unsigned int        bit;
    volatile unsigned int * port;
    volatile unsigned int * tris;
    volatile unsigned int * clr;
    volatile unsigned int * set;
};

struct spisPolarized {
    volatile unsigned int * sckActive;
    volatile unsigned int * sckInactive;
    volatile unsigned int * ssActive;
    volatile unsigned int * ssInactive;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void lldSpiOpen(
    struct spiHandle *);
static void lldSpiClose(
    struct spiHandle *);
static bool lldSpiIsBuffFull(
    struct spiHandle *);
static uint32_t lldSpiExchange(
    struct spiHandle *,
    uint32_t);

/*=======================================================  LOCAL VARIABLES  ==*/

static const struct spisGpio Gpio[SPIS_LAST_PIN_ID] = {
    SPIS_PIN_TABLE(SPIS_GPIO)
};

static struct spisPolarized Polarized;

/*======================================================  GLOBAL VARIABLES  ==*/

const struct spiId GlobalSpis = {
    lldSpiOpen,
    lldSpiClose,
    lldSpiIsBuffFull,
    lldSpiExchange
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void lldSpiOpen(
    struct spiHandle *  handle) {

    const struct spiConfig * config;

    config = handle->config;

    if (config->flags & SPI_CLOCK_POLARITY_IDLE_HIGH) {
        Polarized.sckActive   = Gpio[config->remap.sck].clr;
        Polarized.sckInactive = Gpio[config->remap.sck].set;

    } else {
        Polarized.sckActive   = Gpio[config->remap.sck].set;
        Polarized.sckInactive = Gpio[config->remap.sck].clr;
    }

    if (config->flags & SPI_MASTER_SS_ACTIVE_HIGH) {
        Polarized.ssActive    = Gpio[config->remap.ss].set;
        Polarized.ssInactive  = Gpio[config->remap.ss].clr;
    } else {
        Polarized.ssActive    = Gpio[config->remap.ss].clr;
        Polarized.ssInactive  = Gpio[config->remap.ss].set;
    }
    /*--  Setup SCK  ---------------------------------------------------------*/
    *Gpio[config->remap.sck].tris &= ~Gpio[config->remap.sck].bit;
    *Polarized.sckInactive        |= Gpio[config->remap.sck].bit;

    /*--  Setup SDI  ---------------------------------------------------------*/
    *Gpio[config->remap.sdi].tris |= Gpio[config->remap.sdi].bit;

    /*--  Setup SDO  ---------------------------------------------------------*/
    *Gpio[config->remap.sdo].tris &= ~Gpio[config->remap.sdo].bit;
    *Gpio[config->remap.sdo].clr  |= Gpio[config->remap.sdo].bit;

    /*--  Setup SS  ----------------------------------------------------------*/
    *Gpio[config->remap.ss].tris  &= ~Gpio[config->remap.ss].bit;
    *Polarized.ssInactive         |= Gpio[config->remap.ss].bit;
}

static void lldSpiClose(
    struct spiHandle *  handle) {

    const struct spiConfig * config;

    config = handle->config;
    *Polarized.sckInactive        |= Gpio[config->remap.sck].bit;
    *Polarized.ssInactive         |= Gpio[config->remap.ss].bit;
    *Gpio[config->remap.sdo].clr  |= Gpio[config->remap.sdo].bit;
}

static bool lldSpiIsBuffFull(
    struct spiHandle *  handle) {

    (void)handle;

    return (false);
}

static uint32_t lldSpiExchange(
    struct spiHandle *  handle,
    uint32_t            data) {

    const struct spiConfig * config;
    uint32_t            cnt;
    uint32_t            retval;

    config = handle->config;

    switch (config->flags & (0x3u << 10)) {
        case SPI_DATA_8 : {
            cnt = 8;
            break;
        }
        case SPI_DATA_16 : {
            cnt = 16;
            break;
        }
        default : {
            cnt = 32;
            break;
        }
    }
    *Polarized.sckInactive |= Gpio[config->remap.sck].bit;

    if ((config->flags & SPI_MASTER_SS) != 0) {
        *Polarized.ssActive |= Gpio[config->remap.ss].bit;
    }

    /*
     * Wait SCK/2
     */
    if ((config->flags & SPI_CLOCK_PHASE_FIRST_EDGE) != 0) {
        retval = 0;

        while (cnt != 0) {
            cnt--;

            /*
             * Wait SCK/2
             */
            if ((data & (0x01 << cnt)) != 0) {
                *Gpio[config->remap.sdo].set = Gpio[config->remap.sdo].bit;
            } else {
                *Gpio[config->remap.sdo].clr = Gpio[config->remap.sdo].bit;
            }
            /*
             * Wait SCK/2
             */
            *Polarized.sckActive |= Gpio[config->remap.sck].bit;

            if ((*Gpio[config->remap.sdi].port & Gpio[config->remap.sdi].bit) != 0) {
                retval |= (0x01 << cnt);
            }
            /*
             * Wait SCK
             */
            *Polarized.sckInactive |= Gpio[config->remap.sck].bit;
        }
    } else {
        retval = 0;

        while (cnt != 0) {
            cnt--;

            /*
             * Wait SCK
             */
            *Polarized.sckActive |= Gpio[config->remap.sck].bit;
            /*
             * Wait SCK/2
             */
            if ((data & (0x01 << cnt)) != 0) {
                *Gpio[config->remap.sdo].set = Gpio[config->remap.sdo].bit;
            } else {
                *Gpio[config->remap.sdo].clr = Gpio[config->remap.sdo].bit;
            }
            /*
             * Wait SCK/2
             */
            *Polarized.sckInactive |= Gpio[config->remap.sck].bit;

            if ((*Gpio[config->remap.sdi].port & Gpio[config->remap.sdi].bit) != 0) {
                retval |= (0x01 << cnt);
            }
        }
    }

    /*
     * Wait SCK/2
     */
    if ((config->flags & SPI_MASTER_SS) != 0) {
        *Polarized.ssInactive |= Gpio[config->remap.ss].bit;
    }

    return (retval);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of lld_spis.c
 ******************************************************************************/

