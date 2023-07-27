#ifndef __AYA_DRIVER_PWM_H
#define __AYA_DRIVER_PWM_H
#include "aya_driver_head.h"

typedef enum
{
    pwm_motor1,
    pwm_motor2,
    pwm_motor3,
    pwm_motor4,
    pwm_servo1,
    pwm_servo2,
    pwm_servo3
} pwm_ch_t;

void pwm_init(pwm_ch_t ch);
void pwm_set_freq(pwm_ch_t ch, uint32_t freq_hz);
void pwm_set_duty(pwm_ch_t ch, uint32_t duty_div_10k);

#endif