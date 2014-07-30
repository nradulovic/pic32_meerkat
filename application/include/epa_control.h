/* 
 * File:   epa_sync.h
 * Author: nenad
 *
 * Created on June 25, 2014, 10:12 PM
 */

#ifndef EPA_CONTROL_H
#define	EPA_CONTROL_H

#include "eds/epa.h"

#define CONFIG_CONTROL_PRIORITY          1
#define CONFIG_CONTROL_QUEUE_SIZE        10
#define CONFIG_CONTROL_EVENT_BASE        2000

#ifdef	__cplusplus
extern "C" {
#endif

enum controlEventId {
    EVT_CONTROL_TICK = CONFIG_CONTROL_EVENT_BASE
};

extern const struct esEpaDefine ControlEpa;
extern const struct esSmDefine  ControlSm;
extern struct esEpa *           Control;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_CONTROL_H */

