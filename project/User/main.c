/**
 * main.c
 */
#include <aya_driver_head.h>
#include <aya_device_head.h>

#include "isr.h"

void delay()
{
    volatile uint32_t ui32Loop;
    for (ui32Loop = 0; ui32Loop < 8000000; ui32Loop++)
    {
    }
}

int main(void)
{
    MAP_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

    FPUEnable();
    FPULazyStackingEnable();

    // ConfigureUART();

    // gpio_init(F1, GPO, GPIO_HIGH, GPO_PP);
    // gpio_init(F2, GPO, GPIO_HIGH, GPO_PP);
    // gpio_init(F3, GPO, GPIO_HIGH, GPO_PP);
    // bmi088_init();
    // infrared_init();
    // timer_loop_init(timer_0, 1000000);
    // timer_loop_init(timer_1, 2000000);
    // timer_loop_init(timer_2, 4000000);
    timer_loop_init(timer_3, 100000);
    uart_init(UART_6, 115200);
    // drv8701_init();
    // drv8701_enable();
    int t = 0;
    uint32_t s1, s2, s3;
    float f1, f2, f3;
    //tca9539_init();
    uint8_t buf[3];
    int lt = 0;
    // gpio_init(E5,GPO,GPIO_HIGH,GPO_PP);
    while (1)
    {
        systick_delay_ms(100);
        lt++;
        lt = lt % 512;
        led_setColor(1, lt / 64);
        led_setColor(2, (lt / 8) % 8);
        led_setColor(3, lt % 8);
        //tca9539_led_apply();
        //uart_write_byte(UART_0,'C');
        // buf[0] = i2c_read_reg(I2C_1, 0x74, 0x00);

        // t += 1;
        // if (t >= 1000)
        //     t = 0;
        // gpio_set_level(E5,GPIO_LOW);

        // f1 = sin((t + 0) / 1000.0 * 2 * 3.1415926);
        // f1 = (f1 + 1) / 2;
        // f2 = sin((t + 333) / 1000.0 * 2 * 3.1415926);
        // f2 = (f2 + 1) / 2;
        // f3 = sin((t + 667) / 1000.0 * 2 * 3.1415926);
        // f3 = (f3 + 1) / 2;

        // s1 = pow(1.1, f1 * 30) / pow(1.1, 30) * 10000.0;
        // s2 = pow(1.3, f2 * 30) / pow(1.3, 30) * 10000.0;
        // s3 = pow(1.3, f3 * 30) / pow(1.3, 30) * 10000.0;

        // gpio_toggle_level(F1);
        //  pwm_set_duty(pwm_motor4, prd);
        //  pwm_set_duty(pwm_servo1, prd);
        //  pwm_set_duty(pwm_servo2, prd);
        //  pwm_set_duty(pwm_servo3, prd);
        // UARTprintf("P0=%x\n", buf[0]);
        // systick_delay_ms(2);
        //  UARTprintf("%d\n",s1);
    }
    return 0;
}
