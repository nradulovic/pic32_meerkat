/* 
 * File:   config_pins.h
 * Author: nenad
 *
 * Created on February 22, 2014, 9:04 PM
 */

#ifndef CONFIG_PINS_H
#define	CONFIG_PINS_H

#ifdef	__cplusplus
extern "C" {
#endif

#if (__PIC32_FEATURE_SET__ == 150)
#define CONFIG_BT_CMD_GPIO_PORT         &GlobalGpioC
#define CONFIG_BT_CMD_GPIO_PIN          1
#define CONFIG_BT_DEF_GPIO_PORT         &GlobalGpioC
#define CONFIG_BT_DEF_GPIO_PIN          0
#define CONFIG_BT_PWR_GPIO_PORT         &GlobalGpioC
#define CONFIG_BT_PWR_GPIO_PIN          7
#define CONFIG_BT_UART_TX_PIN           UART1_TX_B15
#define CONFIG_BT_UART_RX_PIN           UART1_RX_B13
#define CONFIG_BT_UART_RTS_PIN          UART1_RTS_A3
#define CONFIG_BT_UART_CTS_PIN          UART1_CTS_A1
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_PINS_H */
