#ifndef __AYA_DEVICE_KEY_H
#define __AYA_DEVICE_KEY_H
#include "aya_device_head.h"
#include "aya_driver_head.h"

#define TCA_9539_ADDR 0x74
#define TCA_9539_I2C I2C_1

extern uint32_t key_cnt[7];

typedef enum
{
    KEY_UP,
    KEY_LEFT,
    KEY_DOWN,
    KEY_RIGHT,
    KEY_CENTER,
    KEY_SW1,
    KEY_SW2,
} key_enum_t;

typedef enum
{
    LED_EMPTY,
    LED_RED,
    LED_GREEN,
    LED_YELLOW,
    LED_BLUE,
    LED_PURPLE,
    LED_CYAN,
    LED_WHITE,
} rgb_color_t;

typedef struct
{
    rgb_color_t LED1, LED2, LED3;
} rgb_bsp_t;



void tca9539_init();
void key_handler();
uint8_t key_read(key_enum_t k);
void tca9539_led_apply();
void led_setColor(uint8_t led_n, rgb_color_t color);
#endif