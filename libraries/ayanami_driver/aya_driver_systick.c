#include "aya_driver_systick.h"
#include "aya_device_head.h"

volatile uint32_t systick_it_count = 0;

void systick_delay_ms(uint32_t ms)
{
    systick_it_count = 0;
    volatile uint32_t p = 0;
    SysTickPeriodSet(SysCtlClockGet() / 1000);
    SysTickIntRegister(SystickIntHandler);
    SysTickIntEnable();
    SysTickEnable();
    while (systick_it_count < ms);

    SysTickIntDisable();
    SysTickDisable();
}

void systick_delay_us(uint32_t us)
{
    systick_it_count = 0;
    volatile uint32_t p = 0;
    SysTickPeriodSet(SysCtlClockGet() / 1000);
    SysTickIntRegister(SystickIntHandler);
    SysTickIntEnable();
    SysTickEnable(); 
    while (systick_it_count < us);

    SysTickIntDisable();
    SysTickDisable();
}

void SystickIntHandler(void)
{
    systick_it_count++;
}