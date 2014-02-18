/* 
 * File:    uart.h
 * Author:  nenad
 * Details: Generic UART driver
 *
 * Created on February 13, 2014, 12:47 AM
 */

#ifndef UART_H_
#define	UART_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>

#include "plat/compiler.h"
#include "arch/systimer.h"

#include "driver/lld_uart1.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_UART_CLOSE_WAIT_TICKS    100

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum uartError {
    UART_ERROR_NONE,
    UART_ERROR_BUSY,
    UART_ERROR_TIMEOUT,
    UART_ERROR_OVERFLOW,
    UART_ERROR_PARITY,
    UART_ERROR_FRAME
};

enum uartFlags {
    UART_TX_ENABLE      = (0x1u << 0),
    UART_RX_ENABLE      = (0x1u << 1),
    UART_HARDWARE_FLOW  = (0x1u << 2),

    UART_DATA_BITS_8    = (0x1u << 3),
    UART_DATA_BITS_9    = (0x1u << 4),

    UART_STOP_BITS_1    = (0x1u << 5),
    UART_STOP_BITS_2    = (0x1u << 6),

    UART_PARITY_NONE    = (0x1u << 7),
    UART_PARITY_ODD     = (0x1u << 8),
    UART_PARITY_EVEN    = (0x1u << 9)
};

struct uartId;

struct uartConfig {
    const struct uartId * id;
    uint32_t            flags;
    uint32_t            speed;
    uint32_t            isrPriority;
    struct uartRemap {
        uint8_t             tx;
        uint8_t             rx;
        uint8_t             cts;
        uint8_t             rts;
    }                   remap;
};

struct uartHandle {
    const struct uartId * id;
    const struct uartConfig * config;
    size_t           (* reader)(enum uartError, void *, size_t);
    void *              readBuffer;
    size_t              readSize;
    size_t           (* writer)(enum uartError, void *, size_t);
    void *              writeBuffer;
    size_t              writeSize;
    esAtomic            state;
};

struct uartId {
    void             (* open)(struct uartHandle *);
    void             (* close)(struct uartHandle *);
    bool             (* isReadBuffEmpty)(struct uartHandle *);
    bool             (* isWriteBuffFull)(struct uartHandle *);
    uint32_t         (* read)(struct uartHandle *);
    void             (* write)(struct uartHandle *, uint32_t);
    void             (* readStart)(struct uartHandle *);
    void             (* readStop)(struct uartHandle *);
    void             (* writeStart)(struct uartHandle *);
    void             (* writeStop)(struct uartHandle *);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initUart(
    void);

void uartOpen(
    struct uartHandle *        handle,
    const struct uartConfig *  config);

enum uartError uartClose(
    struct uartHandle * handle);

enum uartError uartRead(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout);

enum uartError uartWrite(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements,
    esSysTimerTick      timeout);

void uartSetReader(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t));

void uartSetWriter(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t));

enum uartError uartReadStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartReadStop(
    struct uartHandle * handle);

enum uartError uartWriteStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartWriteStop(
    struct uartHandle * handle);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of uart.h
 ******************************************************************************/
#endif /* UART_H_ */