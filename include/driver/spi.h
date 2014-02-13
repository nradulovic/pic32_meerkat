/* 
 * File:   spi.h
 * Author: nenad
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
#include "driver/lld_spi1.h"

/*===============================================================  MACRO's  ==*/

#define SPI_CONFIG_INITIALIZER(channel, flags, speed, priority, timeout, sdi, sdo, ssi, sso) \
    {channel, flags, speed, priority, timeout, {sdo, sso, sdi, ssi}}

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
    uint32_t            relTimeout;
    struct spiRemap {
        volatile unsigned int * sdo;
        volatile unsigned int * sso;
        uint8_t             ssi;
        uint8_t             sdi;
    }                   remap;
};

struct spiHandle {
    const struct spiId * id;
    const struct spiConfig * config;
    esAtomic            state;
};

struct spiId {
    void             (* open)(const struct spiConfig *);
    void             (* close)(void);
    bool             (* isReadBuffEmpty)(void);
    bool             (* isWriteBuffFull)(void);
    uint32_t         (* read)();
    void             (* write)(uint32_t);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void spiOpen(
    struct spiHandle *        handle,
    const struct spiConfig *  config,
    const struct spiId *      id);

enum spiError spiClose(
    struct spiHandle *  handle);

enum spiError spiRead(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements);

enum spiError spiWrite(
    struct spiHandle *  handle,
    void *              buffer,
    size_t              nElements);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of spi.h
 ******************************************************************************/
#endif /* SPI_H_ */
