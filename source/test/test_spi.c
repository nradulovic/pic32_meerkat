/*
 * File:   main.c
 * Author: nenad
 *
 * Created on February 15, 2014, 2:14 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "test/test_spi.h"
#include "driver/spi.h"
#include "config/config_test.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/
/*=======================================================  LOCAL VARIABLES  ==*/
/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/
/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void execTestSpi(
    void) {
#if (CONFIG_PROJ_TEST_SPIS == 1)
    {
        struct spiHandle testSpi;
        struct spiConfig testSpiConfig = SPI_CONFIG_INITIALIZER(
            &GlobalSpis,
            0,
            1000000,
            1,
            SPIS_SDI_A0,
            SPIS_SDO_A1,
            SPIS_SCK_A9,
            SPIS_SS_A10);

        uint8_t         testData[] = {0, 1, 3, 8, 255};

        testSpiConfig.flags = SPI_CLOCK_PHASE_FIRST_EDGE |
                              SPI_CLOCK_POLARITY_IDLE_LOW |
                              SPI_MASTER_MODE |
                              SPI_MASTER_SS |
                              SPI_MASTER_SS_ACTIVE_HIGH |
                              SPI_DATA_8;
        spiOpen(&testSpi, &testSpiConfig);
        spiExchange(&testSpi, &testData, sizeof(testData), 0);
        spiClose(&testSpi);
        testSpiConfig.flags = SPI_CLOCK_PHASE_LAST_EDGE |
                              SPI_CLOCK_POLARITY_IDLE_LOW |
                              SPI_MASTER_MODE |
                              SPI_MASTER_SS |
                              SPI_MASTER_SS_ACTIVE_HIGH |
                              SPI_DATA_8;
        spiOpen(&testSpi, &testSpiConfig);
        spiExchange(&testSpi, &testData, sizeof(testData), 0);
        spiClose(&testSpi);
        testSpiConfig.flags = SPI_CLOCK_PHASE_FIRST_EDGE |
                              SPI_CLOCK_POLARITY_IDLE_HIGH |
                              SPI_MASTER_MODE |
                              SPI_MASTER_SS |
                              SPI_MASTER_SS_ACTIVE_HIGH |
                              SPI_DATA_8;
        spiOpen(&testSpi, &testSpiConfig);
        spiExchange(&testSpi, &testData, sizeof(testData), 0);
        spiClose(&testSpi);
    }
#endif /* (CONFIG_PROJ_TEST_SPIS == 1) */
#if (CONFIG_PROJ_TEST_CODEC_SPI == 1)
    {
        struct spiHandle testSpi;
        struct spiConfig testSpiConfig = SPI_CONFIG_INITIALIZER(
            &GlobalSpis,
            0,
            1000000,
            1,
            SPIS_SDI_C6,
            SPIS_SDO_B8,
            SPIS_SCK_B9,
            SPIS_SS_B10);
        uint16_t testData[] = {
            (0x1u << 15) | (0x01 << 11), /* Read AUX ADC register, it should have some bits set */
            0,
            0
        };
        testSpiConfig.flags = SPI_CLOCK_PHASE_LAST_EDGE |
                              SPI_CLOCK_POLARITY_IDLE_LOW |
                              SPI_MASTER_MODE |
                              SPI_MASTER_SS |
                              SPI_MASTER_SS_ACTIVE_LOW |
                              SPI_DATA_16;

        spiOpen(&testSpi, &testSpiConfig);
        spiExchange(&testSpi, &testData, sizeof(testData), 0);
        spiClose(&testSpi);
    }
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of test_spi.c
 ******************************************************************************/
