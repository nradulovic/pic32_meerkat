/*
 * File:    codec.c
 * Author:  nenad
 * Details: Audio codec driver
 *
 * Created on February 15, 2014, 11:12 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

#include "base/bitop.h"
#include "vtimer/vtimer.h"
#include "driver/codec.h"
#include "driver/clock.h"
#include "bsp.h"
#include "base/bitop.h"
#include "config/config_pins.h"
#include "driver/gpio.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define COMMAND_RW                      ((uint16_t)0x1u << 15)

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static const ES_MODULE_INFO_CREATE("Codec", "Codec driver", "Nenad Radulovic");

/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*
 *  NOTE: The device requires a low-to-high pulse on RESET after power up for
 *        correct operation.
 */
void initCodecDriver(
    void) {

    *(CONFIG_CODEC_POWER_PORT)->tris &= ~(0x1u << CONFIG_CODEC_POWER_PIN);
    *(CONFIG_CODEC_POWER_PORT)->clr   = (0x1u << CONFIG_CODEC_POWER_PIN);
    *(CONFIG_CODEC_RESET_PORT)->tris &= ~(0x1u << CONFIG_CODEC_RESET_PIN);
    *(CONFIG_CODEC_RESET_PORT)->clr   = (0x1u << CONFIG_CODEC_RESET_PIN);
}

void codecResetEnable(
    void) {

    *(CONFIG_CODEC_RESET_PORT)->clr = (0x1u << CONFIG_CODEC_RESET_PIN);
}

void codecResetDisable(
    void) {

    *(CONFIG_CODEC_RESET_PORT)->set = (0x1u << CONFIG_CODEC_RESET_PIN);
}

void codecPowerUp(
    void) {

    *(CONFIG_CODEC_POWER_PORT)->set = (0x1u << CONFIG_CODEC_POWER_PIN);
}

void codecPowerDown(
    void) {

    *(CONFIG_CODEC_POWER_PORT)->clr = (0x1u << CONFIG_CODEC_POWER_PIN);
}

void codecOpen(
    struct codecHandle * handle,
    const struct codecConfig * config) {

    struct spiConfig spiConfig;

    spiConfig.flags     = config->spi->flags & ~(SPI_MASTER_SS);                /* Hardware is not controlling SS pin                       */
    spiConfig.id        = config->spi->id;
    spiConfig.isrPrio   = config->spi->isrPrio;
    spiConfig.remap.sck = config->spi->remap.sck;
    spiConfig.remap.sdi = config->spi->remap.sdi;
    spiConfig.remap.sdo = config->spi->remap.sdo;
    spiConfig.remap.ss  = config->spi->remap.ss;
    spiConfig.speed     = config->spi->speed;
    spiOpen(&handle->spi, &spiConfig);
}

void codecWriteReg(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t            value) {

    uint16_t            buff[2];

    buff[0] = (uint16_t)reg & ~COMMAND_RW;
    buff[1] = value;
    spiSSActivate(&handle->spi);
    spiExchange(&handle->spi, buff, ES_ARRAY_DIMENSION(buff));
    spiSSDeactivate(&handle->spi);
}

uint16_t codecReadReg(
    struct codecHandle * handle,
    enum codecReg       reg) {

    uint16_t            buff[2];

    buff[0] = (uint16_t)reg | COMMAND_RW;
    buff[1] = 0;
    spiSSActivate(&handle->spi);
    spiExchange(&handle->spi, buff, ES_ARRAY_DIMENSION(buff));
    spiSSDeactivate(&handle->spi);

    return (buff[1]);
}

void codecRegModify(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t            mask,
    uint16_t            value) {

    uint16_t            regValue;

    regValue  = codecReadReg(handle, reg);
    regValue &= ~mask;
    regValue |= value & mask;
    codecWriteReg(handle, reg, regValue);
}

void codecWriteArray(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t *          array,
    size_t              size) {

    spiSSActivate(&handle->spi);
    spiExchange(&handle->spi, &reg, 1u);
    spiExchange(&handle->spi, array, size);
    spiSSDeactivate(&handle->spi);
}

void codecClockEnable(
    void) {

    uint32_t            masterClockDiv;
    uint32_t            divider;

    masterClockDiv = clockGetSystemClock() / CONFIG_MASTER_CLOCK;

    switch (masterClockDiv) {
        case 1 : {
            divider = CLOCK_OUT_DIV_1;
            break;
        }
        case 2 : {
            divider = CLOCK_OUT_DIV_2;
            break;
        }
        case 4 : {
            divider = CLOCK_OUT_DIV_4;
            break;
        }
        case 6 : {
            divider = CLOCK_OUT_DIV_6;
            break;
        }
        default : {
            ES_ASSERT_ALWAYS(ES_API_RANGE, "Invalid clock out divider");

            return;
        }
    }
    clockSetOutput(CONFIG_CODEC_CLOCK_OUT_PIN, CLOCK_OUT_SOURCE_SYSCLK, divider);
}

void codecAudioDisable(
    void) {

    clockSetOutput(CLOCK_OUT_DISABLE, CLOCK_OUT_SOURCE_SYSCLK, CLOCK_OUT_DIV_1);
}

void codecClose(
    struct codecHandle * handle) {

    spiClose(&handle->spi);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of spi.c
 ******************************************************************************/
