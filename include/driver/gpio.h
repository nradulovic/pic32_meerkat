/* 
 * File:    gpio.h
 * Author:  nenad
 * Details: GPIO
 *
 * Created on February 16, 2014, 8:47 PM
 */

#ifndef GPIO_H
#define	GPIO_H

/*=========================================================  INCLUDE FILES  ==*/

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

struct gpio {
    volatile unsigned int * port;
    volatile unsigned int * tris;
    volatile unsigned int * lat;
    volatile unsigned int * set;
    volatile unsigned int * clr;
    volatile unsigned int * invert;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct gpio GlobalGpioA;
extern const struct gpio GlobalGpioB;
extern const struct gpio GlobalGpioC;

/*===================================================  FUNCTION PROTOTYPES  ==*/

void initGpio(
    void);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of gpio.h
 ******************************************************************************/
#endif /* GPIO_H */
