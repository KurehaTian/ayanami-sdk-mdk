#ifndef __AYA_DEVICE_DRV8701_H
#define __AYA_DEVICE_DRV8701_H
#include "aya_device_head.h"
#include "aya_driver_head.h"

typedef struct
{
    uint8_t idle;
    uint8_t leftReverse, rightReverse;
    int32_t leftSpeed, rightSpeed;
} drv8701_t;
extern drv8701_t drv_8701_ins;

#define DRV8701_LEFT_PWM pwm_motor2
#define DRV8701_RIGHT_PWM pwm_motor4

#define DRV8701_LEFT_PH E5
#define DRV8701_RIGHT_PH F2

void drv8701_init(void);
void drv8701_apply(void);
void drv8701_setSpeedLeft(int32_t speedL);
void drv8701_setSpeedRight(int32_t speedR);
void drv8701_setSpeed(int32_t speedL, int32_t speedR);
void drv8701_enable(void);
void drv8701_disable(void);

#endif