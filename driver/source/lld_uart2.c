/*
 * File:    lld_UART2.h
 * Author:  nenad
 * Details: Low Level Driver for UART2
 *
 * Created on February 14, 2014, 9:59 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>
#include <sys/attribs.h>

#include "driver/uart.h"
#include "driver/clock.h"
#include "plat/critical.h"
#include "vtimer/vtimer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define IEC1_U2E                        (0x1u << 21)
#define IEC1_U2RX                       (0x1u << 22)
#define IEC1_U2TX                       (0x1u << 23)

#define IFS1_U2E                        (0x1u << 21)
#define IFS1_U2RX                       (0x1u << 22)
#define IFS1_U2TX                       (0x1u << 23)

#define INTR_U2RX                       (0x1u << 22)
#define INTR_U2TX                       (0x1u << 23)

#define U2MODE_STSEL_2_STOP_BITS        (0x1u << 0)
#define U2MODE_PDSEL_9_NO               (0x3u << 1)
#define U2MODE_PDSEL_8_ODD              (0x2u << 1)
#define U2MODE_PDSEL_8_EVEN             (0x1u << 1)
#define U2MODE_EN_CTS_RTS               (0x2u << 8)
#define U2MODE_ON                       (0x1u << 15)

#define U2STA_URXDA                     (0x1u << 0)
#define U2STA_OERR                      (0x1u << 1)
#define U2STA_FERR                      (0x1u << 2)
#define U2STA_PERR                      (0x1u << 3)
#define U2STA_RXISEL_3_4                (0x2u << 6)
#define U2STA_RXISEL_1_2                (0x1u << 6)
#define U2STA_TRMT                      (0x1u << 8)
#define U2STA_UTXBF                     (0x1u << 9)
#define U2STA_TXEN                      (0x1u << 10)
#define U2STA_RXEN                      (0x1u << 12)
#define U2STA_TXISEL_ALL                (0x1u << 14)

#define U2STA_URXISEL_Msk               (0x3u << 6)

#define UART2_PIN_ADDRESS(id, value, address)                                   \
    address,

#define UART2_PIN_VALUE(id, value, address)                                     \
    value,

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static void uartUpdateRxTrigger(
    size_t);
static void lldUartOpen(
    const struct uartConfig *,
    struct uartHandle *);
static void lldUartClose(
    struct uartHandle *);
static bool lldUartIsReadBuffEmpty(
    struct uartHandle *);
static bool lldUartIsWriteBuffFull(
    struct uartHandle *);
static uint32_t lldUartRead(
    struct uartHandle *);
static void lldUartWrite(
    struct uartHandle *,
    uint32_t);
static void lldUartReadStart(
    struct uartHandle *);
static void lldUartReadCancel(
    struct uartHandle *);
static void lldUartReadStop(
    struct uartHandle *);
static void lldUartWriteStart(
    struct uartHandle *);
static void lldUartWriteStop(
    struct uartHandle *);

/*=======================================================  LOCAL VARIABLES  ==*/

static struct esVTimer  ReadAfterTimeout;
static struct uartHandle * g_uart2_handle;
static uint32_t         g_uart2_tx_counter;
static uint32_t         g_uart2_rx_counter;
static bool             g_uart2_rx_cancelled;

static volatile unsigned int * const uartPin2Address[UART2_LAST_PIN_ID] = {
    UART2_PIN_TABLE(UART2_PIN_ADDRESS)
};

