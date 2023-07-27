#ifndef __AYA_DEVICE_INFRARED_H
#define __AYA_DEVICE_INFRARED_H
#include "aya_device_head.h"
#include "aya_driver_head.h"


#define INFRARED_PIN_DATA	B2
#define INFRARED_PIN_CLK	B3
#define INFRARED_PIN_SH		C3

void infrared_init(void);
void infrared_read(void);
float infrared_err_moment1(void);

#endif
