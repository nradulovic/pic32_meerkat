/* 
 * File:   epa_sync.h
 * Author: nenad
 *
 * Created on June 25, 2014, 10:12 PM
 */

#ifndef EPA_SYNC_H
#define	EPA_SYNC_H

#include "eds/epa.h"

#define CONFIG_SYNC_PRIORITY             31
#define CONFIG_SYNC_QUEUE_SIZE           10
#define CONFIG_SYNC_EVENT_BASE           1700

#ifdef	__cplusplus
extern "C" {
#endif

enum syncEventId {
    EVT_SYNC_TICK = CONFIG_SYNC_EVENT_BASE,
    EVT_SYNC_READY,
    EVT_SYNC_REQUEST_TICK,
    EVT_SYNC_DONE
};

extern const struct esEpaDefine SyncEpa;
extern const struct esSmDefine  SyncSm;
extern struct esEpa *   Sync;

#ifdef	__cplusplus
}
#endif

#endif	/* EPA_SYNC_H */

