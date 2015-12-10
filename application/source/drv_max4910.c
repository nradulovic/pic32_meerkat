
/*=========================================================  INCLUDE FILES  ==*/

#include "neon_driver.h"
#include "max4910.h"
#include "hw_config.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define PIN_CB1							(NGPIO_ID(CONFIG_MAX4910_PORT_CB1, CONFIG_MAX4910_PIN_CB1))
#define PIN_CB2							(NGPIO_ID(CONFIG_MAX4910_PORT_CB2, CONFIG_MAX4910_PIN_CB2))
#define PIN_EN							(NGPIO_ID(CONFIG_MAX4910_PORT_EN, CONFIG_MAX4910_PIN_EN))

/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/


void max4910_init(void)
{
	ngpio_init(PIN_CB1, NGPIO_OUTPUT_LOW);
	ngpio_init(PIN_CB2, NGPIO_OUTPUT_HIGH);
	ngpio_init(PIN_EN, NGPIO_OUTPUT_LOW);
}



void max4910_activate(void)
{
	ngpio_
}



void max4910_deactivate(void);
void max4910_set_spkr_radio(void)
{
	ngpio_clear(PIN_CB1);
}



void max4910_set_spkr_codec(void)
{
	ngpio_set(PIN_CB1);
}



void max4910_set_mic_radio(void)
{
	ngpio_set(PIN_CB2);
}



void max4910_set_mic_codec(void)
{
	ngpio_clear(PIN_CB2);
}


/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of drv_max4910.c
 ******************************************************************************/
