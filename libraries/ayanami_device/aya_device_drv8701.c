#include "aya_device_drv8701.h"

#define _LIMIT(a, b, c) (a < b) ? (b) : ((a > c) ? (c) : (a))

drv8701_t drv_8701_ins =
    {
        .leftReverse = 0,
        .rightReverse = 1,
};

void drv8701_init(void)
{
    pwm_init(DRV8701_LEFT_PWM, 10000, 100);
    pwm_init(DRV8701_RIGHT_PWM, 10000, 100);
    gpio_init(DRV8701_LEFT_PH, GPO, GPIO_LOW, GPO_PP);
    gpio_init(DRV8701_RIGHT_PH, GPO, GPIO_LOW, GPO_PP);
}

void drv8701_apply(void)
{
    volatile uint8_t sig_l = 0, sig_r = 0;
    volatile uint32_t spd_l, spd_r;

    if (drv_8701_ins.idle == 0)
    {
        pwm_set_duty(DRV8701_LEFT_PWM, 100);
        pwm_set_duty(DRV8701_RIGHT_PWM, 100);
        return;
    }

    if (drv_8701_ins.leftSpeed < 0)
        sig_l = 1, spd_l = -drv_8701_ins.leftSpeed;
    else
        sig_l = 0, spd_l = drv_8701_ins.leftSpeed;
    if (drv_8701_ins.rightSpeed < 0)
        sig_r = 1, spd_r = -drv_8701_ins.rightSpeed;
    else
        sig_r = 0, spd_r = drv_8701_ins.rightSpeed;

    // if (spd_l <= 100)
    //     spd_l = 100;
    // if (spd_r <= 100)
    //     spd_r = 100;
    pwm_set_duty(DRV8701_LEFT_PWM, spd_l);
    pwm_set_duty(DRV8701_RIGHT_PWM, spd_r);

    if (drv_8701_ins.leftReverse)
        gpio_set_level(DRV8701_LEFT_PH, sig_l);
    else
        gpio_set_level(DRV8701_LEFT_PH, 1 - sig_l);

    if (drv_8701_ins.rightReverse)
        gpio_set_level(DRV8701_RIGHT_PH, sig_r);
    else
        gpio_set_level(DRV8701_RIGHT_PH, 1 - sig_r);
}

void drv8701_setSpeedLeft(int32_t speedL)
{
    _LIMIT(speedL, -9999, 9999);
    drv_8701_ins.leftSpeed = speedL;
}
void drv8701_setSpeedRight(int32_t speedR)
{
    _LIMIT(speedR, -9999, 9999);
    drv_8701_ins.rightSpeed = speedR;
}

void drv8701_setSpeed(int32_t speedL, int32_t speedR)
{
    _LIMIT(speedL, -9999, 9999);
    _LIMIT(speedR, -9999, 9999);
    drv_8701_ins.leftSpeed = speedL;
    drv_8701_ins.rightSpeed = speedR;
}
void drv8701_enable(void)
{
    drv_8701_ins.idle = 1;
}

void drv8701_disable(void)
{
    drv_8701_ins.idle = 0;
}