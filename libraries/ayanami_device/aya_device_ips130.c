#include "aya_device_ips130.h"



static void delay_ms(int ms)
{
    systick_delay_ms(ms);
}

static void delay_us(int us)
{
    systick_delay_us(us);
}

static void IPS130_WR_BUS_8(const uint8_t dat)
{
    uint8_t tx = dat;
    //SSIDataPut(SSI2_BASE,dat);
    spi_write(IPS130_SPI, &tx, 1);
    // for (int8_t i = 0; i < 8; i++)
    // {
    //     //delay_us(10);
    //     gpio_set_level(B4, GPIO_LOW);
    //     if (tx & 0x80)
    //     {
    //         gpio_set_level(B7, GPIO_HIGH);
    //     }
    //     else
    //     {
    //         gpio_set_level(B7, GPIO_LOW);
    //     }
    //     gpio_set_level(B4, GPIO_HIGH);
    //     tx <<= 1;
    // }
}

static void IPS130_WR_DAT_8(const uint8_t dat)
{
    gpio_set_level(IPS130_PIN_DC, GPIO_HIGH);
    IPS130_WR_BUS_8(dat);
}

static void IPS130_WR_DAT_16(const uint16_t dat)
{
    gpio_set_level(IPS130_PIN_DC, GPIO_HIGH);
    IPS130_WR_BUS_8(dat >> 8);
    IPS130_WR_BUS_8(dat & 0xff);
}

static void IPS130_WR_CMD_8(const uint8_t dat)
{
    gpio_set_level(IPS130_PIN_DC, GPIO_LOW);
    IPS130_WR_BUS_8(dat);
    gpio_set_level(IPS130_PIN_DC, GPIO_HIGH);
}

static void IPS130_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    switch (IPS130_DIRECTION)
    {
    case 0:
        IPS130_WR_CMD_8(0x2A); // 列地址设置
        IPS130_WR_DAT_16(x1);
        IPS130_WR_DAT_16(x2);
        IPS130_WR_CMD_8(0x2B); // 行地址设置
        IPS130_WR_DAT_16(y1);
        IPS130_WR_DAT_16(y2);
        IPS130_WR_CMD_8(0x2C); // 储存器写
        break;
    case 1:
        IPS130_WR_CMD_8(0x2A); // 列地址设置
        IPS130_WR_DAT_16(x1);
        IPS130_WR_DAT_16(x2);
        IPS130_WR_CMD_8(0x2B); // 行地址设置
        IPS130_WR_DAT_16(y1 + 80);
        IPS130_WR_DAT_16(y2 + 80);
        IPS130_WR_CMD_8(0x2C); // 储存器写
        break;
    case 2:
        IPS130_WR_CMD_8(0x2A); // 列地址设置
        IPS130_WR_DAT_16(x1);
        IPS130_WR_DAT_16(x2);
        IPS130_WR_CMD_8(0x2B); // 行地址设置
        IPS130_WR_DAT_16(y1);
        IPS130_WR_DAT_16(y2);
        IPS130_WR_CMD_8(0x2C); // 储存器写
        break;
    case 3:
        IPS130_WR_CMD_8(0x2A); // 列地址设置
        IPS130_WR_DAT_16(x1 + 80);
        IPS130_WR_DAT_16(x2 + 80);
        IPS130_WR_CMD_8(0x2B); // 行地址设置
        IPS130_WR_DAT_16(y1);
        IPS130_WR_DAT_16(y2);
        IPS130_WR_CMD_8(0x2C); // 储存器写
        break;
    }
}

