#ifndef __AYA_DEVICE_LCD180_H
#define __AYA_DEVICE_LCD180_H

#include "aya_device_head.h"
#include "aya_driver_head.h"

#define TFT180_SPI_SPEED (10 * 1000 * 1000) // 硬件 SPI 速率
#define TFT180_SPI (SPI_2)                  // 硬件 SPI 号

#define TFT180_RES_PIN (C2) // 液晶复位引脚定义
#define TFT180_DC_PIN (C7)  // 液晶命令位引脚定义
#define TFT180_CS_PIN (B6)  // CS 片选引脚
#define TFT180_BL_PIN (B5)  // 液晶背光引脚定义

#define TFT180_DEFAULT_DISPLAY_DIR (TFT180_CROSSWISE_180) // 默认的显示方向
#define TFT180_DEFAULT_PENCOLOR (0xF5E3)                  // 默认的画笔颜色
#define TFT180_DEFAULT_BGCOLOR (0x0000)                   // 默认的背景颜色
#define TFT180_DEFAULT_DISPLAY_FONT (TFT180_6X8_FONT)     // 默认的字体模式

#define TFT180_DC(x) ((x) ? (gpio_set_level(TFT180_DC_PIN, GPIO_HIGH)) : (gpio_set_level(TFT180_DC_PIN, GPIO_LOW)))
#define TFT180_RST(x) ((x) ? (gpio_set_level(TFT180_RES_PIN, GPIO_HIGH)) : (gpio_set_level(TFT180_RES_PIN, GPIO_LOW)))
#define TFT180_CS(x) ((x) ? (gpio_set_level(TFT180_CS_PIN, GPIO_HIGH)) : (gpio_set_level(TFT180_CS_PIN, GPIO_LOW)))
#define TFT180_BLK(x) ((x) ? (gpio_set_level(TFT180_BL_PIN, GPIO_HIGH)) : (gpio_set_level(TFT180_BL_PIN, GPIO_LOW)))

#define abs(x) (x >= 0) ? (x) : (-x)
#define _max(a, b) (a > b) ? (a) : (b)
#define _min(a, b) (a < b) ? (a) : (b)

typedef enum
{
    TFT180_PORTAIT = 0,       // 竖屏模式
    TFT180_PORTAIT_180 = 1,   // 竖屏模式  旋转180
    TFT180_CROSSWISE = 2,     // 横屏模式
    TFT180_CROSSWISE_180 = 3, // 横屏模式  旋转180
} tft180_dir_enum;

typedef enum
{
    TFT180_6X8_FONT = 0,  // 6x8      字体
    TFT180_8X16_FONT = 1, // 8x16     字体
} tft180_font_size_enum;

extern uint16_t tft180_pencolor;
extern uint16_t tft180_bgcolor;

void tft180_clear(void);
void tft180_fill(const uint16_t color);
void tft180_set_dir(tft180_dir_enum dir);
void tft180_set_font(tft180_font_size_enum font);
void tft180_set_color(const uint16_t pen, const uint16_t bgcolor);
void tft180_draw_point(uint16_t x, uint16_t y, const uint16_t color);
void tft180_draw_line(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color);

void tft180_show_char(uint16_t x, uint16_t y, const char dat);
void tft180_show_string(uint16_t x, uint16_t y, const char dat[]);
void tft180_show_int(uint16_t x, uint16_t y, const int32_t dat, uint8_t num);
void tft180_show_uint(uint16_t x, uint16_t y, const uint32_t dat, uint8_t num);
void tft180_show_float(uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum);

void tft180_show_char_color(uint16_t x, uint16_t y, const char dat, uint16_t colorf, uint16_t colorb);
void tft180_show_string_color(uint16_t x, uint16_t y, const char dat[], uint16_t colorf, uint16_t colorb);
void tft180_show_int_color(uint16_t x, uint16_t y, const int32_t dat, uint8_t num, uint16_t colorf, uint16_t colorb);
void tft180_show_uint_color(uint16_t x, uint16_t y, const uint32_t dat, uint8_t num, uint16_t colorf, uint16_t colorb);
void tft180_show_float_color(uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum, uint16_t colorf, uint16_t colorb);
void tft180_fill_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void tft180_init(void);
#endif