#include "aya_device_lcd180.h"

uint16_t tft180_pencolor = TFT180_DEFAULT_PENCOLOR;
uint16_t tft180_bgcolor = TFT180_DEFAULT_BGCOLOR;

static tft180_dir_enum tft180_display_dir = TFT180_DEFAULT_DISPLAY_DIR;
static tft180_font_size_enum tft180_display_font = TFT180_DEFAULT_DISPLAY_FONT;

static uint8_t tft180_x_max = 160;
static uint8_t tft180_y_max = 128;

static void delay_ms(int ms)
{
    systick_delay_ms(ms);
}

static void delay_us(int us)
{
    systick_delay_us(us);
}

static void func_int_to_str(char *str, int32_t number)
{

    uint8_t data_temp[16]; // 缓冲区
    uint8_t bit = 0;       // 数字位数
    int32_t number_temp = 0;

    do
    {
        if (NULL == str)
        {
            break;
        }

        if (0 > number) // 负数
        {
            *str++ = '-';
            number = -number;
        }
        else if (0 == number) // 或者这是个 0
        {
            *str = '0';
            break;
        }

        while (0 != number) // 循环直到数值归零
        {
            number_temp = number % 10;
            data_temp[bit++] = abs(number_temp); // 倒序将数值提取出来
            number /= 10;                        // 削减被提取的个位数
        }
        while (0 != bit) // 提取的数字个数递减处理
        {
            *str++ = (data_temp[bit - 1] + 0x30); // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit--;
        }
    } while (0);
}

void func_uint_to_str(char *str, uint32_t number)
{
    int8_t data_temp[16]; // 缓冲区
    uint8_t bit = 0;      // 数字位数

    do
    {
        if (NULL == str)
        {
            break;
        }

        if (0 == number) // 这是个 0
        {
            *str = '0';
            break;
        }

        while (0 != number) // 循环直到数值归零
        {
            data_temp[bit++] = (number % 10); // 倒序将数值提取出来
            number /= 10;                     // 削减被提取的个位数
        }
        while (0 != bit) // 提取的数字个数递减处理
        {
            *str++ = (data_temp[bit - 1] + 0x30); // 将数字从倒序数组中倒序取出 变成正序放入字符串
            bit--;
        }
    } while (0);
}

void func_float_to_str(char *str, float number, uint8_t point_bit)
{
    int data_int = 0;        // 整数部分
    int data_float = 0.0;    // 小数部分
    int data_temp[8];        // 整数字符缓冲
    int data_temp_point[6];  // 小数字符缓冲
    uint8_t bit = point_bit; // 转换精度位数

    do
    {
        if (NULL == str)
        {
            break;
        }

        // 提取整数部分
        data_int = (int)number; // 直接强制转换为 int
        if (0 > number)         // 判断源数据是正数还是负数
        {
            *str++ = '-';
        }
        else if (0.0 == number) // 如果是个 0
        {
            *str++ = '0';
            *str++ = '.';
            *str = '0';
            break;
        }

        // 提取小数部分
        number = number - data_int; // 减去整数部分即可
        while (bit--)
        {
            number = number * 10; // 将需要的小数位数提取到整数部分
        }
        data_float = (int)number; // 获取这部分数值

        // 整数部分转为字符串
        bit = 0;
        do
        {
            data_temp[bit++] = data_int % 10; // 将整数部分倒序写入字符缓冲区
            data_int /= 10;
        } while (0 != data_int);
        while (0 != bit)
        {
            *str++ = (abs(data_temp[bit - 1]) + 0x30); // 再倒序将倒序的数值写入字符串 得到正序数值
            bit--;
        }

        // 小数部分转为字符串
        if (point_bit != 0)
        {
            bit = 0;
            *str++ = '.';
            if (0 == data_float)
            {
                *str = '0';
            }
            else
            {
                while (0 != point_bit) // 判断有效位数
                {
                    data_temp_point[bit++] = data_float % 10; // 倒序写入字符缓冲区
                    data_float /= 10;
                    point_bit--;
                }
                while (0 != bit)
                {
                    *str++ = (abs(data_temp_point[bit - 1]) + 0x30); // 再倒序将倒序的数值写入字符串 得到正序数值
                    bit--;
                }
            }
        }
    } while (0);
}
/**
 *
 * @brief TFT180 SPI 写 8bit 数据
 * @param dat   数据
 */
