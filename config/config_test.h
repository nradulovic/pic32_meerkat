/* 
 * File:   config_test.h
 * Author: nenad
 *
 * Created on February 15, 2014, 2:04 PM
 */

#ifndef CONFIG_TEST_H
#define	CONFIG_TEST_H

#include "../config/config_project.h"

#ifdef	__cplusplus
extern "C" {
#endif

#if (CONFIG_PROJ_TEST_SPI == 1)

#define CONFIG_PROJ_TEST_SPIS           0
#define CONFIG_PROJ_TEST_SPI1           0
#define CONFIG_PROJ_TEST_CODEC_SPI      1
#else /* (CONFIG_PROJ_TEST_SPI == 1) */
#define CONFIG_PROJ_TEST_SPIS           0
#define CONFIG_PROJ_TEST_SPI1           0
#define CONFIG_PROJ_TEST_CODEC_SPI      0
#endif /* !(CONFIG_PROJ_TEST_SPI == 1) */

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_TEST_H */

