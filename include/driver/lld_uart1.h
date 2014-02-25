/* 
 * File:    lld_uart1.h
 * Author:  nenad
 * Details: Low Level Driver for UART1
 *
 * Created on February 13, 2014, 12:48 AM
 */

#ifndef LLD_UART1_H_
#define	LLD_UART1_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/uart.h"

/*===============================================================  MACRO's  ==*/

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
#define UART1_PIN_TABLE(entry)                                                  \
    entry(UART1_TX_B15,             0,              &RPB15R)                    \
    entry(UART1_RX_B13,             0x03,           NULL)                       \
    entry(UART1_CTS_A1,             0x00,           NULL)                       \
    entry(UART1_RTS_A3,             0x01,           NULL)                       
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499)))
#elif (((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799)))
#endif

#define UART1_PIN_ID(id, value, address)                                        \
    id,

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum uart1PinId {
    UART1_PIN_TABLE(UART1_PIN_ID)
    UART1_LAST_PIN_ID
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       UART1 identifier
 */
extern const struct uartId Uart1;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spis.h
 ******************************************************************************/
#endif /* LLD_UART1_H_ */