void ips130_init()
{
    // 初始化外设
    spi_init(IPS130_SPI, SPI_MODE_3, 6000000);
    //gpio_init(B4, GPO, GPIO_HIGH, GPO_PP);
    //gpio_init(B7, GPO, GPIO_HIGH, GPO_PP);

    gpio_init(IPS130_PIN_RES, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(IPS130_PIN_DC, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(IPS130_PIN_BLK, GPO, GPIO_HIGH, GPO_PP);

    // 复位屏幕
    gpio_set_level(IPS130_PIN_RES, GPIO_LOW);
    delay_ms(30);
    gpio_set_level(IPS130_PIN_RES, GPIO_HIGH);
    delay_ms(30);

    // 打开背光
    gpio_set_level(IPS130_PIN_BLK, GPIO_HIGH);
    delay_ms(50);
    // IPS130_WR_CMD_8(0x11); // 退出睡眠
    // delay_ms(120);

    IPS130_WR_CMD_8(0x36); // 配置屏幕方向
    
    switch (IPS130_DIRECTION)
    {
    case 0:
        IPS130_WR_DAT_8(0x00);
        break;
    case 1:
        IPS130_WR_DAT_8(0xC0);
        break;
    case 2:
        IPS130_WR_DAT_8(0x70);
        break;
    case 3:
        IPS130_WR_DAT_8(0xA0);
        break;
    }
    UARTprintf("Direction.\n");
    IPS130_WR_CMD_8(0x3A);
    IPS130_WR_DAT_8(0x05);

    IPS130_WR_CMD_8(0xB2);
    IPS130_WR_DAT_8(0x0C);
    IPS130_WR_DAT_8(0x0C);
    IPS130_WR_DAT_8(0x00);
    IPS130_WR_DAT_8(0x33);
    IPS130_WR_DAT_8(0x33);

    IPS130_WR_CMD_8(0xB7);
    IPS130_WR_DAT_8(0x35);

    IPS130_WR_CMD_8(0xBB);
    IPS130_WR_DAT_8(0x19);

    IPS130_WR_CMD_8(0xC0);
    IPS130_WR_DAT_8(0x2C);

    IPS130_WR_CMD_8(0xC2);
    IPS130_WR_DAT_8(0x01);

    IPS130_WR_CMD_8(0xC3);
    IPS130_WR_DAT_8(0x12);

    IPS130_WR_CMD_8(0xC4);
    IPS130_WR_DAT_8(0x20);

    IPS130_WR_CMD_8(0xC6);
    IPS130_WR_DAT_8(0x0F);

    IPS130_WR_CMD_8(0xD0);
    IPS130_WR_DAT_8(0xA4);
    IPS130_WR_DAT_8(0xA1);

    IPS130_WR_CMD_8(0xE0);
    IPS130_WR_DAT_8(0xD0);
    IPS130_WR_DAT_8(0x04);
    IPS130_WR_DAT_8(0x0D);
    IPS130_WR_DAT_8(0x11);
    IPS130_WR_DAT_8(0x13);
    IPS130_WR_DAT_8(0x2B);
    IPS130_WR_DAT_8(0x3F);
    IPS130_WR_DAT_8(0x54);
    IPS130_WR_DAT_8(0x4C);
    IPS130_WR_DAT_8(0x18);
    IPS130_WR_DAT_8(0x0D);
    IPS130_WR_DAT_8(0x0B);
    IPS130_WR_DAT_8(0x1F);
    IPS130_WR_DAT_8(0x23);

    IPS130_WR_CMD_8(0xE1);
    IPS130_WR_DAT_8(0xD0);
    IPS130_WR_DAT_8(0x04);
    IPS130_WR_DAT_8(0x0C);
    IPS130_WR_DAT_8(0x11);
    IPS130_WR_DAT_8(0x13);
    IPS130_WR_DAT_8(0x2C);
    IPS130_WR_DAT_8(0x3F);
    IPS130_WR_DAT_8(0x44);
    IPS130_WR_DAT_8(0x51);
    IPS130_WR_DAT_8(0x2F);
    IPS130_WR_DAT_8(0x1F);
    IPS130_WR_DAT_8(0x1F);
    IPS130_WR_DAT_8(0x20);
    IPS130_WR_DAT_8(0x23);
    IPS130_WR_CMD_8(0x21);
    IPS130_WR_CMD_8(0x11); // 退出睡眠
    IPS130_WR_CMD_8(0x29);
}

void ips130_fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    uint16_t i, j;
    IPS130_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    for (i = ysta; i < yend; i++)
    {
        for (j = xsta; j < xend; j++)
        {
            IPS130_WR_DAT_16(color);
        }
    }
}