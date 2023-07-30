/*
 * aya_driver_i2c.c
 *
 *  Created on: 2023年4月4日
 *      Author: KurehaTian
 */
#include "aya_driver_i2c.h"

#define I2C_SLAVE_ADDR 0x38

void i2c_init(i2c_index_t i2c_n)
{
    switch (i2c_n)
    {
    case I2C_0:
        // sysctl-i2c使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
        // sysctl-gpio使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        // IO配置
        GPIOPinConfigure(GPIO_PB2_I2C0SCL);
        GPIOPinConfigure(GPIO_PB3_I2C0SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
        GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
        // 时钟配置
        I2CMasterInitExpClk(I2C0_BASE, SysCtlClockGet(), true);
        break;
    case I2C_1:
        // sysctl-i2c使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
        // sysctl-gpio使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
        // IO配置
        GPIOPinConfigure(GPIO_PA6_I2C1SCL);
        GPIOPinConfigure(GPIO_PA7_I2C1SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
        GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
        // 时钟配置
        I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
        break;
    case I2C_2:
        // sysctl-i2c使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C2);
        // sysctl-gpio使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        // IO配置
        GPIOPinConfigure(GPIO_PE4_I2C2SCL);
        GPIOPinConfigure(GPIO_PE5_I2C2SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTE_BASE, GPIO_PIN_4);
        GPIOPinTypeI2C(GPIO_PORTE_BASE, GPIO_PIN_5);
        // 时钟配置
        I2CMasterInitExpClk(I2C2_BASE, SysCtlClockGet(), true);
        break;
    case I2C_3:
        // sysctl-i2c使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);
        // sysctl-gpio使能
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        // IO配置
        GPIOPinConfigure(GPIO_PD0_I2C3SCL);
        GPIOPinConfigure(GPIO_PD1_I2C3SDA);
        GPIOPinTypeI2CSCL(GPIO_PORTD_BASE, GPIO_PIN_0);
        GPIOPinTypeI2C(GPIO_PORTD_BASE, GPIO_PIN_1);
        // 时钟配置
        I2CMasterInitExpClk(I2C3_BASE, SysCtlClockGet(), true);
        break;

    default:
        break;
    }
    // sysctl-gpio使能

    // 引脚功能配置
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

void i2c_write_b(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t *data, uint8_t len)
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
        if (len == 1)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_SINGLE_SEND);
        else if (i == 0)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_SEND_START);
        else if (i == len - 1)
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
        else
            I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_SEND_CONT);

        I2CMasterDataPut(pher_base, data[i]);
        while (I2CMasterBusy(pher_base))
            ;
    }
}

uint8_t i2c_read_reg(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t reg)
{
    uint32_t pher_base = 0;
    uint8_t data;
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

    I2CMasterDataPut(pher_base, reg);
    I2CMasterControl(pher_base, I2C_MASTER_CMD_SINGLE_SEND);

    while (I2CMasterBusy(pher_base))
        ;

    I2CMasterSlaveAddrSet(pher_base, slav_addr, true);
    I2CMasterControl(pher_base, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while (I2CMasterBusy(pher_base))
        ;
    data = I2CMasterDataGet(pher_base);
    while (I2CMasterBusy(pher_base))
        ;

    return data;
}

void i2c_write_reg(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t reg, uint8_t data)
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
    I2CMasterDataPut(pher_base, reg);
    I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_SEND_START);

    while (I2CMasterBusy(pher_base))
        ;
    I2CMasterDataPut(pher_base, data);
    I2CMasterControl(pher_base, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while (I2CMasterBusy(pher_base))
        ;
}