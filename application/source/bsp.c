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
#include "driver/adc.h"
#include "driver/systick.h"
#include "config/config_pins.h"
#include "app_pdetector.h"

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
    initClockDriver();
    initIntrDriver();
    initGpioDriver();                                                           /* Initialize GPIO module                                   */
    initSpiDriver();
    initUartDriver();
    initAdcDriver();
    initSysTickDriver();

    /*--  Initialize board peripherals  --------------------------------------*/
    initCodecDriver();
    initCPumpDriver();
    initRadioChargerDriver();
    initBattChargerDriver();
    initAudioSwitch();
    initNotifyLed();
    initPdetectorModule();
}

/*--  Charge pump control  ---------------------------------------------------*/

void initCPumpDriver(
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
void initRadioChargerDriver(
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

void initBattChargerDriver(
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

    *(CONFIG_AUDIO_MUX_CB1_PORT)->tris &= ~(0x1u << CONFIG_AUDIO_MUX_CB1_PIN);
    *(CONFIG_AUDIO_MUX_CB2_PORT)->tris &= ~(0x1u << CONFIG_AUDIO_MUX_CB2_PIN);
    *(CONFIG_AUDIO_MUX_CB1_PORT)->clr   =  (0x1u << CONFIG_AUDIO_MUX_CB1_PIN);
    *(CONFIG_AUDIO_MUX_CB2_PORT)->clr   =  (0x1u << CONFIG_AUDIO_MUX_CB2_PIN);
}

void audioSwitchToRadio(
    void) {

    *(CONFIG_AUDIO_MUX_CB1_PORT)->clr = (0x1u << CONFIG_AUDIO_MUX_CB1_PIN);
    *(CONFIG_AUDIO_MUX_CB2_PORT)->set = (0x1u << CONFIG_AUDIO_MUX_CB2_PIN);
}

void audioSwitchToCodec(
    void) {

    *(CONFIG_AUDIO_MUX_CB1_PORT)->set   =  (0x1u << CONFIG_AUDIO_MUX_CB1_PIN);
    *(CONFIG_AUDIO_MUX_CB2_PORT)->clr   =  (0x1u << CONFIG_AUDIO_MUX_CB2_PIN);
}

/*--  Notification LED  ------------------------------------------------------*/

void initNotifyLed(
    void) {

    *(CONFIG_NOTIFY_LED_A1_PORT)->tris &= ~(0x1u << CONFIG_NOTIFY_LED_A1_PIN);
    *(CONFIG_NOTIFY_LED_A2_PORT)->tris &= ~(0x1u << CONFIG_NOTIFY_LED_A2_PIN);
    *(CONFIG_NOTIFY_LED_A1_PORT)->clr   =  (0x1u << CONFIG_NOTIFY_LED_A1_PIN);
    *(CONFIG_NOTIFY_LED_A2_PORT)->clr   =  (0x1u << CONFIG_NOTIFY_LED_A2_PIN);
}

void notifyLedRed(
    void) {
    *(CONFIG_NOTIFY_LED_A1_PORT)->set = (0x1u << CONFIG_NOTIFY_LED_A1_PIN);
    *(CONFIG_NOTIFY_LED_A2_PORT)->clr = (0x1u << CONFIG_NOTIFY_LED_A2_PIN);
}

void notifyLedGreen(
    void) {
    *(CONFIG_NOTIFY_LED_A1_PORT)->clr = (0x1u << CONFIG_NOTIFY_LED_A1_PIN);
    *(CONFIG_NOTIFY_LED_A2_PORT)->set = (0x1u << CONFIG_NOTIFY_LED_A2_PIN);
}

void notifyLedOff(
    void) {
    *(CONFIG_NOTIFY_LED_A1_PORT)->clr = (0x1u << CONFIG_NOTIFY_LED_A1_PIN);
    *(CONFIG_NOTIFY_LED_A2_PORT)->clr = (0x1u << CONFIG_NOTIFY_LED_A2_PIN);
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of bsp.c
 ******************************************************************************/