static void TFT180_WR_BUS_8(uint8_t dat)
{
    uint8_t tx = dat;
    spi_write(TFT180_SPI, &tx, 1);
}

/**
 * @brief TFT180 SPI 写 16bit 数据
 * @param dat   数据
 */
static void TFT180_WR_BUS_16(uint16_t dat)
{
    uint8_t tx[2];
    tx[0] = (dat & 0xff00) >> 8;
    tx[1] = (dat & 0x00ff);
    spi_write(TFT180_SPI, tx, 2);
}

/**
 * @brief TFT180写命令
 *
 * @param dat
 */
static void TFT180_WR_CMD_8(uint8_t dat)
{
    TFT180_DC(0);
    TFT180_WR_BUS_8(dat);
    TFT180_DC(1);
}

static void TFT180_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (tft180_display_dir == TFT180_PORTAIT || tft180_display_dir == TFT180_PORTAIT_180)
    {
        TFT180_WR_CMD_8(0x2a);
        TFT180_WR_BUS_16(x1 + 2);
        TFT180_WR_BUS_16(x2 + 2);

        TFT180_WR_CMD_8(0x2b);
        TFT180_WR_BUS_16(y1 + 1);
        TFT180_WR_BUS_16(y2 + 1);
    }
    else
    {
        TFT180_WR_CMD_8(0x2a);
        TFT180_WR_BUS_16(x1 + 1);
        TFT180_WR_BUS_16(x2 + 1);

        TFT180_WR_CMD_8(0x2b);
        TFT180_WR_BUS_16(y1 + 2);
        TFT180_WR_BUS_16(y2 + 2);
    }
    TFT180_WR_CMD_8(0x2c);
}

/**
 * @brief 清屏
 *
 */
void tft180_clear(void)
{
    uint32_t pixel = tft180_x_max * tft180_y_max;
    TFT180_CS(0);
    TFT180_Address_Set(0, 0, tft180_x_max, tft180_y_max);
    for (; pixel > 0; pixel--)
    {
        TFT180_WR_BUS_16(tft180_bgcolor);
    }
    TFT180_CS(1);
}

/**
 * @brief 填充
 * @param color
 */
void tft180_fill(const uint16_t color)
{
    uint32_t i = tft180_x_max * tft180_y_max;

    TFT180_CS(0);
    TFT180_Address_Set(0, 0, tft180_x_max, tft180_y_max);
    for (; i > 0; i--)
    {
        TFT180_WR_BUS_16(color);
    }
    TFT180_CS(1);
}

/**
 * @brief 设置显示方向
 * @param dir
 */
void tft180_set_dir(tft180_dir_enum dir)
{
    tft180_display_dir = dir;
    if (dir < 2)
    {
        tft180_x_max = 128;
        tft180_y_max = 160;
    }
    else
    {
        tft180_x_max = 160;
        tft180_y_max = 128;
    }
}

/**
 * @brief 设置显示字体
 * @param font
 */
void tft180_set_font(tft180_font_size_enum font)
{
    tft180_display_font = font;
}

/**
 * @brief 设置显示颜色
 *
 * @param pen
 * @param bgcolor
 */
void tft180_set_color(uint16_t pen, const uint16_t bgcolor)
{
    tft180_pencolor = pen;
    tft180_bgcolor = bgcolor;
}

/**
 * @brief 画点
 *
 * @param x
 * @param y
 * @param color
 */
void tft180_draw_point(uint16_t x, uint16_t y, const uint16_t color)
{
    TFT180_CS(0);
    TFT180_Address_Set(x, y, x, y);
    TFT180_WR_BUS_16(color);
    TFT180_CS(1);
}

/**
 * @brief 画线
 *
 * @param x_start
 * @param y_start
 * @param x_end
 * @param y_end
 * @param color
 */
void tft180_draw_line(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color)
{
    int16_t x_dir = (x_start < x_end ? 1 : -1);
    int16_t y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;
    if (x_start != x_end)
    {
        temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
        temp_b = (float)y_start - (float)x_start * temp_rate;
    }
    else
    {
        while (y_start != y_end)
        {
            tft180_draw_point(x_start, y_start, color);
            y_start += y_dir;
        }
        return;
    }

    if (abs(y_start - y_end) > abs(x_start - x_end))
    {
        while (y_start != y_end)
        {
            tft180_draw_point(x_start, y_start, color);
            y_start += y_dir;
            x_start = (int16_t)(((float)y_start - temp_b) / temp_rate);
        }
    }
    else
    {
        while (x_start != x_end)
        {
            tft180_draw_point(x_start, y_start, color);
            x_start += x_dir;
            y_start = (int16_t)((float)x_start * temp_rate + temp_b);
        }
    }
}

