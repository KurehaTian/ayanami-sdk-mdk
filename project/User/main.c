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
    // timer_loop_init(timer_0, 1000000);
    // timer_loop_init(timer_1, 2000000);
    // timer_loop_init(timer_2, 4000000);
    // timer_loop_init(timer_3, 10000);
    pwm_init(pwm_motor1, 333, 1000);
    pwm_init(pwm_motor2, 333, 1000);
    pwm_init(pwm_motor3, 333, 1000);
    pwm_init(pwm_motor4, 333, 1000);
    pwm_init(pwm_servo1, 333, 1000);
    pwm_init(pwm_servo2, 333, 1000);
    pwm_init(pwm_servo3, 333, 1000);
    int prd = 1666;
    int step = 333;
    while (1)
    {
        // prd = 1000 - prd;
        // pwm_set_duty(pwm_motor1, prd);
        delay();
        pwm_set_duty(pwm_servo1, 2000);
        delay();
        pwm_set_duty(pwm_servo1, 8000);
        // pwm_set_duty(pwm_motor1, prd);
        // pwm_set_duty(pwm_motor2, prd);
        // pwm_set_duty(pwm_motor3, prd);
        // pwm_set_duty(pwm_motor4, prd);
        // pwm_set_duty(pwm_servo1, prd);
        // pwm_set_duty(pwm_servo2, prd);
        // pwm_set_duty(pwm_servo3, prd);

        // prd += step;
        // if (prd < 1666)
        //     prd = 1666, step = 333;
        // if (prd > 8333)
        //     prd = 8333, step = -333;

        // UARTprintf("encoder SPD=%d\n",encoder_read_speed(encoder_0));
    }
    return 0;
}
