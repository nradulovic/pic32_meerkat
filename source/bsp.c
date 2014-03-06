/*
 * File:    bsp.c
 * Author:  nenad
 * Details: Board Support Package
 *
 * Created on February 8, 2014, 11:12 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include <peripheral/system.h>
#include "bsp.h"
#include "driver/intr.h"
#include "driver/clock.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/spi.h"
#include "driver/uart.h"
#include "driver/codec.h"
#include "driver/intr.h"
#include "config/config_pins.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void initBsp(
    void) {

    /*--  Initialize CPU peripherals  ----------------------------------------*/
    initClock();
    initIntr();
    initGpio();                                                                 /* Initialize GPIO module                                   */
    initSpi();
    initUart();

    /*--  Initialize board peripherals  --------------------------------------*/
    initCodec();
    initCPump();
    initRadioCharger();
    initBattCharger();

    codecOpen(NULL);
}

/*--  Charge pump control  ---------------------------------------------------*/

void initCPump(
    void) {

    *(CONFIG_CPUMP_EN_PORT)->tris &= ~(0x1u << CONFIG_CPUMP_EN_PIN);
    *(CONFIG_CPUMP_EN_PORT)->clr   =  (0x1u << CONFIG_CPUMP_EN_PIN);            /* Disable 5V generation for now                            */
}

void cpumpEnable(
    void) {

    *(CONFIG_CPUMP_EN_PORT)->set = (0x1u << CONFIG_CPUMP_EN_PIN);
}

void cpumpDisable(
    void) {

    *(CONFIG_CPUMP_EN_PORT)->set = (0x1u << CONFIG_CPUMP_EN_PIN);
}

/*--  Radio charger control  -------------------------------------------------*/

/* NOTE: Radio charger enable pin is active low
 */
void initRadioCharger(
    void) {

    *(CONFIG_RADIO_CHARGER_EN_PORT)->tris &=                           
        ~(0x1u << CONFIG_RADIO_CHARGER_EN_PIN);
    *(CONFIG_RADIO_CHARGER_EN_PORT)->set = (0x1u << CONFIG_RADIO_CHARGER_EN_PIN);
}

void radioChargerEnable(
    void) {

    *(CONFIG_RADIO_CHARGER_EN_PORT)->clr =
        (0x1u << CONFIG_RADIO_CHARGER_EN_PIN);
}

void radioChargerDisable(
    void) {

    *(CONFIG_RADIO_CHARGER_EN_PORT)->set = (0x1u << CONFIG_RADIO_CHARGER_EN_PIN);
}

/*--  Battery charger control  -----------------------------------------------*/

void initBattCharger(
    void) {

    *(CONFIG_BATT_CHARGER_EN_PORT)->tris     |=
        (0x1u << CONFIG_BATT_CHARGER_EN_PIN);
    *(CONFIG_BATT_CHARGER_EN_PORT)->pulldown |=
        (0x1u << CONFIG_BATT_CHARGER_EN_PIN);                                   /* Set the pull-down so we conserve battery                 */
}

bool isBatteryCharging(
    void) {

    bool                ret;

    *(CONFIG_BATT_CHARGER_EN_PORT)->pulldown &=
        ~(0x1u << CONFIG_BATT_CHARGER_EN_PIN);                                  /* Remove pull-down                                         */
    *(CONFIG_BATT_CHARGER_EN_PORT)->pullup   |=
        (0x1u << CONFIG_BATT_CHARGER_EN_PIN);                                   /* Set the pull-up to read the value                        */

    if ((*(CONFIG_BATT_CHARGER_EN_PORT)->port &
        (0x1u << CONFIG_BATT_CHARGER_EN_PIN)) == 0u) {                          /* Read the value                                           */
        ret = true;
    } else {
        ret = false;
    }
    *(CONFIG_BATT_CHARGER_EN_PORT)->pullup &=
        ~(0x1u << CONFIG_BATT_CHARGER_EN_PIN);                                  /* Turn off pull-up so we conserve the battery              */
    *(CONFIG_BATT_CHARGER_EN_PORT)->pulldown |=
        (0x1u << CONFIG_BATT_CHARGER_EN_PIN);                                   /* Set the pull-down so we conserve battery                 */
    
    return (ret);
}

/*--  Analog audio switch control  -------------------------------------------*/

void initAudioSwitch(
    void) {

}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/