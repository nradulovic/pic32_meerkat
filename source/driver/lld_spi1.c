/*
 * File:   spi.c
 * Author: nenad
 *
 * Created on February 11, 2014, 22:32 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>
#include <xc.h>

#include "driver/spi.h"
#include "bsp.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define IEC1_SPI1E                      (0x1u << 4)
#define IEC1_SPI1RX                     (0x1u << 5)
#define IEC1_SPI1TX                     (0x1u << 6)

#define IFS1_SPI1E                      (0x1u << 4)
#define IFS1_SPI1RX                     (0x1u << 5)
#define IFS1_SPI1TX                     (0x1u << 6)

#define IPC7_SPI1IP_Pos                 26
#define IPC7_SPI1IP_Msk                 (0x7u << IPC7_SPI1IP_Pos)
#define IPC7_SPI1IS_Msk                 (0x6u << 24)

#define SPI1CON_ON                      (0x1u << 15)
#define SPI1CON_ENHBUF                  (0x1u << 16)
#define SPI1CON_MCLKSEL                 (0x1u << 23)
#define SPI1CON_FRMEN                   (0x1u << 31)

#define SPI1STAT_SPITBF                 (0x1u << 1)
#define SPI1STAT_SPIRBE                 (0x1u << 5)
#define SPI1STAT_SPIROV                 (0x1u << 6)
#define SPI1STAT_SPITUR                 (0x1u << 8)

/*======================================================  LOCAL DATA TYPES  ==*/
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
static void lldSpiWrite(
    uint32_t);

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

const struct spiId GlobalSpi1 = {
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

    unsigned int data;
    
    SPI1CON             = 0;
    SPI1CON2            = 0;
    IEC1CLR             = IEC1_SPI1TX | IEC1_SPI1RX | IEC1_SPI1E;               /* Disable all interrupts                                   */
    IFS1CLR             = IFS1_SPI1TX | IFS1_SPI1RX | IFS1_SPI1E;               /* Clear all interrupts                                     */
    data                = SPI1BUF;                                              /* Clear the receive buffer                                 */
    IPC7CLR             = IPC7_SPI1IP_Msk | IPC7_SPI1IS_Msk;                    /* Set ISR priority and clear subpriority                   */
    IPC7SET             = (config->isrPrio << IPC7_SPI1IP_Pos) & IPC7_SPI1IP_Msk;
    SPI1CON             = config->flags & ~SPI1CON_ON;
    SPI1CONCLR          = SPI1CON_FRMEN | SPI1CON_MCLKSEL;
    SPI1CONSET          = SPI1CON_ENHBUF;
    
    if ((config->speed * 2u) >= bspGetPBfclock()) {
        SPI1BRG         = 0;                                                    /* Maximum SPI speed                                        */
    } else {
        unsigned int    divisor;
        
        divisor  = (bspGetPBfclock() / (config->speed * 2u)) - 1u;
        divisor &= 0x0fffu;
        SPI1BRG         = divisor;
    }
    SPI1STATCLR         = SPI1STAT_SPIROV | SPI1STAT_SPITUR;                    /* Clear events                                             */
    SPI1CONSET          = SPI1CON_ON;                                           /* Start SPI module                                         */
    
    /*--  Remap port pins  ---------------------------------------------------*/
    SDI1Rbits.SDI1R     = config->remap.sdi;
    SS1Rbits.SS1R       = config->remap.ssi;
    *config->remap.sdo  = 0x03u;
    *config->remap.sso  = 0x03u;
}

static void lldSpiClose(
    void) {

    SPI1CON             = 0;
    IEC1CLR             = IEC1_SPI1TX | IEC1_SPI1RX | IEC1_SPI1E;               /* Disable all interrupts                                   */
    IFS1CLR             = IFS1_SPI1TX | IFS1_SPI1RX | IFS1_SPI1E;               /* Clear all interrupts                                     */
}

static bool lldSpiIsReadBuffEmpty(
    void) {

    if ((SPI1STAT & SPI1STAT_SPIRBE) != 0) {

        return (true);
    } else {

        return (false);
    }
}

static bool lldSpiIsWriteBuffFull(
    void) {

    if ((SPI1STAT & SPI1STAT_SPITBF) != 0) {

        return (true);
    } else {

        return (false);
    }
}

static uint32_t lldSpiRead(
    void) {

    return (SPI1BUF);
}

static void lldSpiWrite(
    uint32_t            data) {

    SPI1BUF = data;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
