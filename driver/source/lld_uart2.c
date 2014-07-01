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
static struct uartHandle * Global2Handle;
static uint32_t         Global2TxCounter;
static uint32_t         Global2RxCounter;
static bool             Global2RxCancelled;

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
    uartReadCancel(Global2Handle);
}

static void lldUartOpen(
    const struct uartConfig * config,
    struct uartHandle * handle) {

    uint32_t            brg;
    uint32_t            mode;
    uint32_t            sta;

    Global2Handle       = handle;
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

    Global2RxCounter   = 0u;
    Global2RxCancelled = false;
    uartUpdateRxTrigger(0);

    if (handle->readTimeout != (uint32_t)-1)
    {
        esVTimerCancel(&ReadAfterTimeout);
    }
    U2STACLR = U2STA_OERR;                                                      /* Flush old reveived data                                  */
    IFS1CLR  = IFS1_U2RX;
    IEC1SET  = IEC1_U2RX;
}

static void lldUartReadCancel(
    struct uartHandle * handle) {

    (void)handle;

    Global2RxCancelled = true;
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
    Global2TxCounter = 0u;

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

            ((uint8_t *)Global2Handle->readBuffer)[Global2RxCounter] = U2RXREG;
            Global2RxCounter++;
        }

        if (Global2Handle->readTimeout != (uint32_t)-1)
        {
            if (Global2Handle->reader != NULL) {
                esVTimerCancelI(&ReadAfterTimeout);
                esVTimerStartI(&ReadAfterTimeout, Global2Handle->readTimeout, (esVTimerFn)readAfterCallback, NULL);
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
            
            if (Global2Handle->reader != NULL) {
                (void)Global2Handle->reader(
                    Global2Handle,
                    error,
                    Global2Handle->readBuffer,
                    Global2RxCounter);
            }
        } else if ((Global2RxCounter >= Global2Handle->readSize) || (Global2RxCancelled == true)) {
            size_t          request;
            enum uartError  error;

            request = 0;
            error   = UART_ERROR_NONE;

            if (Global2RxCancelled) {
                Global2RxCancelled = false;
                error              = UART_ERROR_CANCEL;
            }
            
            if (Global2Handle->reader != NULL) {
                request = Global2Handle->reader(
                    Global2Handle,
                    error,
                    Global2Handle->readBuffer,
                    Global2RxCounter);
                Global2Handle->readSize += request;
                uartUpdateRxTrigger(0);
            }

            if (request == 0u) {
                IEC1CLR = IEC1_U2RX;
            }
        }
    }
    
    /*-- TX interrupt  -------------------------------------------------------*/
    if ((IEC1 & IFS1 & INTR_U2TX) != 0u) {
        IFS1CLR = IFS1_U2TX;

        while (((U2STA & U2STA_UTXBF) == 0u) &&
            Global2TxCounter < Global2Handle->writeSize) {

            U2TXREG = ((uint8_t *)Global2Handle->writeBuffer)[Global2TxCounter];
            Global2TxCounter++;
        }
        
        if (Global2TxCounter == Global2Handle->writeSize) {
            if (Global2Handle->writer != NULL) {
                size_t  request;

                request = Global2Handle->writer(
                    Global2Handle,
                    UART_ERROR_NONE,
                    Global2Handle->writeBuffer,
                    Global2Handle->writeSize);
                Global2Handle->writeSize += request;

                if (request == 0u) {
                    Global2Handle->state &= ~UART_TX_ACTIVE;
                    IEC1CLR = IEC1_U2TX;                                        /* Stop further transmission                                */
                }
            } else {
                Global2Handle->state &= ~UART_TX_ACTIVE;
                IEC1CLR = IEC1_U2TX;                                            /* Stop further transmission                                */
            }
        }
    }
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of lld_UART2.c
 ******************************************************************************/
