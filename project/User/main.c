/**
 * main.c
 */
#include <aya_driver_head.h>
#include <aya_device_head.h>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
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
    pwm_init(pwm_motor2, 1000, 1000);
    pwm_init(pwm_motor3, 1000, 1000);
    pwm_init(pwm_motor4, 1000, 1000);
    int t = 0;
    uint32_t s1, s2, s3;
    float f1, f2, f3;

    while (1)
    {
        // prd = 1000 - prd;
        // pwm_set_duty(pwm_motor1, prd);
        t += 1;
        if (t >= 3000)
            t = 0;
        systick_delay_ms(1);
        f1 = sin(t / 3000.0 *2* 3.1415926);
        f1 = (f1 + 1) / 2;
        f2 = sin(t / 1500.0 *2* 3.1415926);
        f2 = (f2 + 1) / 2;
        f3 = sin(t / 750.0 *2* 3.1415926);
        f3 = (f3 + 1) / 2;

        s1 = pow(1.35, f1 * 30);
        s2 = pow(1.35, f2 * 30);
        s3 = pow(1.35, f3 * 30);
        pwm_set_duty(pwm_motor2, s1);
        pwm_set_duty(pwm_motor3, s2);
        pwm_set_duty(pwm_motor4, s3);
        // gpio_toggle_level(F1);
        //  pwm_set_duty(pwm_motor4, prd);
        //  pwm_set_duty(pwm_servo1, prd);
        //  pwm_set_duty(pwm_servo2, prd);
        //  pwm_set_duty(pwm_servo3, prd);

        // UARTprintf("t=%d %d %d %d\n", t,s1,s2,s3);
    }
    return 0;
}
