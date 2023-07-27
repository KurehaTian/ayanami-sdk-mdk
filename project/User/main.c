/**
 * main.c
 */
#include <aya_driver_head.h>
#include <aya_device_head.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "isr.h"
void delay()
{
    volatile uint32_t ui32Loop;
    for (ui32Loop = 0; ui32Loop < 8000000; ui32Loop++)
    {
    }
}

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

int main(void)
{
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    FPUEnable();
    FPULazyStackingEnable();

    ConfigureUART();

    gpio_init(F1, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(F2, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(F3, GPO, GPIO_HIGH, GPO_PP);
    // bmi088_init();
    // infrared_init();
    timer_loop_init(timer_0, 1000000);
    timer_loop_init(timer_1, 2000000);
    timer_loop_init(timer_2, 4000000);
    timer_loop_init(timer_3, 10000);
    tft180_init();
    encoder_init(encoder_0, 4096 * 4 - 1, encoder_dir_AB);
    encoder_set_period(encoder_0,5);
    while (1)                                                                                                                                          
    {
        UARTprintf("encoder dir=%d\n", QEIDirectionGet(QEI0_BASE));
    }
    return 0;
}
