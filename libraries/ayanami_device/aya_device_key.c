#include "aya_device_key.h"

rgb_bsp_t rgb_bsp;

uint8_t raw_key_read[7];

uint8_t key_event[7];
uint32_t key_cnt[7];

#define KEY_LONG_PRESS 100

void tca9539_init()
{
    uint8_t buf[10];
    // 初始化I2C外设
    i2c_init(TCA_9539_I2C);

    // 配置输入输出模式
    // i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x06, 0x80);
    // i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x07, 0xff);

    i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x06, 0x7f);
    i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x07, 0x00);
}

void tca9539_key_scan()
{
    uint8_t buf[3];
    buf[0] = i2c_read_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x00);
    buf[1] = i2c_read_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x01);

    for (uint8_t i = 0x01, j = 0; j < 7; i <<= 1, j++)
    {
        raw_key_read[j] = !(buf[0] & i);
    }
}

void key_handler()
{
    tca9539_key_scan();
    for (int i = KEY_UP; i <= KEY_SW2; i++)
    {
        if (raw_key_read[i] == 0)
        {
            // 按键抬起时
            // key_event[i] = 0;
            key_cnt[i] = 0;
        }
        else
        {
// 按键按下时
#ifdef KEY_LONG_PRESS
            UARTprintf("Key %d pressed,Eve= %d,Cnt=%d\n", i, key_event[i], key_cnt[i]);
            if (key_cnt[i] == 1 || key_cnt[i] > KEY_LONG_PRESS)
#else
            if (key_cnt[i] == 1)
#endif
                key_event[i] = 1;
            // else
            //     key_event[i] = 0;
            key_cnt[i]++;
        }
    }
}

uint8_t key_read(key_enum_t k)
{
    uint8_t res = key_event[k];
    if (res)
    {
        key_event[k] = 0;
    }
    return res;
}

void tca9539_led_apply()
{
    uint8_t buf[2] = {0x00, 0x00};
    // 1R
    if (0x01 & rgb_bsp.LED1)
        buf[0] = 0x00;
    else
        buf[0] = 0x80;
    // 1G
    if (0x02 & rgb_bsp.LED1)
        buf[1] |= 0x01;
    // 1B
    if (0x04 & rgb_bsp.LED1)
        buf[1] |= 0x02;
    // 2R
    if (0x01 & rgb_bsp.LED2)
        buf[1] |= 0x04;
    // 2G
    if (0x02 & rgb_bsp.LED2)
        buf[1] |= 0x08;
    // 2B
    if (0x04 & rgb_bsp.LED2)
        buf[1] |= 0x10;
    // 3R
    if (0x01 & rgb_bsp.LED3)
        buf[1] |= 0x20;
    // 3G
    if (0x02 & rgb_bsp.LED3)
        buf[1] |= 0x40;
    // 3B
    if (0x04 & rgb_bsp.LED3)
        buf[1] |= 0x80;
    buf[1] = ~buf[1];

    i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x02, buf[0]);
    i2c_write_reg(TCA_9539_I2C, TCA_9539_ADDR, 0x03, buf[1]);
}

void led_setColor(uint8_t led_n, rgb_color_t color)
{
    // 丝印跟网络标反了= =
    if (led_n == 3)
        rgb_bsp.LED1 = color;
    if (led_n == 2)
        rgb_bsp.LED2 = color;
    if (led_n == 1)
        rgb_bsp.LED3 = color;
}