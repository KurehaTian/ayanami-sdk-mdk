/*
 * aya_driver_i2c.h
 *
 *  Created on: 2023Äê4ÔÂ4ÈÕ
 *      Author: KurehaTian
 */

#ifndef AYA_DRIVER_I2C_H_
#define AYA_DRIVER_I2C_H_
#include <aya_driver_head.h>
#include <aya_device_head.h>

typedef enum
{
    I2C_0 = 0,
    I2C_1,
    I2C_2,
    I2C_3
} i2c_index_t;

/**
 *
typedef enum
{
    I2C0_SCL_PB2,
    I2C1_SCL_PA6,
    I2C2_SCL_PE4,
    I2C3_SCL_PD0,
}i2c_scl_pin;

typedef enum
{
    I2C0_SDA_PB3,
    I2C1_SDA_PA7,
    I2C2_SDA_PE5,
    I2C3_SDA_PD1,
}i2c_sda_pin;
*/

void i2c_init(i2c_index_t i2c_n);
void i2c_write(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t *data, uint8_t len);
void i2c_read(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t *data, uint8_t len);

uint8_t i2c_read_reg(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t reg);
void i2c_write_reg(i2c_index_t i2c_n, uint8_t slav_addr, uint8_t reg, uint8_t data);
#endif /* AYANAMI_DRIVER_AYA_DRIVER_I2C_H_ */
