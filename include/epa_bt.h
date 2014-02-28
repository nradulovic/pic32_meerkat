/* 
 * File:    epa_bt.h
 * Author:  nenad
 * Details: Event Processing Agent for Bluetooth driver
 *
 * Created on February 22, 2014, 7:20 PM
 */

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
#define CONFIG_BT_UART_TIMEOUT_MS       100u

#define BT_DRV_CMD_TABLE(entry)                                                 \
    entry(BT_DRV_SET_AUTH_NONE,         "SA,0",     0)                          \
    entry(BT_DRV_SET_AUDIO_ROUTING,     "S|,",      2)                          \
    entry(BT_DRV_SET_NAME,              "S-,",      8)                          \
    entry(BT_DRV_SET_AUTH,              "SA,",      1)                          \
    entry(BT_DRV_SET_COD,               "SC,",      6)                          \
    entry(BT_DRV_SET_DISCOVERY_MASK,    "SD,",      2)                          \
    entry(BT_DRV_SET_FACTORY,           "SF,1",     0)                          \
    entry(BT_DRV_SET_CONNECTION_MASK,   "SK,",      2)                          \
    entry(BT_DRV_SET_DEVICE_NAME,       "SN,",      8)                          \
    entry(BT_DRV_SET_SECURITY_PIN,      "SP,",      8)                          \
    entry(BT_DRV_SET_DISCOVERABLE,      "@,1",      0)                          \
    entry(BT_DRV_SET_NOT_DISCOVERABLE,  "@,0",      0)                          \
    entry(BT_DRV_GET_DETAILS,           "D",        0)                          \
    entry(BT_DRV_GET_AUTH,              "GA",       0)                          \
    entry(BT_DRV_GET_SECURITY_PIN,      "GP",       0)                          \
    entry(BT_DRV_TOGGLE_ECHO,           "+",        0)                          \
    entry(BT_DRV_VOLUME_UP,             "AV+",      0)                          \
    entry(BT_DRV_VOLUME_DOWN,           "AV-",      0)                          \
    entry(BT_DRV_RECONNECT,             "B",        0)                          \
    entry(BT_DRV_DISCONNECT,            "K,",       2)                          \
    entry(BT_DRV_QUERY,                 "Q",        0)                          \
    entry(BT_DRV_REBOOT,                "R,1",      0)

#define BT_DRV_CMD_EXPAND_ID(id, cmd, args)                                     \
    id,


#define BT_DRV_AUDIO_ROUTE_ANALOG       "00"
#define BT_DRV_AUDIO_ROUTE_I2S          "01"
#define BT_DRV_AUDIO_ROUTE_SPDIF        "02"

#define BT_DRV_AUTH_NONE                "0"
#define BT_DRV_AUTH_SSP_KEYBOARD        "1"
#define BT_DRV_AUTH_SSP_JUST_WORKS      "2"
#define BT_DRV_AUTH_PIN                 "4"

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
    EVT_BT_DRV_REQ      = CONFIG_BT_DRV_EVENT_BASE,
    EVT_BT_DRV_REPLY,
    EVT_BT_DRV_STATUS
};

enum BtStatus {
    BT_DRV_ERR_NONE,
    BT_DRV_ERR_TIMEOUT,
    BT_DRV_ERR_FAILURE,
    BT_DRV_ERR_COMM
};

struct BtStatusEvent {
    struct esEvent      header;
    enum BtStatus       status;
};

struct BtEvent {
    struct esEvent      header;
    enum BtCommandId    cmd;
    char *              arg;
    size_t              argSize;
};

struct BtCmd {
    char *              cmd;
    size_t              size;
};

/*======================================================  GLOBAL VARIABLES  ==*/

extern const struct esEpaDefine BtDrvEpa;
extern const struct esSmDefine  BtDrvSm;
extern struct esEpa *   BtDrv;

extern const struct BtCmd BtCmd[];

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

