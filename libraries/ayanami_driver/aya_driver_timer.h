/*
 * aya_driver_timer.h
 *
 *  Created on: 2023Äê6ÔÂ1ÈÕ
 *      Author: 13164
 */

#ifndef AYA_DRIVER_TIMER_H_
#define AYA_DRIVER_TIMER_H_

#include <aya_driver_head.h>
#include <aya_device_head.h>

typedef enum
{
    timer_0,
    timer_1,
    timer_2,
    timer_3,
    timer_4,
    timer_5,
} timer_index_t;

typedef struct
{
    uint32_t base;

    uint8_t oneshot;
    bool rtc;
} timer_inst_t;

void timer_loop_init(timer_index_t index, uint32_t period_us);
void timer_once_init(timer_index_t index, uint32_t period_us);

#endif /* AYA_DRIVER_TIMER_H_ */
