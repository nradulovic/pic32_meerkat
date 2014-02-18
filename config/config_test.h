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

#if (CONFIG_PROJ_TEST_SPI == 1) || defined(__DOXYGEN__)
/**@brief       Enable/disable Software SPI tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_SPIS           0

/**@brief       Enable/disable SPI1 tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_SPI1           0

/**@brief       Enable/disable Codec SPI tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_CODEC_SPI      1
#else /* (CONFIG_PROJ_TEST_SPI == 1) */
/*--  All configuration options are turned off here  -------------------------*/
#define CONFIG_PROJ_TEST_SPIS           0
#define CONFIG_PROJ_TEST_SPI1           0
#define CONFIG_PROJ_TEST_CODEC_SPI      0
#endif /* !(CONFIG_PROJ_TEST_SPI == 1) */

#if (CONFIG_PROJ_TEST_UART == 1) || defined(__DOXYGEN__)
/**@brief       Enable/disable UART1 tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_UART1          0
#define CONFIG_PROJ_TEST_UART1_ISR      0
#define CONFIG_PROJ_TEST_UART_BT        1
#else
/*--  All configuration options are turned off here  -------------------------*/
#define CONFIG_PROJ_TEST_UART1          0
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_TEST_H */

