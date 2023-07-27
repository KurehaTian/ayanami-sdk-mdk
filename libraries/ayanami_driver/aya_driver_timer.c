/*
 * aya_driver_timer.c
 *
 *  Created on: 2023Äê6ÔÂ1ÈÕ
 *      Author: 13164
 */
#include "aya_driver_timer.h"

void timer_loop_init(timer_index_t index, uint32_t period_us)
{
    switch (index)
    {
    case timer_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER0_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER0A);
        IntMasterEnable();
        TimerEnable(TIMER0_BASE, TIMER_A);
        break;

    case timer_1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER1_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1IntHandler);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER1A);
        IntMasterEnable();
        TimerEnable(TIMER1_BASE, TIMER_A);
        break;

    case timer_2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER2_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER2_BASE, TIMER_A, Timer2IntHandler);
        TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER2A);
        IntMasterEnable();
        TimerEnable(TIMER2_BASE, TIMER_A);
        break;

    case timer_3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER3_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER3_BASE, TIMER_A, Timer3IntHandler);
        TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER3A);
        IntMasterEnable();
        TimerEnable(TIMER3_BASE, TIMER_A);
        break;

    case timer_4:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        TimerConfigure(TIMER4_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER4_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER4_BASE, TIMER_A, Timer4IntHandler);
        TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER4A);
        IntMasterEnable();
        TimerEnable(TIMER4_BASE, TIMER_A);
        break;

    case timer_5:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
        TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
        TimerLoadSet(TIMER5_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER5_BASE, TIMER_A, Timer5IntHandler);
        TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER5A);
        IntMasterEnable();
        TimerEnable(TIMER5_BASE, TIMER_A);
        break;
    }
}

void timer_once_init(timer_index_t index, uint32_t period_us)
{
    switch (index)
    {
    case timer_0:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
        TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER0_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
        TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER0A);
        IntMasterEnable();
        TimerEnable(TIMER0_BASE, TIMER_A);
        break;

    case timer_1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
        TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER1_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1IntHandler);
        TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER1A);
        IntMasterEnable();
        TimerEnable(TIMER1_BASE, TIMER_A);
        break;

    case timer_2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
        TimerConfigure(TIMER2_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER2_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER2_BASE, TIMER_A, Timer2IntHandler);
        TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER2A);
        IntMasterEnable();
        TimerEnable(TIMER2_BASE, TIMER_A);
        break;

    case timer_3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
        TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER3_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER3_BASE, TIMER_A, Timer3IntHandler);
        TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER3A);
        IntMasterEnable();
        TimerEnable(TIMER3_BASE, TIMER_A);
        break;

    case timer_4:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
        TimerConfigure(TIMER4_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER4_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER4_BASE, TIMER_A, Timer4IntHandler);
        TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER4A);
        IntMasterEnable();
        TimerEnable(TIMER4_BASE, TIMER_A);
        break;

    case timer_5:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
        TimerConfigure(TIMER5_BASE, TIMER_CFG_ONE_SHOT);
        TimerLoadSet(TIMER5_BASE, TIMER_A, MAP_SysCtlClockGet() / 1000000 * period_us - 1);
        TimerIntRegister(TIMER5_BASE, TIMER_A, Timer5IntHandler);
        TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);
        IntEnable(INT_TIMER5A);
        IntMasterEnable();
        TimerEnable(TIMER5_BASE, TIMER_A);
        break;
    }
}
