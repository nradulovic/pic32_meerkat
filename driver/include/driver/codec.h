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

#define CODEC_RESET_CODE                    (0xbb00u)

#define CODEC_AUDIO_CTRL_1_ADCFS_DIV1       (0x0u << 0)
#define CODEC_AUDIO_CTRL_1_DACFS_DIV1       (0x0u << 3)
#define CODEC_AUDIO_CTRL_1_DATFM_I2S        (0x0u << 8)
#define CODEC_AUDIO_CTRL_1_WLEN_24BIT       (0x2u << 10)
#define CODEC_AUDIO_CTRL_1_ADCIN_SINGLE_MIC (0x0u << 12)
#define CODEC_AUDIO_CTRL_1_ADCHPF_DISABLED  (0x0u << 14)

#define CODEC_ADC_GAIN_AGCEN_OFF            (0x0u << 0)
#define CODEC_ADC_GAIN_AGCTC_8_100          (0x0u << 1)
#define CODEC_ADC_GAIN_AGCTG_55             (0x0u << 5)
#define CODEC_ADC_GAIN_ADPGA(val)           ((val) << 8)
#define CODEC_ADC_GAIN_ADMUT_NOT_MUTED      (0x0u << 15)
#define CODEC_ADC_GAIN_ADMUT_MUTED          (0x1u << 15)
#define CODEC_ADC_GAIN_ADMUT_Msk            (0x1u << 15)

#define CODEC_DAC_GAIN_DARVL_0              (0x0u << 0)
#define CODEC_DAC_GAIN_DARVL_Msk            (0x7fu << 0)
#define CODEC_DAC_GAIN_DARMU_NOT_MUTED      (0x0u << 7)
#define CODEC_DAC_GAIN_DARMU_Msk            (0x1u << 7)
#define CODEC_DAC_GAIN_DALVL_0              (0x0u << 8)
#define CODEC_DAC_GAIN_DALVL_Msk            (0x7fu << 8)
#define CODEC_DAC_GAIN_DALMU_NOT_MUTED      (0x0u << 15)
#define CODEC_DAC_GAIN_DALMU_Msk            (0x1u << 15)

#define CODEC_SIDETONE_DSTG_0               (0x0u << 1)
#define CODEC_SIDETONE_DSTMU_MUTED          (0x1u << 7)
#define CODEC_SIDETONE_ASTG_M345            (0x0u << 8)
#define CODEC_SIDETONE_ASTMU_MUTED          (0x1u << 15)

#define CODEC_AUDIO_CTRL_2_DASTC_SINGLE     (0x0u << 1)
#define CODEC_AUDIO_CTRL_2_KCLLN_2          (0x0u << 4)
#define CODEC_AUDIO_CTRL_2_KCLFRQ_625       (0x0u << 8)
#define CODEC_AUDIO_CTRL_2_APGASS_SINGLE    (0x0u << 11)
#define CODEC_AUDIO_CTRL_2_KCLAC_LOW        (0x0u << 12)
#define CODEC_AUDIO_CTRL_2_KCLEN_OFF        (0x0u << 15)

#define CODEC_POWER_CTRL_DEEMPF_OFF         (0x0u << 0)
#define CODEC_POWER_CTRL_DEEMPF_Msk         (0x1u << 0)
#define CODEC_POWER_CTRL_EFFCTL_ON          (0x1u << 1)
#define CODEC_POWER_CTRL_EFFCTL_OFF         (0x0u << 1)
#define CODEC_POWER_CTRL_EFFCTL_Msk         (0x1u << 1)
#define CODEC_POWER_CTRL_VBIAS_25           (0x0u << 4)
#define CODEC_POWER_CTRL_VBIAS_Msk          (0x1u << 4)
#define CODEC_POWER_CTRL_ADWSF_POWERDOWN    (0x0u << 5)
#define CODEC_POWER_CTRL_ADWSF_Msk          (0x1u << 5)
#define CODEC_POWER_CTRL_VGPWDN_ON          (0x0u << 8)
#define CODEC_POWER_CTRL_VGPWDN_OFF         (0x1u << 8)
#define CODEC_POWER_CTRL_VGPWDN_Msk         (0x1u << 8)
#define CODEC_POWER_CTRL_ADPWDN_ON          (0x0u << 9)
#define CODEC_POWER_CTRL_ADPWDN_Msk         (0x1u << 9)
#define CODEC_POWER_CTRL_DAPWDN_ON          (0x0u << 10)
#define CODEC_POWER_CTRL_DAPWDN_Msk         (0x1u << 10)
#define CODEC_POWER_CTRL_DAODRC_LOW_POWER   (0x0u << 12)
#define CODEC_POWER_CTRL_DAODRC_HIGH_POWER  (0x1u << 12)
#define CODEC_POWER_CTRL_DAODRC_Msk         (0x1u << 12)
#define CODEC_POWER_CTRL_ASTPWD_OFF         (0x1u << 13)
#define CODEC_POWER_CTRL_ASTPWD_Msk         (0x1u << 13)
#define CODEC_POWER_CTRL_PWDNC_ON           (0x0u << 15)
#define CODEC_POWER_CTRL_PWDNC_Msk          (0x1u << 15)

