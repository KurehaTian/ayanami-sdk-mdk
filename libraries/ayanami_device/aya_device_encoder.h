#ifndef __AYA_DEVICE_ENCODER_H
#define __AYA_DEVICE_ENCODER_H

#include "aya_device_head.h"
#include "aya_driver_head.h"

typedef enum
{
    encoder_0,
    encoder_1
}encoder_index_t;

typedef enum
{
    encoder_dir_AB,
    encoder_dir_BA
}encoder_dir_t;

typedef enum
{
    encoder_mode_velocity,
    encoder_mode_position,
}encoder_mode_t;


void encoder_init(encoder_index_t encoder_n, uint32_t max_position, encoder_dir_t dir);
void encoder_set_period(encoder_index_t encoder_n, uint32_t period_ms);

#endif