/* 
 * File:    conv.h
 * Author:  nenad
 * Details: Converting functions
 *
 * Created on March 16, 2014, 7:11 PM
 */

#ifndef CONV_H
#define	CONV_H

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

uint8_t convToLower(unsigned char character);
uint8_t convAsciiToHex(unsigned char character);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of conv.h
 ******************************************************************************/
#endif /* CONV_H */

