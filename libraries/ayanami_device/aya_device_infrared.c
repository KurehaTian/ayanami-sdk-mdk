#include "aya_device_infrared.h"

// ԭʼ����
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
 * @brief �Թ��ѭ�����������г�ʼ��
 *
 */
void infrared_init()
{
    gpio_init(INFRARED_PIN_DATA, GPI, GPIO_LOW, GPI_FF);
    gpio_init(INFRARED_PIN_CLK, GPO, GPIO_LOW, GPO_PP);
    gpio_init(INFRARED_PIN_SH, GPO, GPIO_HIGH, GPO_PP);
}

/**
 * @brief �ӹ��ѭ����������������
 *
 */
void infrared_read()
{
    // ������������
    gpio_set_level(INFRARED_PIN_SH, GPIO_LOW);
    delay_us(10);
    gpio_set_level(INFRARED_PIN_SH, GPIO_HIGH);
    delay_us(10);

    UARTprintf("\n\n");
    // ��ʼ��������
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
 * @brief ������ѭ����һ�׾أ����ת���õ�Err
 * 
 * @return float 
 */
float infrared_err_moment1()
{
    int16_t sum_xi = 0, sum_i = 0;
    float res;

    const uint8_t TRIGGER_LEVEL = 1; // ��Ϊ���ٶ������ֵ

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
