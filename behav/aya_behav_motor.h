#ifndef __AYA_BEHAV_MOTOR_H
#define __AYA_BEHAV_MOTOR_H
#include "aya_driver_head.h"
#include "aya_device_head.h"

typedef struct
{
    InrcPidNode pid_left_wheel, pid_right_wheel;
    int32_t l_pwm, r_pwm;

    int32_t speed[2];
    int32_t speed_exp[2];
    int32_t speed_center;

} motor_t;

extern motor_t motor;
extern PosiPidNode steering;

void motor_init();
void motor_set_speed(int32_t l, int32_t r);
#endif