/**
 * @brief 显示字符
 *
 * @param x
 * @param y
 * @param dat
 */
void tft180_show_char(uint16_t x, uint16_t y, const char dat)
{

    uint8_t i, j;

    TFT180_CS(0);
    switch (tft180_display_font)
    {
    case TFT180_6X8_FONT:
        for (i = 0; i < 6; i++)
        {
            TFT180_Address_Set(x + i, y, x + i, y + 8);
            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
            uint8_t temp_top = ascii_font_6x8[dat - 32][i];
            for (j = 0; j < 8; j++)
            {
                if (temp_top & 0x01)
                {
                    TFT180_WR_BUS_16(tft180_pencolor);
                }
                else
                {
                    TFT180_WR_BUS_16(tft180_bgcolor);
                }
                temp_top >>= 1;
            }
        }
        break;
    case TFT180_8X16_FONT:
        for (i = 0; i < 8; i++)
        {
            TFT180_Address_Set(x + i, y, x + i, y + 15);
            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
            uint8_t temp_top = ascii_font_8x16[dat - 32][i];
            uint8_t temp_bottom = ascii_font_8x16[dat - 32][i + 8];
            for (j = 0; j < 8; j++)
            {
                if (temp_top & 0x01)
                {
                    TFT180_WR_BUS_16(tft180_pencolor);
                }
                else
                {
                    TFT180_WR_BUS_16(tft180_bgcolor);
                }
                temp_top >>= 1;
            }
            for (j = 0; j < 8; j++)
            {
                if (temp_bottom & 0x01)
                {
                    TFT180_WR_BUS_16(tft180_pencolor);
                }
                else
                {
                    TFT180_WR_BUS_16(tft180_bgcolor);
                }
                temp_bottom >>= 1;
            }
        }
        break;
    }
    TFT180_CS(1);
}

/**
 * @brief 显示字符串
 *
 * @param x
 * @param y
 * @param dat
 */
void tft180_show_string(uint16_t x, uint16_t y, const char dat[])
{
    uint16_t j = 0;
    while (dat[j] != '\0')
    {
        switch (tft180_display_font)
        {
        case TFT180_6X8_FONT:
            tft180_show_char(x + 6 * j, y, dat[j]);
            j++;
            break;
        case TFT180_8X16_FONT:
            tft180_show_char(x + 8 * j, y, dat[j]);
            j++;
            break;
        }
    }
}

/**
 * @brief 显示整形数据
 *
 * @param x
 * @param y
 * @param dat
 * @param num
 */
void tft180_show_int(uint16_t x, uint16_t y, const int32_t dat, uint8_t num)
{
    int32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num + 1);

    if (num < 10)
    {
        for (; num > 0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    // sprintf(data_buffer, "%d", dat_temp);
    tft180_show_string(x, y, (const char *)&data_buffer);
}

void tft180_show_float(uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum)
{
    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num + pointnum + 2);

    if (num < 10)
    {
        for (; num > 0; num--)
            offset *= 10;
        dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    }
    func_float_to_str(data_buffer, dat_temp, pointnum);
    // sprintf(data_buffer, "%.2f", dat_temp);
    tft180_show_string(x, y, data_buffer);
}

void tft180_show_char_color(uint16_t x, uint16_t y, const char dat, uint16_t colorf, uint16_t colorb)
{
    uint8_t i, j;
    TFT180_CS(0);
    switch (tft180_display_font)
    {
    case TFT180_6X8_FONT:
        for (i = 0; i < 6; i++)
        {
            TFT180_Address_Set(x + i, y, x + i, y + 8);
            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
            uint8_t temp_top = ascii_font_6x8[dat - 32][i];
            for (j = 0; j < 8; j++)
            {
                if (temp_top & 0x01)
                {
                    TFT180_WR_BUS_16(colorf);
                }
                else
                {
                    TFT180_WR_BUS_16(colorb);
                }
                temp_top >>= 1;
            }
        }
        break;
    case TFT180_8X16_FONT:
        for (i = 0; i < 8; i++)
        {
            TFT180_Address_Set(x + i, y, x + i, y + 15);
            // 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
            uint8_t temp_top = ascii_font_8x16[dat - 32][i];
            uint8_t temp_bottom = ascii_font_8x16[dat - 32][i + 8];
            for (j = 0; j < 8; j++)
            {
                if (temp_top & 0x01)
                {
                    TFT180_WR_BUS_16(tft180_pencolor);
                }
                else
                {
                    TFT180_WR_BUS_16(tft180_bgcolor);
                }
                temp_top >>= 1;
            }
            for (j = 0; j < 8; j++)
            {
                if (temp_bottom & 0x01)
                {
                    TFT180_WR_BUS_16(tft180_pencolor);
                }
                else
                {
                    TFT180_WR_BUS_16(tft180_bgcolor);
                }
                temp_bottom >>= 1;
            }
        }
        break;
    }
    TFT180_CS(1);
}

