/* 
 * File:    spi.h
 * Author:  nenad
 * Details: Generic SPI driver
 *
 * Created on February 6, 2014, 7:27 PM
 */

#ifndef SPI_H_
#define	SPI_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "plat/compiler.h"
#include "arch/systimer.h"
#include "driver/lld_spi1.h"
#include "driver/lld_spis.h"

/*===============================================================  MACRO's  ==*/

#define SPI_CONFIG_INITIALIZER(channel, flags, speed, priority, timeout, sdi, sdo, sck, ss) \
    {channel, flags, speed, priority, timeout, {sdi, sdo, sck, ss}}

#define CONFIG_SPI_CLOSE_WAIT_TICKS     100

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum spiError {
    SPI_ERROR_NONE,
    SPI_ERROR_BUSY,
    SPI_ERROR_TIMEOUT
};

enum spiConfigFlags {
    SPI_MASTER_MODE              = (0x1u << 5),
    SPI_MASTER_SS                = (0x1u << 28),
    SPI_MASTER_SS_ACTIVE_HIGH    = (0x1u << 29),
    SPI_MASTER_SS_ACTIVE_LOW     = (0x0u << 29),

    SPI_SLAVE_MODE               = 0,
    SPI_SLAVE_SS                 = (0x1u << 7),

    SPI_CLOCK_POLARITY_IDLE_HIGH = (0x1u << 6),
    SPI_CLOCK_POLARITY_IDLE_LOW  = (0x0u << 6),
    SPI_CLOCK_PHASE_FIRST_EDGE   = (0x1u << 8),
    SPI_CLOCK_PHASE_LAST_EDGE    = (0x0u << 8),

    SPI_DATA_8                   = (0x0u << 10),
    SPI_DATA_16                  = (0x1u << 10),
    SPI_DATA_32                  = (0x3u << 10)
};

struct spiId;

struct spiConfig {
    const struct spiId * id;
    enum spiConfigFlags flags;
    uint32_t            speed;
    uint32_t            isrPrio;
    struct spiRemap {
        uint8_t             sdi;
        uint8_t             sdo;
        uint8_t             sck;
        uint8_t             ss;
    }                   remap;
};

struct spiHandle {
    const struct spiId * id;
    const struct spiConfig * config;
    esAtomic            state;
};

struct spiId {
    void             (* open)(struct spiHandle *);
    void             (* close)(struct spiHandle *);
    bool             (* isBuffFull)(struct spiHandle *);
    uint32_t         (* exchange)(struct spiHandle *, uint32_t);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initSpi(
    void);

void spiOpen(
    struct spiHandle *        handle,
    const struct spiConfig *  config);

enum spiError spiClose(
    struct spiHandle *  handle);

enum spiError spiExchange(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of spi.h
 ******************************************************************************/
#endif /* SPI_H_ */
