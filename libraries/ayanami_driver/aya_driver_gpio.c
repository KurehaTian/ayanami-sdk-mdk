/*
 * aya_driver_GPIO.c
 *
 *  Created on: 2023��4��2��
 *      Author: KurehaTian
 */
#include "aya_driver_gpio.h"
const uint32_t GPIO_BASE_ADDRESS[6] = {0x40004000, 0x40005000, 0x40006000, 0x40007000, 0x40024000, 0x40025000};

/**
 * @brief GPIO��ʼ��
 *
 * @param pin ѡ�������
 * @param dir ���ŵķ���   �����GPO   ���룺GPI    ���裺GPHW
 * @param dat ���ų�ʼ��ʱ���õĵ�ƽ״̬�����ʱ��Ч   0���͵�ƽ 1���ߵ�ƽ
 * @param mode �������� e.g. GPO_PU
 */
void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8_t dat, uint32_t mode)
{
    uint32_t sysctl_p = (pin & 0xff00) >> 8 | 0xf0000800;
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;
    uint32_t gpio_strength = mode >> 16;
    uint32_t gpio_mode = mode & 0x0000ffff;

    // ��������
    SysCtlPeripheralEnable(sysctl_p);
    while (!SysCtlPeripheralReady(sysctl_p))
        ;

    // ���IO����JTAG�ӿڣ�����Ҫ����������
    // ���������gpio_jtag
    if (pin == C2 || pin == C3)
    {
        HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x04;
        HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) &= 0xfb;
        HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x08;
        HWREG(GPIO_PORTC_BASE + GPIO_O_AFSEL) &= 0xf7;
        HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTC_BASE + GPIO_O_CR) = 0x00;
        HWREG(GPIO_PORTC_BASE + GPIO_O_LOCK) = 0;
    }
    if (pin == D7)
    {
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x04;
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
    }
    if (pin == F0)
    {
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
        HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
    }

    // �������ŷ���
    GPIODirModeSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, dir);
    // ������������
    GPIOPadConfigSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, gpio_strength, gpio_mode);

    if (dir == GPO)
        gpio_set_level(pin, dat);
}

/**
 * @brief GPIO�������
 *
 * @param pin ѡ�������
 * @param dat 0���͵�ƽ 1���ߵ�ƽ
 */
void gpio_set_level(gpio_pin_enum pin, uint8_t dat)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;
    if (dat)
        GPIOPinWrite(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, gpio_pin);
    else
        GPIOPinWrite(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, 0x0);
}

/**
 * @brief GPIO״̬��ȡ
 *
 * @param pin ѡ�������
 * @return uint8_t 0���͵�ƽ 1���ߵ�ƽ
 */
uint8_t gpio_get_level(gpio_pin_enum pin)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;
    uint32_t tmp = GPIOPinRead(GPIO_BASE_ADDRESS[gpio_port], gpio_pin) & 0xff;
    return tmp != 0;
}

/**
 * @brief  GPIO ��ת
 *
 * @param pin ѡ�������
 */
void gpio_toggle_level(gpio_pin_enum pin)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;

    if (gpio_get_level(pin))
        GPIOPinWrite(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, 0);
    else
        GPIOPinWrite(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, gpio_pin);
}

/**
 * @brief GPIO��������
 *
 * @param pin ѡ�������
 * @param dir ���ŵķ���   �����GPO   ���룺GPI    ���裺GPHW
 */
void gpio_set_dir(gpio_pin_enum pin, gpio_dir_enum dir)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;

    GPIODirModeSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, dir);
}
