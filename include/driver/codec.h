/* 
 * File:    codec.h
 * Author:  nenad
 * Details: Audio codec driver
 *
 * Created on February 15, 2014, 11:11 PM
 */

#ifndef CODEC_H_
#define	CODEC_H_

/*=========================================================  INCLUDE FILES  ==*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "plat/compiler.h"
#include "driver/spi.h"

/*===============================================================  MACRO's  ==*/

#define CONFIG_MASTER_CLOCK             12000000ul

#define CODEC_REG_ADDR(page, offset)    (((page) << 11) | ((offset) << 5))

#define CODEC_AUDIO_CTRL_1_ADCFS_DIV1       (0x0u << 0)
#define CODEC_AUDIO_CTRL_1_DACFS_DIV1       (0x0u << 3)
#define CODEC_AUDIO_CTRL_1_DATFM_I2S        (0x0u << 8)
#define CODEC_AUDIO_CTRL_1_WLEN_24BIT       (0x2u << 10)
#define CODEC_AUDIO_CTRL_1_ADCIN_SINGLE_MIC (0x0u << 12)
#define CODEC_AUDIO_CTRL_1_ADCHPF_DISABLED  (0x0u << 14)

#define CODEC_ADC_GAIN_AGCEN_OFF            (0x0 << 0)
#define CODEC_ADC_GAIN_AGCTC_8_100          (0x0 << 1)
#define CODEC_ADC_GAIN_AGCTG_55             (0x0 << 5)
#define CODEC_ADC_GAIN_ADPGA_0              (0x0 << 8)
#define CODEC_ADC_GAIN_ADMUT_NOT_MUTED      (0x0 << 15)

#define CODEC_DAC_GAIN_DARVL_0              (0x0 << 0)
#define CODEC_DAC_GAIN_DARMU_NOT_MUTED      (0x0 << 7)
#define CODEC_DAC_GAIN_DALVL_0              (0x0 << 8)
#define CODEC_DAC_GAIN_DALMU_NOT_MUTED      (0x0 << 15)

#define CODEC_SIDETONE_DSTG_0               (0x0 << 1)
#define CODEC_SIDETONE_DSTMU_MUTED          (0x1 << 7)
#define CODEC_SIDETONE_ASTG_M345            (0x0 << 8)
#define CODEC_SIDETONE_ASTMU_MUTED          (0x1 << 15)

#define CODEC_AUDIO_CTRL_2_DASTC_SINGLE     (0x0 << 1)
#define CODEC_AUDIO_CTRL_2_KCLLN_2          (0x0 << 4)
#define CODEC_AUDIO_CTRL_2_KCLFRQ_625       (0x0 << 8)
#define CODEC_AUDIO_CTRL_2_APGASS_SINGLE    (0x0 << 11)
#define CODEC_AUDIO_CTRL_2_KCLAC_LOW        (0x0 << 12)
#define CODEC_AUDIO_CTRL_2_KCLEN_OFF        (0x0 << 15)

#define CODEC_POWER_CTRL_DEEMPF_OFF         (0x0 << 0)
#define CODEC_POWER_CTRL_EFFCTL_OFF         (0x0 << 1)
#define CODEC_POWER_CTRL_VBIAS_25           (0x0 << 4)
#define CODEC_POWER_CTRL_ADWSF_POWERDOWN    (0x0 << 5)
#define CODEC_POWER_CTRL_VGPWDN_ON          (0x0 << 8)
#define CODEC_POWER_CTRL_ADPWDN_ON          (0x0 << 9)
#define CODEC_POWER_CTRL_DAPWDN_ON          (0x0 << 10)
#define CODEC_POWER_CTRL_DAODRC_LOW_POWER   (0x0 << 12)
#define CODEC_POWER_CTRL_ASTPWD_OFF         (0x1 << 13)
#define CODEC_POWER_CTRL_PWDNC_ON           (0x0 << 15)

#define CODEC_PLL_1_JVAL_PAR(val)           (((val) & 0x3fu) << 2)
#define CODEC_PLL_1_PVAL_PAR(val)           (((val) & 0x7u) << 8)
#define CODEC_PLL_1_QVAL_PAR(val)           (((val) & 0xfu) << 11)
#define CODEC_PLL_1_PLLSEL_ON               (0x1 << 15)

#define CODEC_PLL_2_DVAL_PAR(val)           (((val) & 0x3fffu) << 2)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum codecReg {
    CODEC_REG_STATUS            = CODEC_REG_ADDR(0x1, 0x01),
    CODEC_REG_AUDIO_CTRL_1      = CODEC_REG_ADDR(0x2, 0x00),
    CODEC_REG_ADC_GAIN          = CODEC_REG_ADDR(0x2, 0x01),
    CODEC_REG_DAC_GAIN          = CODEC_REG_ADDR(0x2, 0x02),
    CODEC_REG_SIDETONE          = CODEC_REG_ADDR(0x2, 0x03),
    CODEC_REG_AUDIO_CTRL_2      = CODEC_REG_ADDR(0x2, 0x04),
    CODEC_REG_POWER_CTRL        = CODEC_REG_ADDR(0x2, 0x05),
    CODEC_REG_PLL_1             = CODEC_REG_ADDR(0x2, 0x1b),
    CODEC_REG_PLL_2             = CODEC_REG_ADDR(0x2, 0x1c)
};

struct codecConfig {
    const struct spiConfig * spi;
};

struct codecHandle {
    struct spiHandle    spi;
};

/*======================================================  GLOBAL VARIABLES  ==*/
/*===================================================  FUNCTION PROTOTYPES  ==*/

void initCodecDriver(
    void);

void codecOpen(
    struct codecHandle * handle,
    const struct codecConfig * config);

void codecWriteReg(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t            value);

uint16_t codecReadReg(
    struct codecHandle * handle,
    enum codecReg       reg);

void codecAudioEnable(
    void);

void codecAudioDisable(
    void);

void codecClose(
    struct codecHandle * handle);

/*--------------------------------------------------------  C++ extern end  --*/
#ifdef __cplusplus
}
#endif

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//** @} *//*********************************************
 * END of spi.h
 ******************************************************************************/
#endif /* CODEC_H_ */
