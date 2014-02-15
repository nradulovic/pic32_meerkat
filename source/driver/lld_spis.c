/*
 * File:   lld_spis.c
 * Author: nenad
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
    const struct spiConfig *);
static void lldSpiClose(
    void);
static bool lldSpiIsReadBuffEmpty(
    void);
static bool lldSpiIsWriteBuffFull(
    void);
static uint32_t lldSpiRead(
    void);
static uint32_t lldSpiWrite(
    uint32_t);

/*=======================================================  LOCAL VARIABLES  ==*/

static const struct spisGpio Gpio[SPIS_LAST_PIN_ID] = {
    SPIS_PIN_TABLE(SPIS_GPIO)
};

static struct spiConfig     Config;
static struct spisPolarized Polarized;

/*======================================================  GLOBAL VARIABLES  ==*/

const struct spiId GlobalSpis = {
    lldSpiOpen,
    lldSpiClose,
    lldSpiIsReadBuffEmpty,
    lldSpiIsWriteBuffFull,
    lldSpiRead,
    lldSpiWrite
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void lldSpiOpen(
    const struct spiConfig * config) {

    Config.flags     = config->flags;
    Config.remap.sdi = config->remap.sdi;
    Config.remap.sdo = config->remap.sdo;
    Config.remap.sck = config->remap.sck;
    Config.remap.ss  = config->remap.ss;

    if (config->flags & SPI_CLOCK_POLARITY_IDLE_HIGH) {
        Polarized.sckActive   = Gpio[Config.remap.sck].clr;
        Polarized.sckInactive = Gpio[Config.remap.sck].set;
        
    } else {
        Polarized.sckActive   = Gpio[Config.remap.sck].set;
        Polarized.sckInactive = Gpio[Config.remap.sck].clr;
    }

    if (config->flags & SPI_MASTER_SS_ACTIVE_HIGH) {
        Polarized.ssActive    = Gpio[Config.remap.ss].set;
        Polarized.ssInactive  = Gpio[Config.remap.ss].clr;
    } else {
        Polarized.ssActive    = Gpio[Config.remap.ss].clr;
        Polarized.ssInactive  = Gpio[Config.remap.ss].set;
    }
    /*--  Setup SCK  ---------------------------------------------------------*/
    *Gpio[Config.remap.sck].tris  &= ~Gpio[Config.remap.sck].bit;
    *Polarized.sckInactive        |= Gpio[Config.remap.sck].bit;

    /*--  Setup SDI  ---------------------------------------------------------*/
    *Gpio[Config.remap.sdi].tris  |= Gpio[Config.remap.sdi].bit;
    
    /*--  Setup SDO  ---------------------------------------------------------*/
    *Gpio[Config.remap.sdo].tris  &= ~Gpio[Config.remap.sdo].bit;
    *Gpio[Config.remap.sdo].clr   |= Gpio[Config.remap.sdo].bit;

    /*--  Setup SS  ----------------------------------------------------------*/
    *Gpio[Config.remap.ss].tris   &= ~Gpio[Config.remap.ss].bit;
    *Polarized.ssInactive         |= Gpio[Config.remap.ss].bit;
}

static void lldSpiClose(
    void) {

    *Polarized.sckInactive        |= Gpio[Config.remap.sck].bit;
    *Polarized.ssInactive         |= Gpio[Config.remap.ss].bit;
    *Gpio[Config.remap.sdo].clr   |= Gpio[Config.remap.sdo].bit;
}

static bool lldSpiIsReadBuffEmpty(
    void) {

    return (false);
}

static bool lldSpiIsWriteBuffFull(
    void) {

    return (false);
}

static uint32_t lldSpiRead(
    void) {

    return (lldSpiWrite(0u));
}

static uint32_t lldSpiWrite(
    uint32_t            data) {

    uint32_t            cnt;
    uint32_t            retval;

    switch (Config.flags & (0x3u << 10)) {
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
    *Polarized.sckInactive |= Gpio[Config.remap.sck].bit;

    if ((Config.flags & SPI_MASTER_SS) != 0) {
        *Polarized.ssActive |= Gpio[Config.remap.ss].bit;
    }
    
    /*
     * Wait SCK/2
     */
    if ((Config.flags & SPI_CLOCK_PHASE_FIRST_EDGE) != 0) {
        retval = 0;

        while (cnt != 0) {
            cnt--;

            /*
             * Wait SCK/2
             */
            if ((data & (0x01 << cnt)) != 0) {
                *Gpio[Config.remap.sdo].set = Gpio[Config.remap.sdo].bit;
            } else {
                *Gpio[Config.remap.sdo].clr = Gpio[Config.remap.sdo].bit;
            }
            /*
             * Wait SCK/2
             */
            *Polarized.sckActive |= Gpio[Config.remap.sck].bit;
            
            if ((*Gpio[Config.remap.sdi].port & Gpio[Config.remap.sdi].bit) != 0) {
                retval |= (0x01 << cnt);
            }
            /*
             * Wait SCK
             */
            *Polarized.sckInactive |= Gpio[Config.remap.sck].bit;
        }
    } else {
        retval = 0;
        
        while (cnt != 0) {
            cnt--;

            /*
             * Wait SCK
             */
            *Polarized.sckActive |= Gpio[Config.remap.sck].bit;
            /*
             * Wait SCK/2
             */
            if ((data & (0x01 << cnt)) != 0) {
                *Gpio[Config.remap.sdo].set = Gpio[Config.remap.sdo].bit;
            } else {
                *Gpio[Config.remap.sdo].clr = Gpio[Config.remap.sdo].bit;
            }
            /*
             * Wait SCK/2
             */
            *Polarized.sckInactive |= Gpio[Config.remap.sck].bit;

            if ((*Gpio[Config.remap.sdi].port & Gpio[Config.remap.sdi].bit) != 0) {
                retval |= (0x01 << cnt);
            }
        }
    }
    
    /*
     * Wait SCK/2
     */
    if ((Config.flags & SPI_MASTER_SS) != 0) {
        *Polarized.ssInactive |= Gpio[Config.remap.ss].bit;
    }

    return (retval);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of lld_spis.c
 ******************************************************************************/

