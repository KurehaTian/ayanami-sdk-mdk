#ifndef __AYA_DRIVER_SYSTICK_H
#define __AYA_DRIVER_SYSTICK_H
#include "aya_driver_head.h"

void systick_delay_ms(uint32_t ms);
void systick_delay_us(uint32_t us);
void SystickIntHandler(void);

#endif
