/*
 * File:    lld_uart1.h
 * Author:  nenad
 * Details: Low Level Driver for UART1
 *
 * Created on February 14, 2014, 9:59 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <xc.h>
#include <sys/attribs.h>

#include "driver/uart.h"
#include "driver/clock.h"
#include "plat/critical.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define IEC1_U1E                        (0x1u << 7)
#define IEC1_U1RX                       (0x1u << 8)
#define IEC1_U1TX                       (0x1u << 9)

#define IFS1_U1E                        (0x1u << 7)
#define IFS1_U1RX                       (0x1u << 8)
#define IFS1_U1TX                       (0x1u << 9)

#define INTR_U1RX                       (0x1u << 8)
#define INTR_U1TX                       (0x1u << 9)

#define U1MODE_STSEL_2_STOP_BITS        (0x1u << 0)
#define U1MODE_PDSEL_9_NO               (0x3u << 1)
#define U1MODE_PDSEL_8_ODD              (0x2u << 1)
#define U1MODE_PDSEL_8_EVEN             (0x1u << 1)
#define U1MODE_EN_CTS_RTS               (0x2u << 8)
#define U1MODE_ON                       (0x1u << 15)

#define U1STA_URXDA                     (0x1u << 0)
#define U1STA_OERR                      (0x1u << 1)
#define U1STA_FERR                      (0x1u << 2)
#define U1STA_PERR                      (0x1u << 3)
#define U1STA_RXISEL_3_4                (0x2u << 6)
#define U1STA_RXISEL_1_2                (0x1u << 6)
#define U1STA_TRMT                      (0x1u << 8)
#define U1STA_UTXBF                     (0x1u << 9)
#define U1STA_TXEN                      (0x1u << 10)
#define U1STA_RXEN                      (0x1u << 12)
#define U1STA_TXISEL_ALL                (0x1u << 14)

#define U1STA_URXISEL_Msk               (0x3u << 6)

#define UART1_PIN_ADDRESS(id, value, address)                                   \
    address,

#define UART1_PIN_VALUE(id, value, address)                                     \
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

static struct uartHandle * GlobalHandle;
static uint32_t         GlobalTxCounter;
static uint32_t         GlobalRxCounter;
static bool             GlobalRxCancelled;

static volatile unsigned int * const uartPinAddress[UART1_LAST_PIN_ID] = {
    UART1_PIN_TABLE(UART1_PIN_ADDRESS)
};

static const unsigned int uartPinValue[UART1_LAST_PIN_ID] = {
    UART1_PIN_TABLE(UART1_PIN_VALUE)
};

/*======================================================  GLOBAL VARIABLES  ==*/

const struct uartId Uart1 = {
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

    U1STACLR = U1STA_URXISEL_Msk;

    if (size >= 6) {
        U1STASET = U1STA_RXISEL_3_4;
    } else if (size >= 4) {
        U1STASET = U1STA_RXISEL_1_2;
    }
}

static void lldUartOpen(
    const struct uartConfig * config,
    struct uartHandle * handle) {

    uint32_t            brg;
    uint32_t            mode;
    uint32_t            sta;

    GlobalHandle        = handle;
    U1MODE              = 0;
    U1STA               = 0;
    IEC1CLR             = IEC1_U1TX | IEC1_U1RX | IEC1_U1E;
    IPC8bits.U1IP       = config->isrPriority;
    IPC8bits.U1IS       = 0;
    sta                 = 0;

    if ((config->flags & UART_TX_ENABLE) != 0u) {
        sta            |= U1STA_TXEN;
    }

    if ((config->flags & UART_RX_ENABLE) != 0u) {
        sta            |= U1STA_RXEN;
    }
    sta                |= U1STA_TXISEL_ALL; /* | U1STA_RXISEL_3_4;*/
    U1STA               = sta;
    brg = clockGetPeripheralClock() / config->speed;
    brg++;
    brg >>= 4;
    brg--;
    brg &= 0xffffu;
    U1BRG               = brg;
    mode                = U1MODE_ON;

    if ((config->flags & UART_HARDWARE_FLOW) != 0u) {
        mode           |= U1MODE_EN_CTS_RTS;
        /*--  Remap CTS & RTS port pins  -------------------------------------*/
        U1CTSRbits.U1CTSR = uartPinValue[config->remap.cts];
        *uartPinAddress[config->remap.rts] = 0x01u;
    }

    if ((config->flags & UART_DATA_BITS_9) != 0u) {
        mode           |= U1MODE_PDSEL_9_NO;
    } else if ((config->flags & UART_PARITY_EVEN) != 0u) {
        mode           |= U1MODE_PDSEL_8_EVEN;
    } else if ((config->flags & UART_PARITY_ODD) != 0u) {
        mode           |= U1MODE_PDSEL_8_ODD;
    }

    if ((config->flags & UART_STOP_BITS_2) != 0u) {
        mode           |= U1MODE_STSEL_2_STOP_BITS;
    }
    /*--  Remap RX & TX port pins  -------------------------------------------*/
    *uartPinAddress[config->remap.tx]  = 0x01u;
    U1RXRbits.U1RXR     = uartPinValue[config->remap.rx];
    U1MODE              = mode;
}

