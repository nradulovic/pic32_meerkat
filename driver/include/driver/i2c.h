/* 
 * File:    i2c.h
 * Author:  nenad
 * Details: Inter-Integrated Circuit (I2C)
 *
 * Created on February 14, 2014, 9:59 PM
 */

#ifndef I2C_H_
#define	I2C_H_

/*=========================================================  INCLUDE FILES  ==*/

/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum i2cError {
    I2C_ERROR_NONE,
    I2C_ERROR_COLISION
};

struct i2cId;

struct i2cConfig {
    const struct i2cId * id;
};

struct i2cHandle {
    const struct i2cId * id;
    const struct i2cConfig * config;
};

struct i2cId {
    enum i2cError    (* open)(struct i2cHandle *);
    enum i2cError    (* close)(struct i2cHandle *);
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

enum i2cError i2cOpen(
    struct i2cHandle *  handle,
    const struct i2cConfig * config);

enum i2cError i2cClose(
    struct i2cHandle *  handle);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of i2c.h
 ******************************************************************************/
#endif /* I2C_H_ */
