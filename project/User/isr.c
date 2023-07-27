/*
 * isr.c
 *
 *  Created on: 2023Äê6ÔÂ1ÈÕ
 *      Author: 13164
 */
#include "isr.h"
int fps = 0;

void Timer0IntHandler(void)
{
    MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    gpio_toggle_level(F1);
    fps = 0;
}

void Timer1IntHandler(void)
{
    MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    gpio_toggle_level(F2);
}

void Timer2IntHandler(void)
{
    MAP_TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
    gpio_toggle_level(F3);
}

void Timer3IntHandler(void)
{
    MAP_TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    // bmi088_read_data();
    // bmi088_bsx_calc();

    //		char s[100];
    //		sprintf(s, "ROLL=%.2f\tYAW=%.2f\tPIT=%.2f\n\n",bsx_out_data.orientation.roll * 180.0 / 3.1415926535, bsx_out_data.orientation.yaw * 180.0 / 3.1415926535,bsx_out_data.orientation.pitch * 180.0 / 3.1415926535);
    //		UARTprintf("%s",s);
}

void Timer4IntHandler(void)
{
    MAP_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer5IntHandler(void)
{
    MAP_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
}