static const unsigned int uartPin2Value[UART2_LAST_PIN_ID] = {
    UART2_PIN_TABLE(UART2_PIN_VALUE)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct uartId Uart2 = {
    lldUartOpen,
    lldUartClose,
    lldUartIsReadBuffEmpty,
    lldUartIsWriteBuffFull,
    lldUartRead,
    lldUartWrite,
    lldUartReadStart,
    lldUartReadCancel,
    lldUartReadStop,
    lldUartWriteStart,
    lldUartWriteStop
};

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static void uartUpdateRxTrigger(
    size_t              size) {

    U2STACLR = U2STA_URXISEL_Msk;

    if (size >= 6) {
        U2STASET = U2STA_RXISEL_3_4;
    } else if (size >= 4) {
        U2STASET = U2STA_RXISEL_1_2;
    }
}

static void readAfterCallback(void) {
    uartReadCancel(g_uart2_handle);
}

static void lldUartOpen(
    const struct uartConfig * config,
    struct uartHandle * handle) {

    uint32_t            brg;
    uint32_t            mode;
    uint32_t            sta;

    g_uart2_handle       = handle;
    U2MODE              = 0;
    U2STA               = 0;
    IEC1CLR             = IEC1_U2TX | IEC1_U2RX | IEC1_U2E;
    IPC9bits.U2IP       = config->isrPriority;
    IPC9bits.U2IS       = 0;
    sta                 = 0;

    if ((config->flags & UART_TX_ENABLE) != 0u) {
        sta            |= U2STA_TXEN;
    }

    if ((config->flags & UART_RX_ENABLE) != 0u) {
        sta            |= U2STA_RXEN;
    }
    sta                |= U2STA_TXISEL_ALL; /* | U2STA_RXISEL_3_4;*/
    U2STA               = sta;
    brg = clockGetPeripheralClock() / config->speed;
    brg++;
    brg >>= 4;
    brg--;
    brg &= 0xffffu;
    U2BRG               = brg;
    mode                = U2MODE_ON;

    if ((config->flags & UART_HARDWARE_FLOW) != 0u) {
        mode           |= U2MODE_EN_CTS_RTS;
        /*--  Remap CTS & RTS port pins  -------------------------------------*/
        U2CTSRbits.U2CTSR = uartPin2Value[config->remap.cts];
        *uartPin2Address[config->remap.rts] = 0x02u;
    }

    if ((config->flags & UART_DATA_BITS_9) != 0u) {
        mode           |= U2MODE_PDSEL_9_NO;
    } else if ((config->flags & UART_PARITY_EVEN) != 0u) {
        mode           |= U2MODE_PDSEL_8_EVEN;
    } else if ((config->flags & UART_PARITY_ODD) != 0u) {
        mode           |= U2MODE_PDSEL_8_ODD;
    }

    if ((config->flags & UART_STOP_BITS_2) != 0u) {
        mode           |= U2MODE_STSEL_2_STOP_BITS;
    }
    /*--  Remap RX & TX port pins  -------------------------------------------*/
    *uartPin2Address[config->remap.tx] = 0x02u;
    U2RXRbits.U2RXR     = uartPin2Value[config->remap.rx];
    U2MODE              = mode;
    esVTimerInit(&ReadAfterTimeout);
}

static void lldUartClose(
    struct uartHandle * handle) {

    lldUartWriteStop(handle);
    lldUartReadStop(handle);

    while ((U2STA & U2STA_TRMT) == 0u);                                         /* Wait for last write                                      */
    U2MODECLR = U2MODE_ON;
}

static bool lldUartIsReadBuffEmpty(
    struct uartHandle * handle) {

    (void)handle;

    if ((U2STA & U2STA_URXDA) == 0u) {
        return (true);
    } else {
        return (false);
    }
}

static bool lldUartIsWriteBuffFull(
    struct uartHandle * handle) {

    (void)handle;
    
    if ((U2STA & U2STA_UTXBF) != 0u) {
        return (true);
    } else {
        return (false);
    }
}

static uint32_t lldUartRead(
    struct uartHandle * handle) {

    (void)handle;

    return (U2RXREG);
}

static void lldUartWrite(
    struct uartHandle * handle,
    uint32_t            data) {

    (void)handle;
    U2TXREG = data;
}

static void lldUartReadStart(
    struct uartHandle * handle) {

    g_uart2_rx_counter   = 0u;
    g_uart2_rx_cancelled = false;
    uartUpdateRxTrigger(0);

    if (handle->readTimeout != (uint32_t)-1)
    {
        esVTimerCancel(&ReadAfterTimeout);
    }
    U2STACLR = U2STA_OERR | U2STA_PERR;                                         /* Flush old reveived data                                  */
    IFS1CLR  = IFS1_U2RX;
    IEC1SET  = IEC1_U2RX;
}

static void lldUartReadCancel(
    struct uartHandle * handle) {

    (void)handle;

    g_uart2_rx_cancelled = true;
    IFS1SET           = IFS1_U2RX;
}

static void lldUartReadStop(
    struct uartHandle * handle) {

    (void)handle;

    IEC1CLR = IEC1_U2RX;
}

static void lldUartWriteStart(
    struct uartHandle * handle) {

    (void)handle;
    g_uart2_tx_counter = 0u;

    IFS1CLR = IFS1_U2TX;
    IEC1SET = IEC1_U2TX;
}

static void lldUartWriteStop(
    struct uartHandle * handle) {

    (void)handle;

    IEC1CLR = IEC1_U2TX;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*
 * TODO: ISR supports only 8-bit data size
 */
void __ISR(_UART2_VECTOR) lldUART2Handler(void) {

    /*--  RX interrupt  ------------------------------------------------------*/
    if ((IEC1 & IFS1 & INTR_U2RX) != 0u) {
        IFS1CLR         = IFS1_U2RX;

        while ((U2STA & U2STA_URXDA) != 0u) {

            ((uint8_t *)g_uart2_handle->readBuffer)[g_uart2_rx_counter] = U2RXREG;
            g_uart2_rx_counter++;
        }

        if (g_uart2_handle->readTimeout != (uint32_t)-1)
        {
            if (g_uart2_handle->reader != NULL) {
                esVTimerCancelI(&ReadAfterTimeout);
                esVTimerStartI(&ReadAfterTimeout, g_uart2_handle->readTimeout, (esVTimerFn)readAfterCallback, NULL);
            }
        }
        
        if ((U2STA & (U2STA_OERR | U2STA_FERR | U2STA_PERR)) != 0u) {
            enum uartError error;

            if ((U2STA & U2STA_OERR) != 0u) {
                error = UART_ERROR_OVERFLOW;
            } else if ((U2STA & U2STA_FERR) != 0u) {
                error = UART_ERROR_FRAME;
            } else {
                error = UART_ERROR_PARITY;
            }
            U2STACLR = U2STA_OERR;                                              /* Flush the rest in fifo buffer                            */
            IEC1CLR  = IEC1_U2RX;
            g_uart2_handle->state &= ~UART_RX_ACTIVE;
            
            if (g_uart2_handle->reader != NULL) {
                (void)g_uart2_handle->reader(
                    g_uart2_handle,
                    error,
                    g_uart2_handle->readBuffer,
                    g_uart2_rx_counter);
            }
        } else if ((g_uart2_rx_counter >= g_uart2_handle->readSize) || (g_uart2_rx_cancelled == true)) {
            size_t          request;
            enum uartError  error;

            request = 0;
            error   = UART_ERROR_NONE;

            if (g_uart2_rx_cancelled) {
                g_uart2_rx_cancelled = false;
                error                = UART_ERROR_CANCEL;
            }
            
            if (g_uart2_handle->reader != NULL) {
                request = g_uart2_handle->reader(
                    g_uart2_handle,
                    error,
                    g_uart2_handle->readBuffer,
                    g_uart2_rx_counter);
                g_uart2_handle->readSize = request;
                uartUpdateRxTrigger(0);
            }

            if (request == 0) {
                IEC1CLR = IEC1_U2RX;
                g_uart2_handle->state &= ~UART_RX_ACTIVE;
            }
        }
    }
    
    /*-- TX interrupt  -------------------------------------------------------*/
    if ((IEC1 & IFS1 & INTR_U2TX) != 0u) {
        IFS1CLR = IFS1_U2TX;

        while (((U2STA & U2STA_UTXBF) == 0u) &&
            g_uart2_tx_counter < g_uart2_handle->writeSize) {

            U2TXREG = ((uint8_t *)g_uart2_handle->writeBuffer)[g_uart2_tx_counter];
            g_uart2_tx_counter++;
        }
        
        if (g_uart2_tx_counter == g_uart2_handle->writeSize) {
            size_t  request;
            
            if (g_uart2_handle->writer != NULL) {
                

                request = g_uart2_handle->writer(
                    g_uart2_handle,
                    UART_ERROR_NONE,
                    g_uart2_handle->writeBuffer,
                    g_uart2_handle->writeSize);
                g_uart2_handle->writeSize = request;

            }
            if (request == 0u) {
                g_uart2_handle->state &= ~UART_TX_ACTIVE;
                IEC1CLR = IEC1_U2TX;                                        /* Stop further transmission                                */
            }
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of lld_UART2.c
 ******************************************************************************/
