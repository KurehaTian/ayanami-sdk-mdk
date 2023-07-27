/*
 * aya_driver_GPIO.c
 *
 *  Created on: 2023年4月2日
 *      Author: KurehaTian
 */
#include "aya_driver_gpio.h"
const uint32_t GPIO_BASE_ADDRESS[6] = {0x40004000, 0x40005000, 0x40006000, 0x40007000, 0x40024000, 0x40025000};

/**
 * @brief GPIO初始化
 *
 * @param pin 选择的引脚
 * @param dir 引脚的方向   输出：GPO   输入：GPI    外设：GPHW
 * @param dat 引脚初始化时设置的电平状态，输出时有效   0：低电平 1：高电平
 * @param mode 引脚配置 e.g. GPO_PU
 */
void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8_t dat, uint32_t mode)
{
    uint32_t sysctl_p = (pin & 0xff00) >> 8 | 0xf0000800;
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;
    uint32_t gpio_strength = mode >> 16;
    uint32_t gpio_mode = mode & 0x0000ffff;

    // 启动外设
    SysCtlPeripheralEnable(sysctl_p);
    while (!SysCtlPeripheralReady(sysctl_p))
        ;

    // 如果IO口是JTAG接口，则需要将配置锁打开
    // 详情见例程gpio_jtag
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

    // 配置引脚方向
    GPIODirModeSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, dir);
    // 配置引脚设置
    GPIOPadConfigSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, gpio_strength, gpio_mode);

    if (dir == GPO)
        gpio_set_level(pin, dat);
}

/**
 * @brief GPIO输出设置
 *
 * @param pin 选择的引脚
 * @param dat 0：低电平 1：高电平
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
 * @brief GPIO状态获取
 *
 * @param pin 选择的引脚
 * @return uint8_t 0：低电平 1：高电平
 */
uint8_t gpio_get_level(gpio_pin_enum pin)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;
    uint32_t tmp = GPIOPinRead(GPIO_BASE_ADDRESS[gpio_port], gpio_pin) & 0xff;
    return tmp != 0;
}

/**
 * @brief  GPIO 翻转
 *
 * @param pin 选择的引脚
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
 * @brief GPIO方向设置
 *
 * @param pin 选择的引脚
 * @param dir 引脚的方向   输出：GPO   输入：GPI    外设：GPHW
 */
void gpio_set_dir(gpio_pin_enum pin, gpio_dir_enum dir)
{
    uint32_t gpio_port = (pin & 0xff00) >> 8;
    uint32_t gpio_pin = pin & 0xff;

    GPIODirModeSet(GPIO_BASE_ADDRESS[gpio_port], gpio_pin, dir);
}