static void lldUartClose(
    struct uartHandle * handle) {

    lldUartWriteStop(handle);
    lldUartReadStop(handle);

    while ((U1STA & U1STA_TRMT) == 0u);                                         /* Wait for last write                                      */
    U1MODECLR = U1MODE_ON;
}

static bool lldUartIsReadBuffEmpty(
    struct uartHandle * handle) {

    (void)handle;

    if ((U1STA & U1STA_URXDA) == 0u) {
        return (true);
    } else {
        return (false);
    }
}

static bool lldUartIsWriteBuffFull(
    struct uartHandle * handle) {

    (void)handle;
    
    if ((U1STA & U1STA_UTXBF) != 0u) {
        return (true);
    } else {
        return (false);
    }
}

static uint32_t lldUartRead(
    struct uartHandle * handle) {

    (void)handle;

    return (U1RXREG);
}

static void lldUartWrite(
    struct uartHandle * handle,
    uint32_t            data) {

    (void)handle;
    U1TXREG = data;
}

static void lldUartReadStart(
    struct uartHandle * handle) {

    GlobalRxCounter   = 0u;
    GlobalRxCancelled = false;
    uartUpdateRxTrigger(handle->readSize);
    U1STACLR = U1STA_OERR;                                                      /* Flush old reveived data                                  */
    IFS1CLR  = IFS1_U1RX;
    IEC1SET  = IEC1_U1RX;
}

static void lldUartReadCancel(
    struct uartHandle * handle) {

    (void)handle;

    GlobalRxCancelled = true;
    IFS1SET           = IFS1_U1RX;
}

static void lldUartReadStop(
    struct uartHandle * handle) {

    (void)handle;

    IEC1CLR = IEC1_U1RX;
}

static void lldUartWriteStart(
    struct uartHandle * handle) {

    (void)handle;
    GlobalTxCounter = 0u;

    IFS1CLR = IFS1_U1TX;
    IEC1SET = IEC1_U1TX;
}

static void lldUartWriteStop(
    struct uartHandle * handle) {

    (void)handle;

    IEC1CLR = IEC1_U1TX;
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

/*
 * TODO: ISR supports only 8-bit data size
 */
void __ISR(_UART1_VECTOR) lldUart1Handler(void) {

    /*--  RX interrupt  ------------------------------------------------------*/
    if ((IEC1 & IFS1 & INTR_U1RX) != 0u) {
        IFS1CLR = IFS1_U1RX;

        while ((U1STA & U1STA_URXDA) != 0u) {

            ((uint8_t *)GlobalHandle->readBuffer)[GlobalRxCounter] = U1RXREG;
            GlobalRxCounter++;
        }
        
        if ((U1STA & (U1STA_OERR | U1STA_FERR | U1STA_PERR)) != 0u) {
            enum uartError error;

            if ((U1STA & U1STA_OERR) != 0u) {
                error = UART_ERROR_OVERFLOW;
            } else if ((U1STA & U1STA_FERR) != 0u) {
                error = UART_ERROR_FRAME;
            } else {
                error = UART_ERROR_PARITY;
            }
            U1STACLR = U1STA_OERR;                                              /* Flush the rest in fifo buffer                            */
            IEC1CLR  = IEC1_U1RX;
            
            if (GlobalHandle->reader != NULL) {
                (void)GlobalHandle->reader(
                    error,
                    GlobalHandle->readBuffer,
                    GlobalRxCounter);
            }
        } else if ((GlobalRxCounter >= GlobalHandle->readSize) || (GlobalRxCancelled == true)) {
            GlobalRxCancelled = false;
            
            if (GlobalHandle->reader != NULL) {
                size_t  request;
                
                request = GlobalHandle->reader(
                    UART_ERROR_NONE,
                    GlobalHandle->readBuffer,
                    GlobalRxCounter);
                GlobalHandle->readSize += request;
                uartUpdateRxTrigger(request);

                if (request == 0u) {
                    IEC1CLR = IEC1_U1RX;
                }
            } else {
                IEC1CLR = IEC1_U1RX;
            }
        }
    }
    
    /*-- TX interrupt  -------------------------------------------------------*/
    if ((IEC1 & IFS1 & INTR_U1TX) != 0u) {
        IFS1CLR = IFS1_U1TX;

        while (((U1STA & U1STA_UTXBF) == 0u) &&
            GlobalTxCounter < GlobalHandle->writeSize) {

            U1TXREG = ((uint8_t *)GlobalHandle->writeBuffer)[GlobalTxCounter];
            GlobalTxCounter++;
        }
        
        if (GlobalTxCounter == GlobalHandle->writeSize) {
            if (GlobalHandle->writer != NULL) {
                size_t  request;

                request = GlobalHandle->writer(
                    UART_ERROR_NONE,
                    GlobalHandle->writeBuffer,
                    GlobalHandle->writeSize);
                GlobalHandle->writeSize += request;

                if (request == 0u) {
                    IEC1CLR = IEC1_U1TX;                                        /* Stop further transmission                                */
                }
            } else {
                IEC1CLR = IEC1_U1TX;                                            /* Stop further transmission                                */
            }
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of lld_uart1.c
 ******************************************************************************/
