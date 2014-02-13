/* 
 * File:   lld_spi1.h
 * Author: nenad
 *
 * Created on February 11, 2014, 10:28 PM
 */

#ifndef LLD_SPI1_H_
#define	LLD_SPI1_H_

/*=========================================================  INCLUDE FILES  ==*/

#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#define REMAP_SPI1_SDI_RPA1             0x00
#define REMAP_SPI1_SDI_RPB5             0x01

#define REMAP_SPI1_SDO_RPA2             &RPA2R

#define REMAP_SPI1_SS_RPA0              &RPA0R

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct spiId GlobalSpi1;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of lld_spi1.h
 ******************************************************************************/
#endif /* LLD_SPI1_H_ */
