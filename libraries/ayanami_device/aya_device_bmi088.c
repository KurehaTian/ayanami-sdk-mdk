#include "aya_device_bmi088.h"

bmi088_data_t bmi088_data;
#define BMI088_ACCEL_SEN BMI088_ACCEL_12G_SEN
#define BMI088_GYRO_SEN BMI088_GYRO_2000_SEN

bsxlite_instance_t bmi088_ins;
vector_3d_t bsx_input_acc, bsx_input_gyo;
bsxlite_out_t bsx_out_data;

float delta_angle(float current, float target)
{
    float ans = current - target;
    if (ans > 180)
        return ans - 360;
    if (ans < -180)
        return ans + 360;
    return ans;
}

static void delay_ms(int ms)
{
     systick_delay_ms(ms);
}

static void delay_us(int us)
{
   systick_delay_us(us);
}

static uint8_t bmi088_acc_read_reg(uint8_t reg)
{
    uint8_t dat[2] = {reg | 0x80, 0};
    uint8_t dat_r[2];
    gpio_set_level(BMI088_PIN_CS1, GPIO_LOW);
    spi_readwrite(BMI088_SPI, dat, dat_r, 2);
    // UARTprintf("Acc Read Reg-%x", reg);
    // UARTprintf("-%x-%x\n", dat_r[0],dat_r[1]);
    gpio_set_level(BMI088_PIN_CS1, GPIO_HIGH);
    return dat_r[1];
}

static uint8_t bmi088_gyro_read_reg(uint8_t reg)
{
    uint8_t dat[3] = {reg | 0x80, 0x00, 0x00};
    uint8_t dat_r[3];
    gpio_set_level(BMI088_PIN_CS2, GPIO_LOW);
    spi_readwrite(BMI088_SPI, dat, dat_r, 2);
    // UARTprintf("Gyro Read Reg-%x", reg);
    // UARTprintf("-%x %x %x\n", dat_r[0], dat_r[1],dat_r[2]);
    gpio_set_level(BMI088_PIN_CS2, GPIO_HIGH);
    return dat_r[1];
}

static void bmi088_acc_read_regs(uint8_t reg, uint8_t *buf, int len)
{
    gpio_set_level(BMI088_PIN_CS1, GPIO_LOW);
    spi_readwritebyte(BMI088_SPI, reg);
    spi_read(BMI088_SPI, buf, len);
    gpio_set_level(BMI088_PIN_CS1, GPIO_HIGH);
}

static void bmi088_gyro_read_regs(uint8_t reg, uint8_t *buf, int len)
{
    gpio_set_level(BMI088_PIN_CS2, GPIO_LOW);
    spi_readwritebyte(BMI088_SPI, reg);
    spi_read(BMI088_SPI, buf, len);
    gpio_set_level(BMI088_PIN_CS2, GPIO_HIGH);
}

static void bmi088_acc_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t tx_buf[2] = {reg, value};
    gpio_set_level(BMI088_PIN_CS1, GPIO_LOW);
    spi_write(BMI088_SPI, tx_buf, 2);
    gpio_set_level(BMI088_PIN_CS1, GPIO_HIGH);
}

static void bmi088_gyro_write_reg(uint8_t reg, uint8_t value)
{
    uint8_t tx_buf[2] = {reg, value};
    gpio_set_level(BMI088_PIN_CS2, GPIO_LOW);
    spi_write(BMI088_SPI, tx_buf, 2);
    gpio_set_level(BMI088_PIN_CS2, GPIO_HIGH);
}

