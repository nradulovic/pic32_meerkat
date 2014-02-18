/* 
 * File:    clock.h
 * Author:  nenad
 * Details: Clock management
 *
 * Created on February 17, 2014, 1:18 AM
 */

#ifndef CLOCK_H
#define	CLOCK_H

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

void initClock(
    void);

uint32_t clockGetSystemClock(
    void);

uint32_t clockGetPeripheralClock (
    void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of clock.h
 ******************************************************************************/
#endif /* CLOCK_H */

