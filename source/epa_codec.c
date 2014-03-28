/*
 * File:    epa_codec.c
 * Author:  nenad
 * Details: Codec Event Processing Agent
 *
 * Created on March 8, 2014, 10:26 AM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "arch/intr.h"
#include "base/bitop.h"
#include "vtimer/vtimer.h"
#include "events.h"
#include "driver/codec.h"
#include "bsp.h"

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
static void startCodec(struct wspace *);
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
            
            initCodec(wspace);
            codecPowerDown();

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
            codecPowerUp();
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
            codecClockEnable();
            startCodec(wspace);
            cpumpEnable();
            audioSwitchSpeaker();

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
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(10000u),
                codecTimeout,
                NULL);

            return (ES_STATE_HANDLED());
        }
        case EVT_TIMEOUT_ : {
            esVTimerStart(
                &wspace->timeout,
                ES_VTMR_TIME_TO_TICK_MS(10000u),
                codecTimeout,
                NULL);
            
#if (CONFIG_DEBUG == 1)
            {
                volatile uint16_t reg;

                reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_1);
                reg = codecReadReg(&wspace->codec, CODEC_REG_ADC_GAIN);
                reg = codecReadReg(&wspace->codec, CODEC_REG_DAC_GAIN);
                reg = codecReadReg(&wspace->codec, CODEC_REG_SIDETONE);
                reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_2);
                reg = codecReadReg(&wspace->codec, CODEC_REG_POWER_CTRL);
                reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_3);
                reg = codecReadReg(&wspace->codec, CODEC_REG_PLL_1);
                reg = codecReadReg(&wspace->codec, CODEC_REG_PLL_2);
                reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_4);
                reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_5);
            }
#endif
            return (ES_STATE_HANDLED());
        }
        case EVT_CODEC_ENABLE_AUDIO : {

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
}

static void invertOneChannel(
    struct wspace *     wspace) {

    int16_t leftCoef[] = {
        32767,      0,          0,          32767,      0,          0,
        0,          0,          0,          0
    };
    int16_t rightCoef[] = {
        -32767,     0,          0,          32767,      0,          0,
        0,          0,          0,          0
    };
    codecWriteArray(
        &wspace->codec,
        CODEC_REG_LEFT_COEF,
        (uint16_t *)leftCoef,
        ES_ARRAY_DIMENSION(leftCoef));
    codecWriteArray(
        &wspace->codec,
        CODEC_REG_RIGHT_COEF,
        (uint16_t *)rightCoef,
        ES_ARRAY_DIMENSION(rightCoef));
}

static void startCodec(
    struct wspace *     wspace) {

    /*==  Reset the codec to power on defaults  ==============================*/
    codecWriteReg(&wspace->codec, CODEC_REG_RESET, CODEC_RESET_CODE);

    /*==  Codec power-up and set up/enable the PLL  ==========================*/
    /*--  Clock settings  ----------------------------------------------------*/
    /*
     * MCLK = 12MHz, Fsref = 48kHz, Q = N/A, P = 1, K = 8192, J = 8, D = 1920
     */
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_PLL_1,
        CODEC_PLL_1_PLLSEL_ON     | CODEC_PLL_1_QVAL_PAR(0x0) |
        CODEC_PLL_1_PVAL_PAR(0x1) | CODEC_PLL_1_JVAL_PAR(0x8));
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_PLL_2,
        CODEC_PLL_2_DVAL_PAR(1920));

    /*--  DAC Volume, sampling rate, master/slave selection  -----------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_3,
        CODEC_AUDIO_CTRL3_DMSVOL_INDEPENDENT | CODEC_AUDIO_CTRL3_REFFS_48KHZ |
        CODEC_AUDIO_CTRL3_DAXFM_256S         | CODEC_AUDIO_CTRL3_SLVMS_SLAVE |
        CODEC_AUDIO_CTRL3_DAPK2PK_2633       | CODEC_AUDIO_CTRL3_AGCNL_60DB  |
        CODEC_AUDIO_CTRL3_CLPST_OFF);

    /*--  I2S, input, high-pass, Fsaaple settings  ---------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_1,
        CODEC_AUDIO_CTRL_1_ADCHPF_DISABLED | CODEC_AUDIO_CTRL_1_ADCIN_SINGLE_MIC |
        CODEC_AUDIO_CTRL_1_WLEN_24BIT      | CODEC_AUDIO_CTRL_1_DATFM_I2S        |
        CODEC_AUDIO_CTRL_1_DACFS_DIV1      | CODEC_AUDIO_CTRL_1_ADCFS_DIV1);

    /*--  Soft-stepping, keyclick settings  ----------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_2,
        CODEC_AUDIO_CTRL_2_KCLEN_OFF     | CODEC_AUDIO_CTRL_2_KCLAC_LOW  |
        CODEC_AUDIO_CTRL_2_APGASS_SINGLE | CODEC_AUDIO_CTRL_2_KCLFRQ_625 |
        CODEC_AUDIO_CTRL_2_KCLLN_2       | CODEC_AUDIO_CTRL_2_DASTC_SINGLE);

    /*--  ADC settings  ------------------------------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_ADC_GAIN,
        CODEC_ADC_GAIN_ADMUT_MUTED | CODEC_ADC_GAIN_ADPGA_0     |
        CODEC_ADC_GAIN_AGCTG_55    | CODEC_ADC_GAIN_AGCTC_8_100 |
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

    /*--  Disable short-circuit protection  ----------------------------------*/
    codecWriteReg(
        &wspace->codec,
        CODEC_REG_AUDIO_CTRL_4,
        0);

    /*==  Set up input circuitry  ============================================*/
    codecRegModify(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_PWDNC_Msk | CODEC_POWER_CTRL_ASTPWD_Msk,
        CODEC_POWER_CTRL_PWDNC_ON  | CODEC_POWER_CTRL_ASTPWD_OFF);
    codecRegModify(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_ADPWDN_Msk,
        CODEC_POWER_CTRL_ADPWDN_ON);
    codecRegModify(
        &wspace->codec,
        CODEC_REG_ADC_GAIN,
        CODEC_ADC_GAIN_ADMUT_Msk,
        CODEC_ADC_GAIN_ADMUT_NOT_MUTED);

    /*==  Set up output circuitry  ===========================================*/
    codecRegModify(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_VGPWDN_Msk,
        CODEC_POWER_CTRL_VGPWDN_OFF);
    codecRegModify(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_DAPWDN_Msk,
        CODEC_POWER_CTRL_DAPWDN_ON);
    codecRegModify(
        &wspace->codec,
        CODEC_REG_POWER_CTRL,
        CODEC_POWER_CTRL_DAODRC_Msk,
        CODEC_POWER_CTRL_DAODRC_HIGH_POWER);
    codecRegModify(
        &wspace->codec,
        CODEC_REG_DAC_GAIN,
        CODEC_DAC_GAIN_DALMU_Msk | CODEC_DAC_GAIN_DALVL_Msk |
        CODEC_DAC_GAIN_DARMU_Msk | CODEC_DAC_GAIN_DARVL_Msk,
        CODEC_DAC_GAIN_DALMU_NOT_MUTED | CODEC_DAC_GAIN_DALVL_0 |
        CODEC_DAC_GAIN_DARMU_NOT_MUTED | CODEC_DAC_GAIN_DARVL_0);

#if (CONFIG_DEBUG == 1)
    {
        volatile uint16_t reg;

        reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_1);
        reg = codecReadReg(&wspace->codec, CODEC_REG_ADC_GAIN);
        reg = codecReadReg(&wspace->codec, CODEC_REG_DAC_GAIN);
        reg = codecReadReg(&wspace->codec, CODEC_REG_SIDETONE);
        reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_2);
        reg = codecReadReg(&wspace->codec, CODEC_REG_POWER_CTRL);
        reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_3);
        reg = codecReadReg(&wspace->codec, CODEC_REG_PLL_1);
        reg = codecReadReg(&wspace->codec, CODEC_REG_PLL_2);
        reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_4);
        reg = codecReadReg(&wspace->codec, CODEC_REG_AUDIO_CTRL_5);
    }
#endif
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
