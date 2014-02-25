/*
 * File:   main.c
 * Author: nenad
 *
 * Created on February 16, 2014, 5:07 PM
 */

/*=========================================================  INCLUDE FILES  ==*/

#include "test/test_uart.h"
#include "driver/uart.h"
#include "config/config_test.h"

/*=========================================================  LOCAL MACRO's  ==*/
/*======================================================  LOCAL DATA TYPES  ==*/
/*=============================================  LOCAL FUNCTION PROTOTYPES  ==*/

#if ((CONFIG_PROJ_TEST_UART1_ISR == 1) || (CONFIG_PROJ_TEST_UART_BT == 1))
size_t testUartReader(
    enum uartError,
    void *,
    size_t);
#endif
#if (CONFIG_PROJ_TEST_UART1_ISR == 1)
size_t testUartWriter(
    enum uartError,
    void *,
    size_t);
#endif

/*=======================================================  LOCAL VARIABLES  ==*/

#if (CONFIG_PROJ_TEST_UART1_ISR == 1)
static bool GlobalWriteWait;
#endif
#if (CONFIG_PROJ_TEST_UART_BT == 1)
static bool GlobalReadWait;
#endif

/*======================================================  GLOBAL VARIABLES  ==*/
/*============================================  LOCAL FUNCTION DEFINITIONS  ==*/

#if ((CONFIG_PROJ_TEST_UART1_ISR == 1) || (CONFIG_PROJ_TEST_UART_BT == 1))
size_t testUartReader(
    enum uartError      error,
    void *              buffer,
    size_t              bufferSize) {

    (void)error;
    (void)buffer;
    (void)bufferSize;
#if (CONFIG_PROJ_TEST_UART_BT == 1)
    GlobalReadWait = false;
#endif
    return (0);
}
#endif
#if (CONFIG_PROJ_TEST_UART1_ISR == 1)
size_t testUartWriter(
    enum uartError      error,
    void *              buffer,
    size_t              bufferSize) {

    GlobalWriteWait = false;
    (void)error;
    (void)buffer;
    (void)bufferSize;

    return (0);
}
#endif

/*===================================  GLOBAL PRIVATE FUNCTION DEFINITIONS  ==*/
/*====================================  GLOBAL PUBLIC FUNCTION DEFINITIONS  ==*/

void execTestUart(
    void) {
#if (CONFIG_PROJ_TEST_UART1 == 1)
    {
        struct uartHandle testUart;
        struct uartConfig testUartConfig = {
            &Uart1,
            UART_TX_ENABLE |
            UART_RX_ENABLE |
            UART_DATA_BITS_8 |
            UART_STOP_BITS_1 |
            UART_PARITY_NONE,
            115200u,
            2u,
            {
                UART1_TX_B15,
                UART1_RX_B13,
                UART1_CTS_A1,
                UART1_RTS_A3
            }
        };
        uint8_t         testData[] = {0, 1, 3, 8, 255, 0};
        uartOpen(&testUart, &testUartConfig);
        uartWrite(&testUart, testData, sizeof(testData), 0);
        uartClose(&testUart);
    }
#endif
#if (CONFIG_PROJ_TEST_UART1_ISR == 1)
        {
        struct uartHandle testUart;
        struct uartConfig testUartConfig = {
            &Uart1,
            UART_TX_ENABLE |
            UART_RX_ENABLE |
            UART_DATA_BITS_8 |
            UART_STOP_BITS_1 |
            UART_PARITY_NONE,
            115200,
            2u,
            {
                UART1_TX_B15,
                UART1_RX_B13,
                UART1_CTS_A1,
                UART1_RTS_A3
            }
        };
        uint8_t         testData[] = {0, 64, 255};
        uint8_t         receivedData[100];

        GlobalWriteWait = true;

        uartOpen(&testUart, &testUartConfig);
        uartSetReader(&testUart, testUartReader);
        uartSetWriter(&testUart, testUartWriter);
        uartReadStart(&testUart, receivedData, sizeof(receivedData));
        uartWriteStart(&testUart, testData, sizeof(testData));
        
        while (GlobalWriteWait == true); /* Normaly we would not wait for write but this test case is an exception */

        uartWriteStop(&testUart);
        uartReadStop(&testUart);
        uartClose(&testUart);
    }
#endif
#if (CONFIG_PROJ_TEST_UART_BT == 1)
    {
        struct uartHandle testUart;
        struct uartConfig testUartConfig = {
            &Uart1,
            UART_TX_ENABLE |
            UART_RX_ENABLE |
            UART_DATA_BITS_8 |
            UART_STOP_BITS_1 |
            UART_PARITY_NONE,
            115200,
            2u,
            {
                UART1_TX_B15,
                UART1_RX_B13,
                UART1_CTS_A1,
                UART1_RTS_A3
            }
        };
        uint8_t         receivedData[5];

        GlobalReadWait = true;
        
        uartOpen(&testUart, &testUartConfig);
        uartSetReader(&testUart, testUartReader);
        uartReadStart(&testUart, receivedData, sizeof(receivedData));
        LATCCLR  = (0x1 << 1);

        while (GlobalReadWait == true); /* Normaly we would not wait for read but this test case is an exception */

        uartReadStop(&testUart);
        uartClose(&testUart);
        LATCSET  = (0x1 << 1);
    }
#endif
}

/*================================*//** @cond *//*==  CONFIGURATION ERRORS  ==*/
/** @endcond *//** @} *//******************************************************
 * END of test_spi.c
 ******************************************************************************/
