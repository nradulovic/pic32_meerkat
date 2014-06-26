/* 
 * File:   epa_sync.h
 * Author: nenad
 *
 * Created on June 25, 2014, 10:12 PM
 */

#ifndef EPA_SYNC_H
#define	EPA_SYNC_H

#include "eds/epa.h"

#define CONFIG_SYNC_PRIORITY             27
#define CONFIG_SYNC_QUEUE_SIZE           10
#define CONFIG_SYNC_EVENT_BASE           1700

#ifdef	__cplusplus
extern "C" {
#endif

extern const struct esEpaDefine SyncEpa;
extern const struct esSmDefine  SyncSm;
extern struct esEpa *   Sync;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_SYNC_H */

