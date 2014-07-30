/* 
 * File:    epa_codec.h
 * Author:  nenad
 * Details: Codec Event Processing Agent
 *
 * Created on March 8, 2014, 10:09 AM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_CODEC_H
#define EPA_CODEC_H

/*=========================================================  INCLUDE FILES  ==*/

#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_CODEC_PRIORITY           2
#define CONFIG_CODEC_QUEUE_SIZE         10
#define CONFIG_CODEC_EVENT_BASE         1200

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum codecEvents {
    EVT_CODEC_NOTIFY_READY = CONFIG_CODEC_EVENT_BASE,
    EVT_CODEC_ENABLE_AUDIO,
    EVT_CODEC_DISABLE_AUDIO
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine CodecEpa;
extern const struct esSmDefine  CodecSm;
extern struct esEpa *   Codec;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_codec.h
 ******************************************************************************/
#endif /* EPA_CODEC_H */