void tft180_show_string_color(uint16_t x, uint16_t y, const char dat[], uint16_t colorf, uint16_t colorb)
{
    uint16_t j = 0;
    while (dat[j] != '\0')
    {
        switch (tft180_display_font)
        {
        case TFT180_6X8_FONT:
            tft180_show_char_color(x + 6 * j, y, dat[j], colorf, colorb);
            j++;
            break;
        case TFT180_8X16_FONT:
            tft180_show_char_color(x + 8 * j, y, dat[j], colorf, colorb);
            j++;
            break;
        }
    }
}

/**
 * @brief 显示无符号整数
 *
 * @param x
 * @param y
 * @param dat
 * @param num
 * @param colorf
 * @param colorb
 */
void tft180_show_uint_color(uint16_t x, uint16_t y, const uint32_t dat, uint8_t num, uint16_t colorf, uint16_t colorb)
{

    uint32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if (num < 10)
    {
        for (; num > 0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    // sprintf(data_buffer, "%d", dat_temp);
    tft180_show_string_color(x, y, (const char *)&data_buffer, colorf, colorb);
}

void tft180_show_int_color(uint16_t x, uint16_t y, const int32_t dat, uint8_t num, uint16_t colorf, uint16_t colorb)
{
    int32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num + 1);

    if (num < 10)
    {
        for (; num > 0; num--)
            offset *= 10;
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    // sprintf(data_buffer, "%d", dat_temp);
    tft180_show_string_color(x, y, (const char *)&data_buffer, colorf, colorb);
}

void tft180_show_float_color(uint16_t x, uint16_t y, const float dat, uint8_t num, uint8_t pointnum, uint16_t colorf, uint16_t colorb)
{
    float dat_temp = dat;
    float offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num + pointnum + 2);

    if (num < 10)
    {
        for (; num > 0; num--)
            offset *= 10;
        dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    }
    func_float_to_str(data_buffer, dat_temp, pointnum);
    // sprintf(data_buffer, "%.2f", dat_temp);
    tft180_show_string_color(x, y, data_buffer, colorf, colorb);
}

void tft180_fill_block(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{

    uint16_t X1 = _min(x1, x2);
    uint16_t X2 = _max(x1, x2);
    uint16_t Y1 = _min(y1, y2);
    uint16_t Y2 = _max(y1, y2);

    TFT180_CS(0);
    for (int i = Y1; i <= Y2; i++)
    {
        TFT180_Address_Set(X1, i, X2, i);
        for (int j = X1; j <= X2; j++)
            TFT180_WR_BUS_16(color);
    }
    TFT180_CS(1);
}

void tft180_init(void)
{
    spi_init(TFT180_SPI, SPI_MODE_3, TFT180_SPI_SPEED);

    gpio_init(TFT180_DC_PIN, GPO, GPIO_LOW, GPO_PP);
    gpio_init(TFT180_RES_PIN, GPO, GPIO_LOW, GPO_PP);
    gpio_init(TFT180_CS_PIN, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(TFT180_BL_PIN, GPO, GPIO_HIGH, GPO_PP);

    tft180_set_dir(tft180_display_dir);
    tft180_set_color(tft180_pencolor, tft180_bgcolor);

    TFT180_RST(0);
    delay_ms(10);

    TFT180_RST(1);
    delay_ms(120);
    TFT180_CS(0);

    TFT180_WR_CMD_8(0x11);
    delay_ms(120);

    TFT180_WR_CMD_8(0xB1);
    TFT180_WR_BUS_8(0x01);
    TFT180_WR_BUS_8(0x2C);
    TFT180_WR_BUS_8(0x2D);

    TFT180_WR_CMD_8(0xB2);
    TFT180_WR_BUS_8(0x01);
    TFT180_WR_BUS_8(0x2C);
    TFT180_WR_BUS_8(0x2D);

    TFT180_WR_CMD_8(0xB3);
    TFT180_WR_BUS_8(0x01);
    TFT180_WR_BUS_8(0x2C);
    TFT180_WR_BUS_8(0x2D);
    TFT180_WR_BUS_8(0x01);
    TFT180_WR_BUS_8(0x2C);
    TFT180_WR_BUS_8(0x2D);

    TFT180_WR_CMD_8(0xB4);
    TFT180_WR_BUS_8(0x07);

    TFT180_WR_CMD_8(0xC0);
    TFT180_WR_BUS_8(0xA2);
    TFT180_WR_BUS_8(0x02);
    TFT180_WR_BUS_8(0x84);
    TFT180_WR_CMD_8(0xC1);
    TFT180_WR_BUS_8(0xC5);

    TFT180_WR_CMD_8(0xC2);
    TFT180_WR_BUS_8(0x0A);
    TFT180_WR_BUS_8(0x00);

    TFT180_WR_CMD_8(0xC3);
    TFT180_WR_BUS_8(0x8A);
    TFT180_WR_BUS_8(0x2A);
    TFT180_WR_CMD_8(0xC4);
    TFT180_WR_BUS_8(0x8A);
    TFT180_WR_BUS_8(0xEE);

    TFT180_WR_CMD_8(0xC5);
    TFT180_WR_BUS_8(0x0E);

    TFT180_WR_CMD_8(0x36);
    switch (tft180_display_dir) // y x v
    {
    case 0:
        TFT180_WR_BUS_8(1 << 7 | 1 << 6 | 0 << 5);
        break; // 竖屏模式
    case 1:
        TFT180_WR_BUS_8(0 << 7 | 0 << 6 | 0 << 5);
        break; // 竖屏模式  旋转180
    case 2:
        TFT180_WR_BUS_8(1 << 7 | 0 << 6 | 1 << 5);
        break; // 横屏模式
    case 3:
        TFT180_WR_BUS_8(0 << 7 | 1 << 6 | 1 << 5);
        break; // 横屏模式  旋转180
    }

    TFT180_WR_CMD_8(0xe0);
    TFT180_WR_BUS_8(0x0f);
    TFT180_WR_BUS_8(0x1a);
    TFT180_WR_BUS_8(0x0f);
    TFT180_WR_BUS_8(0x18);
    TFT180_WR_BUS_8(0x2f);
    TFT180_WR_BUS_8(0x28);
    TFT180_WR_BUS_8(0x20);
    TFT180_WR_BUS_8(0x22);
    TFT180_WR_BUS_8(0x1f);
    TFT180_WR_BUS_8(0x1b);
    TFT180_WR_BUS_8(0x23);
    TFT180_WR_BUS_8(0x37);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x07);
    TFT180_WR_BUS_8(0x02);
    TFT180_WR_BUS_8(0x10);

    TFT180_WR_CMD_8(0xe1);
    TFT180_WR_BUS_8(0x0f);
    TFT180_WR_BUS_8(0x1b);
    TFT180_WR_BUS_8(0x0f);
    TFT180_WR_BUS_8(0x17);
    TFT180_WR_BUS_8(0x33);
    TFT180_WR_BUS_8(0x2c);
    TFT180_WR_BUS_8(0x29);
    TFT180_WR_BUS_8(0x2e);
    TFT180_WR_BUS_8(0x30);
    TFT180_WR_BUS_8(0x30);
    TFT180_WR_BUS_8(0x39);
    TFT180_WR_BUS_8(0x3f);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x07);
    TFT180_WR_BUS_8(0x03);
    TFT180_WR_BUS_8(0x10);

    TFT180_WR_CMD_8(0x2a);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x00 + 2);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x80 + 2);

    TFT180_WR_CMD_8(0x2b);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x00 + 3);
    TFT180_WR_BUS_8(0x00);
    TFT180_WR_BUS_8(0x80 + 3);

    TFT180_WR_CMD_8(0xF0);
    TFT180_WR_BUS_8(0x01);
    TFT180_WR_CMD_8(0xF6);
    TFT180_WR_BUS_8(0x00);

    TFT180_WR_CMD_8(0x3A);
    TFT180_WR_BUS_8(0x05);
    TFT180_WR_CMD_8(0x29);
    TFT180_CS(1);

    tft180_clear();
}
