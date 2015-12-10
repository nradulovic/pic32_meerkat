

#ifndef DRV_MAX4910_H_
#define DRV_MAX4910_H_

/*=========================================================  INCLUDE FILES  ==*/
/*===============================================================  MACRO's  ==*/
/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif
/*============================================================  DATA TYPES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void max4910_init(void);
void max4910_activate(void);
void max4910_deactivate(void);
void max4910_set_spkr_radio(void);
void max4910_set_spkr_codec(void);
void max4910_set_mic_radio(void);
void max4910_set_mix_codec(void);

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_bt.h
 ******************************************************************************/
#endif /* DRV_MAX4910_H_ */
