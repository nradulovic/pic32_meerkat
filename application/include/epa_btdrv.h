/* 
 * File:    epa_bt.h
 * Author:  nenad
 * Details: Event Processing Agent for Bluetooth driver
 *
 * Created on February 22, 2014, 7:20 PM
 */
/*----------------------------------------------------------------------------*
 * NOTE: This file must be included only through events.h file
 *----------------------------------------------------------------------------*/

#ifndef EPA_BT_H
#define EPA_BT_H

/*=========================================================  INCLUDE FILES  ==*/

#include "config/config_pins.h"
#include "eds/epa.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_BT_DRV_NAME              "Bluetooth driver"
#define CONFIG_BT_DRV_PRIORITY          30
#define CONFIG_BT_DRV_QUEUE_SIZE        10
#define CONFIG_BT_DRV_EVENT_BASE        1000
#define CONFIG_BT_UART_SPEED            115200
#define CONFIG_BT_UART_TIMEOUT_MS       400u

#define BT_DRV_CMD_TABLE(entry)                                                 \
    entry(BT_SET_AUTH_NONE,             "SA,0",     0)                          \
    entry(BT_SET_MODULE_NAME,           "S-,",      8)                          \
    entry(BT_SET_DISCOVERY_MASK,        "SD,",      2)                          \
    entry(BT_SET_CONNECTION_MASK,       "SK,",      2)                          \
    entry(BT_SET_AUDIO_I2S,             "S|,01",    0)                          \
    entry(BT_SET_AUDIO_ANALOG,          "S|,00",    0)                          \
    entry(BT_SET_MUTE_OFF,              "M,0",      0)                          \
    entry(BT_SET_AVRCP,                 "S%,01",    0)                          \
    entry(BT_SET_DEVICE_NAME,           "SN,",      8)                          \
    entry(BT_SET_DISCOVERABLE,          "@,1",      0)                          \
    entry(BT_QUERY,                     "Q",        0)                          \
    entry(BT_CALL,                      "A,789",        0)                          \
    entry(BT_REBOOT,                    "R,1",      0)

#define BT_DRV_CMD_EXPAND_ID(id, cmd, args)                                     \
    id,

#define BT_DRV_PROFILE_iAP_Msk          (0x1u << 0)
#define BT_DRV_PROFILE_SPP_Msk          (0x1u << 1)
#define BT_DRV_PROFILE_A2DP_Msk         (0x1u << 2)
#define BT_DRV_PROFILE_HFP_Msk          (0x1u << 3)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum BtCommandId {
    BT_DRV_CMD_TABLE(BT_DRV_CMD_EXPAND_ID)
    LAST_BT_CMD_ID
};

enum BtEvents {
    EVT_BT_CMD_MODE_ENTER = CONFIG_BT_DRV_EVENT_BASE,
    EVT_BT_REQ,
    EVT_BT_REPLY,
    EVT_BT_STATUS,
    EVT_BT_CMD_MODE_EXIT,
    EVT_BT_RESTART,
    EVT_BT_NOTIFY_READY,
    EVT_BT_SEND_DATA
};

enum BtStatus {
    BT_ERR_NONE,
    BT_ERR_TIMEOUT,
    BT_ERR_FAILURE,
    BT_ERR_COMM
};

struct BtSendEvent {
    struct esEvent      header;
    char *              arg;
    size_t              argSize;
};

struct BtReqEvent {
    struct esEvent      header;
    enum BtCommandId    cmd;
    char *              arg;
    size_t              argSize;
};

struct BtReplyEvent {
    struct esEvent      header;
    enum BtStatus       status;
    char *              arg;
    size_t              argSize;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine BtDrvEpa;
extern const struct esSmDefine  BtDrvSm;
extern struct esEpa *   BtDrv;

/*===================================================  FUNCTION PROTOTYPES  ==*/

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of epa_bt.h
 ******************************************************************************/
#endif /* EPA_BT_H */

