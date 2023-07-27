/*
 * aya_driver_i2c.c
 *
 *  Created on: 2023��4��4��
 *      Author: KurehaTian
 */
#include "aya_driver_i2c.h"

#define I2C_SLAVE_ADDR 0x38

void i2c_init(i2c_index_t i2c_n)
{
    switch (i2c_n)
    {
    case I2C_0:
        // sysctl-i2cʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
        // sysctl-gpioʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        // IO����
        GPIOPinConfigure(GPIO_PB2_I2C0SCL);
        GPIOPinConfigure(GPIO_PB3_I2C0SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
        GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
        // ʱ������
        I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
        break;
    case I2C_1:
        // sysctl-i2cʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
        // sysctl-gpioʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        // IO����
        GPIOPinConfigure(GPIO_PA6_I2C1SCL);
        GPIOPinConfigure(GPIO_PA7_I2C1SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
        GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
        // ʱ������
        I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);
        break;
    case I2C_2:
        // sysctl-i2cʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
        // sysctl-gpioʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        // IO����
        GPIOPinConfigure(GPIO_PE4_I2C2SCL);
        GPIOPinConfigure(GPIO_PE5_I2C2SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTE_BASE, GPIO_PIN_4);
        GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_5);
        // ʱ������
        I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), true);
        break;
    case I2C_3:
        // sysctl-i2cʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
        // sysctl-gpioʹ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        // IO����
        GPIOPinConfigure(GPIO_PD0_I2C3SCL);
        GPIOPinConfigure(GPIO_PD1_I2C3SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
        GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
        // ʱ������
        I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), true);
        break;

    default:
        break;
    }
    // sysctl-gpioʹ��

    // ���Ź�������
}

void i2c_write(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t *data, uint8_t len)
{
    uint32_t pher_base = 0;

    switch (i2c_n)
    {
    case I2C_0:
        pher_base = I2C0_BASE;
        break;
    case I2C_1:
        pher_base = I2C1_BASE;
        break;
    case I2C_2:
        pher_base = I2C2_BASE;
        break;
    case I2C_3:
        pher_base = I2C3_BASE;
        break;
    }
    I2CMasterSlaveAddrSet(pher_base, slav_addr, false);
    for (uint8_t i = 0; i < len; i++)
    {
        I2CMasterDataPut(pher_base, data[i]);
        I2CMasterControl(pher_base, I2C_MASTER_CMD_SINGLE_SEND);
        while (I2CMasterBusy(pher_base))
            ;
    }
}

void i2c_read(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t *data, uint8_t len)
{
    uint32_t pher_base = 0;
    switch (i2c_n)
    {
    case I2C_0:
        pher_base = I2C0_BASE;
        break;
    case I2C_1:
        pher_base = I2C1_BASE;
        break;
    case I2C_2:
        pher_base = I2C2_BASE;
        break;
    case I2C_3:
        pher_base = I2C3_BASE;
        break;
    }
    I2CMasterSlaveAddrSet(pher_base, slav_addr, true);
    // Read data
    for (uint8_t i = 0; i < len; i++)
    {
        if (len == 1)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
        else if (i == 0)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_RECEIVE_START);
        else if (i == len - 1)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        else
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_RECEIVE_CONT);

        while (I2CMasterBusy(pher_base))
            ;
        data[i] = I2CMasterDataGet(pher_base);
    }
}
