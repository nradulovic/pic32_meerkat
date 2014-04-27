/* 
 * File:    lld_UART2.h
 * Author:  nenad
 * Details: Low Level Driver for UART2
 *
 * Created on April 26, 2014, 12:48 AM
 */

#ifndef LLD_UART2_H_
#define	LLD_UART2_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/uart.h"

/*===============================================================  MACRO's  ==*/

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
#define UART2_PIN_TABLE(entry)                                                  \
    entry(UART2_TX_C4,              0,              &RPC4R)                     \
    entry(UART2_RX_A9,              0x07,           NULL)                       \
    entry(UART2_CTS_A4,             0x02,           NULL)                       \
    entry(UART2_RTS_C5,             0,              &RPC5R)
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 499)))
#elif (((__PIC32_FEATURE_SET__ >= 500) && (__PIC32_FEATURE_SET__ <= 799)))
#endif

#define UART2_PIN_ID(id, value, address)                                        \
    id,

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum UART2PinId {
    UART2_PIN_TABLE(UART2_PIN_ID)
    UART2_LAST_PIN_ID
};

/*======================================================  GLOBAL VARIABLES  ==*/

/**@brief       UART2 identifier
 */
extern const struct uartId Uart2;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spis.h
 ******************************************************************************/
#endif /* LLD_UART2_H_ */
