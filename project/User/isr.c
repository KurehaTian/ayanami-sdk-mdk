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
    key_handler();
    //UARTprintf("KEY:\tUP\tLF\tRT\tDN\tCT\tS1\tS2\n");
    //UARTprintf("Prs:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", key_read(0), key_read(1), key_read(2), key_read(3), key_read(4), key_read(5), key_read(6));
    //UARTprintf("Tim:\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", key_cnt[0], key_cnt[1], key_cnt[2], key_cnt[3], key_cnt[4], key_cnt[5], key_cnt[6]);
}

void Timer4IntHandler(void)
{
    MAP_TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
}

void Timer5IntHandler(void)
{
    MAP_TimerIntClear(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
}
