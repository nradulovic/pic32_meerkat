/* 
 * File:    codec.h
 * Author:  nenad
 * Details: Audio codec driver
 *
 * Created on February 15, 2014, 11:11 PM
 */

#ifndef CODEC_H_
#define	CODEC_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "plat/compiler.h"
#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum codecError {
    CODEC_ERROR_NONE
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initCodec(
    void);

enum codecError codecOpen(
    struct spiHandle *  spi);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of spi.h
 ******************************************************************************/
#endif /* CODEC_H_ */