#define CODEC_AUDIO_CTRL3_CLPST_OFF             (0x0u << 3)
#define CODEC_AUDIO_CTRL3_AGCNL_60DB            (0x0u << 4)
#define CODEC_AUDIO_CTRL3_DAPK2PK_2192          (0x1u << 9)
#define CODEC_AUDIO_CTRL3_DAPK2PK_2402          (0x2u << 9)
#define CODEC_AUDIO_CTRL3_DAPK2PK_2633          (0x3u << 9)
#define CODEC_AUDIO_CTRL3_SLVMS_SLAVE           (0x0u << 11)
#define CODEC_AUDIO_CTRL3_DAXFM_CONTINUOS       (0x0u << 12)
#define CODEC_AUDIO_CTRL3_DAXFM_256S            (0x1u << 12)
#define CODEC_AUDIO_CTRL3_REFFS_48KHZ           (0x0u << 13)
#define CODEC_AUDIO_CTRL3_DMSVOL_INDEPENDENT    (0x0u << 14)

#define CODEC_AUDIO_CTRL4_SHCKT_DIS             (0x1u << 8)

#define CODEC_PLL_1_JVAL_PAR(val)           (((val) & 0x3fu) << 2)
#define CODEC_PLL_1_PVAL_PAR(val)           (((val) & 0x7u) << 8)
#define CODEC_PLL_1_QVAL_PAR(val)           (((val) & 0xfu) << 11)
#define CODEC_PLL_1_PLLSEL_ON               (0x1u << 15)

#define CODEC_AUDIO_CTRL4_SHCKT_DIS_OFF     (0x1u << 8)

#define CODEC_PLL_2_DVAL_PAR(val)           (((val) & 0x3fffu) << 2)

/*------------------------------------------------------  C++ extern begin  --*/
#ifdef	__cplusplus
extern "C" {
#endif

/*============================================================  DATA TYPES  ==*/

enum codecReg {
    CODEC_REG_STATUS            = CODEC_REG_ADDR(0x1u, 0x01u),
    CODEC_REG_RESET             = CODEC_REG_ADDR(0x1u, 0x04u),
    CODEC_REG_AUDIO_CTRL_1      = CODEC_REG_ADDR(0x2u, 0x00u),
    CODEC_REG_ADC_GAIN          = CODEC_REG_ADDR(0x2u, 0x01u),
    CODEC_REG_DAC_GAIN          = CODEC_REG_ADDR(0x2u, 0x02u),
    CODEC_REG_SIDETONE          = CODEC_REG_ADDR(0x2u, 0x03u),
    CODEC_REG_AUDIO_CTRL_2      = CODEC_REG_ADDR(0x2u, 0x04u),
    CODEC_REG_POWER_CTRL        = CODEC_REG_ADDR(0x2u, 0x05u),
    CODEC_REG_AUDIO_CTRL_3      = CODEC_REG_ADDR(0x2u, 0x06u),
    CODEC_REG_LEFT_COEF_N0      = CODEC_REG_ADDR(0x2u, 0x07u),
    CODEC_REG_LEFT_COEF_N1      = CODEC_REG_ADDR(0x2u, 0x08u),
    CODEC_REG_LEFT_COEF_N2      = CODEC_REG_ADDR(0x2u, 0x09u),
    CODEC_REG_LEFT_COEF_N3      = CODEC_REG_ADDR(0x2u, 0x0au),
    CODEC_REG_LEFT_COEF_N4      = CODEC_REG_ADDR(0x2u, 0x0bu),
    CODEC_REG_LEFT_COEF_N5      = CODEC_REG_ADDR(0x2u, 0x0cu),
    CODEC_REG_LEFT_COEF_D1      = CODEC_REG_ADDR(0x2u, 0x0du),
    CODEC_REG_LEFT_COEF_D2      = CODEC_REG_ADDR(0x2u, 0x0eu),
    CODEC_REG_LEFT_COEF_D4      = CODEC_REG_ADDR(0x2u, 0x0fu),
    CODEC_REG_LEFT_COEF_D5      = CODEC_REG_ADDR(0x2u, 0x10u),
    CODEC_REG_RIGHT_COEF_N0     = CODEC_REG_ADDR(0x2u, 0x11u),
    CODEC_REG_RIGHT_COEF_N1     = CODEC_REG_ADDR(0x2u, 0x12u),
    CODEC_REG_RIGHT_COEF_N2     = CODEC_REG_ADDR(0x2u, 0x13u),
    CODEC_REG_RIGHT_COEF_N3     = CODEC_REG_ADDR(0x2u, 0x14u),
    CODEC_REG_RIGHT_COEF_N4     = CODEC_REG_ADDR(0x2u, 0x15u),
    CODEC_REG_RIGHT_COEF_N5     = CODEC_REG_ADDR(0x2u, 0x16u),
    CODEC_REG_RIGHT_COEF_D1     = CODEC_REG_ADDR(0x2u, 0x17u),
    CODEC_REG_RIGHT_COEF_D2     = CODEC_REG_ADDR(0x2u, 0x18u),
    CODEC_REG_RIGHT_COEF_D4     = CODEC_REG_ADDR(0x2u, 0x19u),
    CODEC_REG_RIGHT_COEF_D5     = CODEC_REG_ADDR(0x2u, 0x1au),
    CODEC_REG_PLL_1             = CODEC_REG_ADDR(0x2u, 0x1bu),
    CODEC_REG_PLL_2             = CODEC_REG_ADDR(0x2u, 0x1cu),
    CODEC_REG_AUDIO_CTRL_4      = CODEC_REG_ADDR(0x2u, 0x1du),
    CODEC_REG_AUDIO_CTRL_5      = CODEC_REG_ADDR(0x2u, 0x1eu)
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

void codecResetEnable(
    void);

void codecResetDisable(
    void);

void codecPowerUp(
    void);

void codecPowerDown(
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

void codecRegModify(
    struct codecHandle * handle,
    enum codecReg       reg,
    uint16_t            mask,
    uint16_t            value);

void codecWriteArray(
    struct codecHandle * handle,
    enum codecReg       reg,
    const uint16_t *    array,
    size_t              size);

void codecClockEnable(
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
