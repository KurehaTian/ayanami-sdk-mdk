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

    ConfigureUART();

    drv8701_init();
    // drv8701_enable();
    motor_init();
    tft180_init();
    infrared_init();

    // gpio_init(F1, GPO, GPIO_HIGH, GPO_PP);
    // gpio_init(F2, GPO, GPIO_HIGH, GPO_PP);
    // gpio_init(F3, GPO, GPIO_HIGH, GPO_PP);
    // bmi088_init();
    // infrared_init();
    // timer_loop_init(timer_0, 1000000);
    // timer_loop_init(timer_1, 2000000);
    timer_loop_init(timer_2, 5000);
    timer_loop_init(timer_3, 10000);

    int t = 0;
    int32_t s1, s2, s3;
    float f1, f2, f3;
    tca9539_init();
    uint8_t buf[3];
    int lt = 0;

    // encoder_init(encoder_0, 4096 * 4 - 1, encoder_dir_AB);
    // encoder_init(encoder_1, 4096 * 4 - 1, encoder_dir_BA);
    // encoder_set_period(encoder_0, 20);
    // encoder_set_period(encoder_1, 20);
    ui_init();
    drv8701_enable();
    // gpio_init(E5,GPO,GPIO_HIGH,GPO_PP);
    while (1)
    {

        infrared_read();
        float err = CalcPosiPidOut(&steering, 0, infrared_err_moment_exp());
        motor_set_speed(motor.speed_center * (1 - err / 250.0), motor.speed_center * (1 + err / 250.0));

        ui_handler();

        systick_delay_ms(20);
        lt++;
        lt = lt % 512;
        led_setColor(1, lt / 64);
        led_setColor(2, (lt / 8) % 8);
        led_setColor(3, lt % 8);
        tca9539_led_apply();
        // uart_write_byte(UART_0,'C');
        //  buf[0] = i2c_read_reg(I2C_1, 0x74, 0x00);
        UARTprintf("%d,%d\n", motor.speed[0], motor.speed[1]);
        t += 1;
        if (t >= 100)
            t = 0;

        f1 = sin((t + 0) / 100.0 * 2 * 3.1415926);
        // f1 = (f1 + 1) / 2;
        f2 = sin((t + 33.3) / 100.0 * 2 * 3.1415926);
        // f2 = (f2 + 1) / 2;

        s1 = f1 * 500;
        s2 = f2 * 500;

        // s1 = pow(1.1, f1 * 30) / pow(1.1, 30) * 10000.0;
        // s2 = pow(1.3, f2 * 30) / pow(1.3, 30) * 10000.0;
        // drv8701_setSpeed(0, 0);
        // drv8701_apply();
        // gpio_toggle_level(F1);
        //  pwm_set_duty(pwm_motor4, prd);
        //  pwm_set_duty(pwm_servo1, prd);
        //  pwm_set_duty(pwm_servo2, prd);
        //  pwm_set_duty(pwm_servo3, prd);
        // UARTprintf("P0=%x\n", buf[0]);
        // systick_delay_ms(2);
        //  UARTprintf("%d\n",s1);

        // UARTprintf("%d,%d,%d,%d\n", encoder_read_speed(encoder_0), encoder_read_speed(encoder_1),drv_8701_ins.leftSpeed,drv_8701_ins.rightSpeed);
    }
    return 0;
}
