#include "aya_behav_motor.h"


motor_t motor =
    {
        .speed_center = 250,
};

#define _LIMIT(a, b, c) (a < b) ? (b) : ((a > c) ? (c) : (a))
PosiPidNode steering;

void motor_init()
{
    drv8701_init();
    encoder_init(encoder_0, 4096 * 4 - 1, encoder_dir_AB);
    encoder_init(encoder_1, 4096 * 4 - 1, encoder_dir_BA);
    encoder_set_period(encoder_0, 5);
    encoder_set_period(encoder_1, 5);

    SetInrcPidParm(&motor.pid_left_wheel, 4, 1.0, 1.0);
    motor.pid_left_wheel.limit_out_abs = 9999;
    motor.pid_left_wheel.limit_integral_abs = 2000;

    SetInrcPidParm(&motor.pid_right_wheel, 4, 1.0, 1.0);
    motor.pid_right_wheel.limit_out_abs = 9999;
    motor.pid_right_wheel.limit_integral_abs = 2000;

    SetPosiPidParm(&steering, 1.2, 0, 0.1);
    steering.limit_out_abs = 9999;
}

void motor_control()
{
    if (drv_8701_ins.idle == 0)
    {
        motor.l_pwm = 0;
        motor.r_pwm = 0;
        drv8701_setSpeed(motor.l_pwm, motor.r_pwm);
        drv8701_apply();
        return;
    }
    motor.speed[0] = encoder_read_speed(encoder_0);
    motor.speed[1] = encoder_read_speed(encoder_1);
    motor.l_pwm += CalcInrcPidOut(&motor.pid_left_wheel, motor.speed_exp[0], motor.speed[0]);
    motor.r_pwm += CalcInrcPidOut(&motor.pid_right_wheel, motor.speed_exp[1], motor.speed[1]);
    motor.l_pwm = _LIMIT(motor.l_pwm, -9999, 9999);
    motor.r_pwm = _LIMIT(motor.r_pwm, -9999, 9999);
    drv8701_setSpeed(motor.l_pwm, motor.r_pwm);
    drv8701_apply();
}

void motor_set_speed(int32_t l, int32_t r)
{
    motor.speed_exp[0] = l;
    motor.speed_exp[1] = r;
}