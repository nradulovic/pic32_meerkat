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
#include "vtimer/vtimer.h"

/*=========================================================  LOCAL MACRO's  ==*/

#define CODEC_TABLE(entry)                                                      \
    entry(stateInit,                TOP)                                        \
    entry(stateReset,               TOP)                                        \
    entry(stateIdle,                TOP)

/*======================================================  LOCAL DATA TYPES  ==*/

enum codecStateId {
    ES_STATE_ID_INIT(CODEC_TABLE)
};

enum codecLocalId {
    EVT_TIMEOUT_ = ES_EVENT_LOCAL_ID
};

struct wspace {
    struct codecHandle codec;
    struct esVTimer    timeout;
};

/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

static esAction stateInit           (struct wspace *, esEvent *);
static esAction stateReset          (struct wspace *, esEvent *);
static esAction stateIdle           (struct wspace *, esEvent *);

/*--  Support functions  -----------------------------------------------------*/

static void initCodec(struct wspace *);
static void codecTimeout(void *);

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
    sizeof(struct wspace),
    stateInit);

struct esEpa *          Codec;

/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

static esAction stateInit(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_INIT : {
            esVTimerInit(&wspace->timeout);

            return (ES_STATE_TRANSITION(stateReset));
        }
        default : {

            return (ES_STATE_IGNORED());
        }
    }
}

static esAction stateReset(struct wspace * wspace, esEvent * event) {

    (void)wspace;

    switch (event->id) {
        case ES_ENTRY : {
            codecResetEnable();
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(100),
                codecTimeout,
                NULL);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            codecResetDisable();
            codecPowerUp();
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
        case ES_ENTRY : {
#if 0
            esEvent *   notify;

            ES_ENSURE(esEventCreate(
                sizeof(esEvent),
                EVT_CODEC_NOTIFY_READY,
                &notify));
            ES_ENSURE(esEpaSendEvent(notify))
#endif
            return (ES_STATE_HANDLED());
        }
        case EVT_CODEC_ENABLE_AUDIO : {
            //codecAudioEnable();

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

    spiConfig.id        = &SpiSoft;
    spiConfig.flags     = SPI_MASTER_MODE | SPI_MASTER_SS_ACTIVE_LOW |
                          SPI_SLAVE_MODE  | SPI_CLOCK_POLARITY_IDLE_LOW |
                          SPI_CLOCK_PHASE_FIRST_EDGE | SPI_DATA_16;
    spiConfig.isrPrio   = ES_INTR_DEFAULT_ISR_PRIO;
    spiConfig.remap.sdi = CONFIG_CODEC_SDI_PIN;
    spiConfig.remap.sdo = CONFIG_CODEC_SDO_PIN;
    spiConfig.remap.sck = CONFIG_CODEC_SCK_PIN;
    spiConfig.remap.ss  = CONFIG_CODEC_SS_PIN;
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

    /*--  Clock settings  ----------------------------------------------------*/
    /*
     * MCLK = 12MHz, Fsref = 48kHz, Q = N/A, P = 1, K = 8192, J = 8, D = 1920
     */
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_PLL_1,
        CODEC_PLL_1_PLLSEL_ON   | CODEC_PLL_1_QVAL_PAR(0x0) |
        CODEC_PLL_1_PVAL_PAR(0x1) | CODEC_PLL_1_JVAL_PAR(0x8));
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_PLL_2,
        CODEC_PLL_2_DVAL_PAR(1920));

    /*--  Power control  -----------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_PWDNC_ON         | CODEC_POWER_CTRL_ASTPWD_OFF |
        CODEC_POWER_CTRL_DAODRC_LOW_POWER | CODEC_POWER_CTRL_DAPWDN_ON  |
        CODEC_POWER_CTRL_ADPWDN_ON        | CODEC_POWER_CTRL_VGPWDN_ON  |
        CODEC_POWER_CTRL_ADWSF_POWERDOWN  | CODEC_POWER_CTRL_VBIAS_25   |
        CODEC_POWER_CTRL_EFFCTL_OFF       | CODEC_POWER_CTRL_DEEMPF_OFF);
}

void codecTimeout(
    void* arg) {

    esEvent *           timeout;

    (void)arg;
    esEventCreate(sizeof(esEvent), EVT_TIMEOUT_, &timeout);
    esEpaSendEvent(Codec, timeout);
}

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/
/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of epa_codec.c
 ******************************************************************************/
