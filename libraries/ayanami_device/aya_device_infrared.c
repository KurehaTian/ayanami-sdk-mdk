#include "aya_device_infrared.h"

// 原始数据
uint8_t raw_buff[20];

static void delay_us(int us)
{
    volatile uint32_t dummy = 0;
    while (us)
    {
        us--;
        dummy = 80;
        while (dummy--)
            ;
    }
}

/**
 * @brief 对光电循迹传感器进行初始化
 *
 */
void infrared_init()
{
    gpio_init(INFRARED_PIN_DATA, GPI, GPIO_LOW, GPI_FF);
    gpio_init(INFRARED_PIN_CLK, GPO, GPIO_LOW, GPO_PP);
    gpio_init(INFRARED_PIN_SH, GPO, GPIO_HIGH, GPO_PP);
}

/**
 * @brief 从光电循迹传感器读出数据
 *
 */
void infrared_read()
{
    // 重新锁存数据
    gpio_set_level(INFRARED_PIN_SH, GPIO_LOW);
    delay_us(10);
    gpio_set_level(INFRARED_PIN_SH, GPIO_HIGH);
    delay_us(10);

    UARTprintf("\n\n");
    // 开始读入数据
    for (int i = 0; i < 16; i++)
    {
        gpio_set_level(INFRARED_PIN_CLK, GPIO_LOW);
        raw_buff[i] = gpio_get_level(INFRARED_PIN_DATA);
        delay_us(10);
        gpio_set_level(INFRARED_PIN_CLK, GPIO_HIGH);
        delay_us(10);
        UARTprintf("%d ", raw_buff[i]);
    }
    UARTprintf("\n");
}

/**
 * @brief 计算光电循迹的一阶矩，求出转向用的Err
 * 
 * @return float 
 */
float infrared_err_moment1()
{
    int16_t sum_xi = 0, sum_i = 0;
    float res;

    const uint8_t TRIGGER_LEVEL = 1; // 视为跟踪对象的数值

    for (int i = 0; i < 8; i++)
    {
        if (raw_buff[i] == TRIGGER_LEVEL)
        {
            sum_i++;
            sum_xi += (i - 8);
        }
    }
    for (int i = 8; i < 16; i++)
    {
        if (raw_buff[i] == TRIGGER_LEVEL)
        {
            sum_i++;
            sum_xi += (i - 7);
        }
    }
    if (sum_i != 0)
    {
        res = sum_xi * 12.5f / sum_i;
        //UARTprintf("err=%d\n", (int)res);
        return res;
    }
    else
        return 0;
}
