/*
 * aya_driver_uart.c
 *
 *  Created on: 2023年4月2日
 *      Author: KurehaTian
 */

#include "aya_driver_uart.h"

const uint32_t UART_BASE[8] = {UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE, UART4_BASE, UART5_BASE, UART6_BASE, UART7_BASE};

void uart_init(uart_index_t uart_n, uint32_t baudrate)
{
    // 1.Init the clock of UART and it's gpio.
    // 2.GPIO configure.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0 + uart_n);
    switch (uart_n)
    {
    case UART_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        GPIOPinConfigure(GPIO_PA0_U0RX);
        GPIOPinConfigure(GPIO_PA1_U0TX);
        GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), baudrate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTFIFOEnable(UART0_BASE);
        // 接收发送的FIFO都为1/4，也就是16*1/4=4个字节
        UARTFIFOLevelSet(UART0_BASE, UART_FIFO_TX1_8,
                         UART_FIFO_RX1_8);

        UARTIntEnable(UART0_BASE, UART_INT_RX);
        UARTIntRegister(UART0_BASE, Uart0IntHandler);
        IntPrioritySet(INT_UART0, 0);
        IntEnable(INT_UART0);
        IntPrioritySet(INT_UART0, 0);
        IntPrioritySet(INT_UART0, 0);
        break;
    case UART_1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinConfigure(GPIO_PB0_U1RX);
        GPIOPinConfigure(GPIO_PB1_U1TX);
        GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
        UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), baudrate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTFIFOEnable(UART1_BASE);
        // 接收发送的FIFO都为1/4，也就是16*1/4=4个字节
        UARTFIFOLevelSet(UART1_BASE, UART_FIFO_TX1_8,
                         UART_FIFO_RX1_8);

        UARTIntEnable(UART1_BASE, UART_INT_RX);
        UARTIntRegister(UART1_BASE, Uart1IntHandler);
        IntPrioritySet(INT_UART1, 0);
        IntEnable(INT_UART1);
        IntPrioritySet(INT_UART1, 0);
        IntPrioritySet(INT_UART1, 0);
        break;
    case UART_6:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinConfigure(GPIO_PD4_U6RX);
        GPIOPinConfigure(GPIO_PD5_U6TX);
        GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_4 | GPIO_PIN_5);
        UARTConfigSetExpClk(UART6_BASE, SysCtlClockGet(), baudrate, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

        UARTFIFOEnable(UART6_BASE);
        // 接收发送的FIFO都为1/4，也就是16*1/4=4个字节
        UARTFIFOLevelSet(UART6_BASE, UART_FIFO_TX1_8,
                         UART_FIFO_RX1_8);

        UARTIntEnable(UART6_BASE, UART_INT_RX);
        UARTIntRegister(UART6_BASE, Uart6IntHandler);
        IntPrioritySet(INT_UART6, 0);
        IntEnable(INT_UART6);
        IntPrioritySet(INT_UART6, 0);
        IntPrioritySet(INT_UART6, 0);

        break;
    }
}

void uart_write_byte(uart_index_t uart_n, uint8_t dat)
{
    while (UARTBusy(UART_BASE[uart_n]))
        ;
    UARTCharPut(UART_BASE[uart_n], dat);
}

void uart_write_buffer(uart_index_t uart_n, uint8_t *buff, uint32_t len)
{
}

void uart_write_string(uart_index_t uart_n, const char *str)
{
}

uint8_t uart_read_byte(uart_index_t uart_n)
{
}

uint8_t uart_quary_byte(uart_index_t uart_n, uint8_t *dat)
{
}

// 系统自带的= =
void ConfigureUART(void)
{
    //
    // Enable the GPIO Peripheral used by the UART.
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    // Enable UART0
    //
    MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    //
    // Configure GPIO Pins for UART mode.
    //
    MAP_GPIOPinConfigure(GPIO_PA0_U0RX);
    MAP_GPIOPinConfigure(GPIO_PA1_U0TX);
    MAP_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Use the internal 16MHz oscillator as the UART clock source.
    //
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);

    //
    // Initialize the UART for console I/O.
    //
    UARTStdioConfig(0, 115200, 16000000);
}