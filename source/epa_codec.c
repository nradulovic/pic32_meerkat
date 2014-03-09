/*
 * File:    epa_codec.c
 * Author:  nenad
 * Details: Codec Event Processing Agent
 *
 * Created on March 8, 2014, 10:26 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "events.h"
#include "driver/codec.h"
#include "arch/intr.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CODEC_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \
    entry(stateIdle,                TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum codecStateId {
    ES_STATE_ID_INIT(CODEC_TABLE)
};

struct wspace {
    struct codecHandle codec;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, esEvent *);
static esAction stateIdle           (struct wspace *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static void initCodec(struct wspace *);

/*=======================================================  LOCAL VARIABLES  ==*/

static const ES_MODULE_INFO_CREATE("Codec", "Audio codec manager", "Nenad Radulovic");

static const esSmTable CodecTable[] = ES_STATE_TABLE_INIT(CODEC_TABLE);

/*======================================================  GLOBAL VARIABLES  ==*/

const struct esEpaDefine CodecEpa = ES_EPA_DEFINE(
    "Codec",
    CONFIG_CODEC_PRIORITY,
    CONFIG_CODEC_QUEUE_SIZE);

const struct esSmDefine CodecSm = ES_SM_DEFINE(
    CodecTable,
    0,
    stateInit);

struct esEpa *          Codec;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_INIT : {
            initCodec(wspace);

            return (ES_STATE_TRANSITION(stateIdle));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateIdle(struct wspace * wspace, esEvent * event) {
    (void)wspace;

    switch (event->id) {
        case EVT_BT_NOTIFY_READY : {

            return (ES_STATE_HANDLED());
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

/*--  Support functions  -----------------------------------------------------*/

static void initCodec(
    struct wspace *     wspace) {

    struct spiConfig    spiConfig;
    struct codecConfig  codecConfig;
    uint32_t            value;
    
    spiConfig.id        = &SpiSoft;
    spiConfig.flags     = SPI_MASTER_MODE | SPI_MASTER_SS_ACTIVE_LOW |
                          SPI_SLAVE_MODE  | SPI_CLOCK_POLARITY_IDLE_LOW |
                          SPI_CLOCK_PHASE_FIRST_EDGE | SPI_DATA_16;
    spiConfig.isrPrio   = ES_INTR_DEFAULT_ISR_PRIO;
    spiConfig.remap.sdi = SPIS_SDI_C6;
    spiConfig.remap.sdo = SPIS_SDO_B8;
    spiConfig.remap.sck = SPIS_SCK_B9;
    spiConfig.remap.ss  = SPIS_SS_B10;
    spiConfig.speed     = 1000000ul;
    codecConfig.spi     = &spiConfig;
    codecOpen(&wspace->codec, &codecConfig);

    /*--  I2S, input, high-pass, Fsaaple settings  ---------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_1,
        CODEC_AUDIO_CTRL_1_ADCHPF_DISABLED | CODEC_AUDIO_CTRL_1_ADCIN_SINGLE_MIC |
        CODEC_AUDIO_CTRL_1_WLEN_24BIT      | CODEC_AUDIO_CTRL_1_DATFM_I2S        |
        CODEC_AUDIO_CTRL_1_DACFS_DIV1      | CODEC_AUDIO_CTRL_1_ADCFS_DIV1);

    /*--  ADC settings  ------------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_ADC_GAIN,
        CODEC_ADC_GAIN_ADMUT_NOT_MUTED | CODEC_ADC_GAIN_ADPGA_0     |
        CODEC_ADC_GAIN_AGCTG_55        | CODEC_ADC_GAIN_AGCTC_8_100 |
        CODEC_ADC_GAIN_AGCEN_OFF);

    /*--  DAC settings  ------------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_DAC_GAIN,
        CODEC_DAC_GAIN_DALMU_NOT_MUTED | CODEC_DAC_GAIN_DALVL_0 |
        CODEC_DAC_GAIN_DARMU_NOT_MUTED | CODEC_DAC_GAIN_DARVL_0);

    /*--  Sidetone settings  -------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_SIDETONE,
        CODEC_SIDETONE_ASTMU_MUTED | CODEC_SIDETONE_ASTG_M345 |
        CODEC_SIDETONE_DSTMU_MUTED | CODEC_SIDETONE_DSTG_0);

    /*--  Soft-stepping, keyclick settings  ----------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_2,
        CODEC_AUDIO_CTRL_2_KCLEN_OFF     | CODEC_AUDIO_CTRL_2_KCLAC_LOW  |
        CODEC_AUDIO_CTRL_2_APGASS_SINGLE | CODEC_AUDIO_CTRL_2_KCLFRQ_625 |
        CODEC_AUDIO_CTRL_2_KCLLN_2       | CODEC_AUDIO_CTRL_2_DASTC_SINGLE);

    /*--  Power control  -----------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_PWDNC_ON         | CODEC_POWER_CTRL_ASTPWD_OFF |
        CODEC_POWER_CTRL_DAODRC_LOW_POWER | CODEC_POWER_CTRL_DAPWDN_ON  )
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_codec.c
 ******************************************************************************/
