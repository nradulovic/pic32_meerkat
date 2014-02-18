/* 
 * File:   project.h
 * Author: nenad
 *
 * Created on February 15, 2014, 1:58 PM
 */

#ifndef CONFIG_PROJECT_H_
#define	CONFIG_PROJECT_H_

#ifdef	__cplusplus
extern "C" {
#endif

/**@brief       Enable/disable SPI tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_SPI            0

/**@brief       Enable/disable UART tests
 * @details     0 - disable
 *              1 - enable
 */
#define CONFIG_PROJ_TEST_UART           1

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_PROJECT_H_ */