void bmi088_init()
{
    // 相关外设初始化

    gpio_init(BMI088_PIN_CS1, GPO, GPIO_HIGH, GPO_PP);
    gpio_init(BMI088_PIN_CS2, GPO, GPIO_HIGH, GPO_PP);
    spi_init(SPI_0, SPI_MODE_3, 2000000);
    gpio_set_level(BMI088_PIN_CS1, GPIO_LOW);
    gpio_set_level(BMI088_PIN_CS1, GPIO_HIGH);

    // 对解算程序进行初始化
    int16_t ret = 0;
    ret = bsxlite_init(&bmi088_ins);
    UARTprintf("Bsxlite init:%d\n", ret);

    //----------加速度初始化
    // 检查连接
    UARTprintf("Connect Check.\n");
    while (bmi088_acc_read_reg(BMI088_ACC_CHIP_ID) != BMI088_ACC_CHIP_ID_VALUE)
        delay_ms(1);
    UARTprintf("Connect Fine.\n");
    // 软重启
    bmi088_acc_write_reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
    delay_ms(BMI088_LONG_DELAY_TIME);

    // 检查连接
    while (bmi088_acc_read_reg(BMI088_ACC_CHIP_ID) != BMI088_ACC_CHIP_ID_VALUE)
        delay_ms(1);
    UARTprintf("Connect Fine2.\n");
    bmi088_acc_write_reg(BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON);
    delay_ms(1);

    bmi088_acc_write_reg(BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE);
    delay_ms(1);

    bmi088_acc_write_reg(BMI088_ACC_CONF, BMI088_ACC_NORMAL | BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set);
    delay_ms(1);

    // 设置加速度量程!!!! -
    bmi088_acc_write_reg(BMI088_ACC_RANGE, BMI088_ACC_RANGE_12G);
    delay_ms(1);

    bmi088_acc_write_reg(BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW);
    delay_ms(1);

    bmi088_acc_write_reg(BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT);
    delay_ms(1);

    //--------陀螺仪初始化
    // 检查连接
    UARTprintf("Connect Gyro.\n");
    while (bmi088_gyro_read_reg(BMI088_GYRO_CHIP_ID) != BMI088_GYRO_CHIP_ID_VALUE)
        delay_ms(1);

    // 软重启
    bmi088_gyro_write_reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
    delay_ms(BMI088_LONG_DELAY_TIME);

    // 检查连接
    while (bmi088_gyro_read_reg(BMI088_GYRO_CHIP_ID) != BMI088_GYRO_CHIP_ID_VALUE)
        delay_ms(1);

    // 设置陀螺仪量程
    bmi088_gyro_write_reg(BMI088_GYRO_RANGE, BMI088_GYRO_2000);
    delay_ms(1);

    bmi088_gyro_write_reg(BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set);
    delay_ms(1);

    bmi088_gyro_write_reg(BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE);
    delay_ms(1);

    bmi088_gyro_write_reg(BMI088_GYRO_CTRL, BMI088_DRDY_ON);
    delay_ms(1);

    bmi088_gyro_write_reg(BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW);
    delay_ms(1);

    bmi088_gyro_write_reg(BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3);
    delay_ms(1);
    UARTprintf("init finish.\n");
}

void bmi088_read_data()
{
    uint8_t dat[10];
    bmi088_acc_read_regs(BMI088_ACCEL_XOUT_L | 0x80, dat, 7);
    bmi088_data.raw_acc_x = ((int16_t)dat[2] << 8) | dat[1];
    bmi088_data.raw_acc_y = ((int16_t)dat[4] << 8) | dat[3];
    bmi088_data.raw_acc_z = ((int16_t)dat[6] << 8) | dat[5];

    bmi088_data.acc_x = bmi088_data.raw_acc_x * BMI088_ACCEL_SEN;
    bmi088_data.acc_y = bmi088_data.raw_acc_y * BMI088_ACCEL_SEN;
    bmi088_data.acc_z = bmi088_data.raw_acc_z * BMI088_ACCEL_SEN;

    bmi088_gyro_read_regs(BMI088_GYRO_CHIP_ID | 0x80, dat, 8);
    if (dat[0] == BMI088_GYRO_CHIP_ID_VALUE)
    {
        bmi088_data.raw_gyro_x = ((int16_t)dat[3] << 8) | dat[2];
        bmi088_data.raw_gyro_y = ((int16_t)dat[5] << 8) | dat[4];
        bmi088_data.raw_gyro_z = ((int16_t)dat[7] << 8) | dat[6];

        bmi088_data.gyro_x = bmi088_data.raw_gyro_x * BMI088_GYRO_SEN;
        bmi088_data.gyro_y = bmi088_data.raw_gyro_y * BMI088_GYRO_SEN;
        bmi088_data.gyro_z = bmi088_data.raw_gyro_z * BMI088_GYRO_SEN;
    }
}

void bmi088_bsx_calc()
{
    bmi088_data.time_stamp_us += 10000;
    bsx_input_acc.x = bmi088_data.acc_x;
    bsx_input_acc.y = bmi088_data.acc_y;
    bsx_input_acc.z = bmi088_data.acc_z;
    bsx_input_gyo.x = bmi088_data.gyro_x;
    bsx_input_gyo.y = bmi088_data.gyro_y;
    bsx_input_gyo.z = bmi088_data.gyro_z;
    bsxlite_do_step(&bmi088_ins, bmi088_data.time_stamp_us, &bsx_input_acc, &bsx_input_gyo, &bsx_out_data);
}