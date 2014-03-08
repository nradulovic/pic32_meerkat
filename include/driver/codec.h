/* 
 * File:    codec.h
 * Author:  nenad
 * Details: Audio codec driver
 *
 * Created on February 15, 2014, 11:11 PM
 */

#ifndef CODEC_H_
#define	CODEC_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "plat/compiler.h"
#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_MASTER_CLOCK             12000000ul

#define CODEC_REG_ADDR(page, offset)    (((page) << 11) | ((offset) << 5))

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum codecReg {
    CODEC_REG_STATUS            = CODEC_REG_ADDR(0x1, 0x01),
    CODEC_REG_AUDIO_CTRL_1      = CODEC_REG_ADDR(0x2, 0x00),
    CODEC_REG_SIDETONE          = CODEC_REG_ADDR(0x2, 0x03)
};

struct codecConfig {
    const struct spiConfig * spi;
    struct gpio *       csPort;
    uint32_t            csPin;
};

struct codecHandle {
    const struct codecConfig * config;
    struct spiHandle    spi;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initCodec(
    void);

void codecOpen(
    struct codecHandle * handle,
    const struct codecConfig * config);

void codecWriteReg(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t            value);

uint16_t codecReadReg(
    struct codecHandle * handle,
    enum codecReg       reg);

void codecClose(
    struct codecHandle * handle);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of spi.h
 ******************************************************************************/
#endif /* CODEC_H_ */
