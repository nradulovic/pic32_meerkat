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
#define CONFIG_BT_UART                  &Uart1
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

#define CONFIG_CODEC_SS_PIN             SPIS_SS_B10
#define CONFIG_CODEC_SCK_PIN            SPIS_SCK_B9
#define CONFIG_CODEC_SDO_PIN            SPIS_SDO_B8
#define CONFIG_CODEC_SDI_PIN            SPIS_SDI_C6
#define CONFIG_CODEC_CLOCK_OUT_PIN      CLOCK_OUT_1
#define CONFIG_CODEC_RESET_PORT         &GpioB
#define CONFIG_CODEC_RESET_PIN          11
#define CONFIG_CODEC_POWER_PORT         &GpioB
#define CONFIG_CODEC_POWER_PIN          12

#define CONFIG_RADIO_UART               &Uart2
#define CONFIG_RADIO_UART_TX_PIN        UART2_TX_C4
#define CONFIG_RADIO_UART_RX_PIN        UART2_RX_A9
#define CONFIG_RADIO_UART_RTS_PIN       UART2_RTS_C5
#define CONFIG_RADIO_UART_CTS_PIN       UART2_CTS_A4

#define CONFIG_AUDIO_MUX_CB1_PORT       &GpioB
#define CONFIG_AUDIO_MUX_CB1_PIN        6
#define CONFIG_AUDIO_MUX_CB2_PORT       &GpioB
#define CONFIG_AUDIO_MUX_CB2_PIN        7

#define CONFIG_NOTIFY_LED_A1_PORT       &GpioB
#define CONFIG_NOTIFY_LED_A1_PIN        5
#define CONFIG_NOTIFY_LED_A2_PORT       &GpioA
#define CONFIG_NOTIFY_LED_A2_PIN        4
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_PINS_H */

