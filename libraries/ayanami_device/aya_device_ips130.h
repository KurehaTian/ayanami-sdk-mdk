#ifndef __AYA_DEVICE_IPS130_H
#define __AYA_DEVICE_IPS130_H

#include "aya_device_head.h"
#include "aya_driver_head.h"

#define IPS130_SPI SPI_2
#define IPS130_PIN_RES A4
#define IPS130_PIN_DC  A2
#define IPS130_PIN_BLK A3

//0- ˙∆¡  1- ˙∆¡ 2-∫·∆¡ 3-∫·∆¡
#define IPS130_DIRECTION 3
#define IPS130_W 240
#define IPS130_H 240


void IPS130_init();

void IPS130_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);


#endif