/* 
 * File:   uart.h
 * Author: nenad
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
    UART_ERROR_TIMEOUT
};

struct uartId;

struct uartConfig {
    const struct uartId * id;
    uint32_t            flags;
    uint32_t            speed;
    esSysTimerTick      relTimeout;
};

struct uartHandle {
    const struct uartId * id;
    const struct uartConfig * config;
    size_t           (* reader)(enum uartError, void *, size_t);
    size_t           (* writer)(enum uartError, void *, size_t);
    esAtomic            state;
};

struct uartId {
    void             (* open)(const struct uartConfig *);
    void             (* close)(void);
    bool             (* isReadBuffEmpty)(void);
    bool             (* isWriteBuffFull)(void);
    uint32_t         (* read)();
    void             (* write)(uint32_t);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void uartInit(
    struct uartHandle *        handle,
    const struct uartConfig *  config,
    const struct uartId *      id);

enum uartError uartTerm(
    struct uartHandle * handle);

enum uartError uartRead(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

enum uartError uartWrite(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartSetReader(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t));

void uartSetWriter(
    struct uartHandle * handle,
    size_t           (* notify)(enum uartError, void *, size_t));

void uartReadStart(
    struct uartHandle * handle,
    void *              buffer,
    size_t              nElements);

void uartReadStop(
    struct uartHandle * handle);

void uartWriteStart(
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