/* 
 * File:   neon_eds_app_config.h
 * Author: nenad
 *
 * Created on March 20, 2016, 11:28 PM
 */

#ifndef ENVIRONMENT_NEON_EDS_APP_CONFIG_H_
#define ENVIRONMENT_NEON_EDS_APP_CONFIG_H_

#define CONFIG_DEBUG 1
#define CONFIG_API_VALIDATION 1
#define CONFIG_ASSERT_INTERNAL 1

#define CONFIG_CORE_TIMER_SOURCE 0

/* NOTE:
 * Timer 5 is allocated to APB1 with prescaler 2
 */
#define CONFIG_CORE_TIMER_CLOCK_FREQ    80000000

#define CONFIG_CORE_TIMER_EVENT_FREQ    1000

/* NOTE:
 * Some ISR will have higher level of priority than Neon ISRs
 */
#define CONFIG_CORE_LOCK_MAX_LEVEL 159

#endif /* ENVIRONMENT_NEON_EDS_APP_CONFIG_H_ */
