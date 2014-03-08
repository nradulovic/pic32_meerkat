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
#define CONFIG_BT_GPIO_CMD_PORT         &GpioC
#define CONFIG_BT_GPIO_CMD_PIN          1
#define CONFIG_BT_GPIO_DEF_PORT         &GpioC
#define CONFIG_BT_GPIO_DEF_PIN          0
#define CONFIG_BT_GPIO_PWR_PORT         &GpioC
#define CONFIG_BT_GPIO_PWR_PIN          8
#define CONFIG_BT_UART_TX_PIN           UART1_TX_B15
#define CONFIG_BT_UART_RX_PIN           UART1_RX_B13
#define CONFIG_BT_UART_RTS_PIN          UART1_RTS_A3
#define CONFIG_BT_UART_CTS_PIN          UART1_CTS_A1

#define CONFIG_CPUMP_EN_PORT            &GpioB
#define CONFIG_CPUMP_EN_PIN             4

#define CONFIG_RADIO_CHARGER_EN_PORT    &GpioA
#define CONFIG_RADIO_CHARGER_EN_PIN     3

#define CONFIG_BATT_CHARGER_EN_PORT     &GpioA
#define CONFIG_BATT_CHARGER_EN_PIN      8

#define CONFIG_CODEC_CLOCK_OUT_PIN      CLOCK_OUT_1

#define CONFIG_AUDIO_SW_CB1_PORT
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_PINS_H */

