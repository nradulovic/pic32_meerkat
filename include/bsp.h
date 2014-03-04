/* 
 * File:    bsp.h
 * Author:  nenad
 * Details: Board Support Package
 *
 * Created on February 10, 2014, 7:11 PM
 */

#ifndef BSP_H_
#define	BSP_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdbool.h>

/*===============================================================  MACRO's  ==*/

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

/**@brief       Initialize Board Support Package
 * @details     This function will initialize basic system clocks
 */
void initBsp(
    void);

void initCPump(
    void);
void cpumpEnable(
    void);
void cpumpDisable(
    void);

void initRadioCharger(
    void);
void radioChargerEnable(
    void);
void radioChargerDisable(
    void);

void initBattCharger(
    void);
bool isBatteryCharging(
    void);

void initAudioSwitch(
    void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of bsp.h
 ******************************************************************************/
#endif /* BSP_H_ */

