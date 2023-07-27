/*
 * aya_driver_GPIO.c
 *
 *  Created on: 2023年4月2日
 *      Author: KurehaTian
 */

#ifndef AYA_DRIVER_GPIO_C_
#define AYA_DRIVER_GPIO_C_
#include <aya_driver_head.h>

typedef enum // 枚举端口号
{
    A0 = 0x0001,
    A1 = 0x0002,
    A2 = 0x0004,
    A3 = 0x0008,
    A4 = 0x0010,
    A5 = 0x0020,
    A6 = 0x0040,
    A7 = 0x0080,
    B0 = 0x0101,
    B1 = 0x0102,
    B2 = 0x0104,
    B3 = 0x0108,
    B4 = 0x0110,
    B5 = 0x0120,
    B6 = 0x0140,
    B7 = 0x0180,
    C2 = 0x0204,
    C3 = 0x0208,
    C4 = 0x0210, // C0-C3用作JTAG
    C5 = 0x0220,
    C6 = 0x0240,
    C7 = 0x0280,
    D0 = 0x0301,
    D1 = 0x0302,
    D2 = 0x0304,
    D3 = 0x0308,
    D4 = 0x0310,
    D5 = 0x0320,
    D6 = 0x0340,
    D7 = 0x0380,
    E0 = 0x0401,
    E1 = 0x0402,
    E2 = 0x0404,
    E3 = 0x0408,
    E4 = 0x0410,
    E5 = 0x0520,
    F0 = 0x0501,
    F1 = 0x0502,
    F2 = 0x0504,
    F3 = 0x0508,
    F4 = 0x0510
} gpio_pin_enum;

typedef enum // 枚举端口方向
{
    GPI = 0x00,  // 定义管脚输入方向
    GPO = 0x01,  // 定义管脚输出方向
    GPHW = 0x02, // 定义管脚为某硬件功能
} gpio_dir_enum;

typedef enum // 枚举端口电平
{
    GPIO_LOW = 0x00,  // 定义管脚初始化电平为低
    GPIO_HIGH = 0x01, // 定义管脚初始化电平为高
} gpio_level_enum;

/*设置参数援引自TI官方库可以根据需求自己组合，可以根据需求自己组合
    组合方式：  STRENGTH <<16 | CONFIG ;
    STRENGTH:电流强度
    GPIO_STRENGTH_2MA 0x00000001    // 2mA 驱动电流
    GPIO_STRENGTH_4MA 0x00000002    // 4mA 驱动电流
    GPIO_STRENGTH_6MA 0x00000065    // 6mA 驱动电流
    GPIO_STRENGTH_8MA 0x00000066    // 8mA 驱动电流
    GPIO_STRENGTH_8MA_SC 0x0000006E // 8mA 驱动电流,边沿斜率限制
    GPIO_STRENGTH_10MA 0x00000075   // 10mA 驱动电流
    GPIO_STRENGTH_12MA 0x00000077   // 12mA 驱动电流

    CONFIG:配置方式
    GPIO_PIN_TYPE_STD       0x00000008  // 推挽
    GPIO_PIN_TYPE_STD_WPU   0x0000000A  // 推挽，弱上拉
    GPIO_PIN_TYPE_STD_WPD   0x0000000C  // 推挽，弱下拉
    GPIO_PIN_TYPE_OD        0x00000009  // 开漏
    GPIO_PIN_TYPE_ANALOG    0x00000000  // 模拟比较器
    GPIO_PIN_TYPE_WAKE_HIGH 0x00000208  // 高电平休眠唤醒
    GPIO_PIN_TYPE_WAKE_LOW  0x00000108  // 低电平休眠唤醒
*/

typedef enum
{
    GPI_PU = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPU, // 上拉输入
    GPI_PD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPD, // 下拉输入
    GPI_FF = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD,     // 浮空输入
    GPI_AIN= GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_ANALOG,  // 模拟输入

    GPO_PP = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD,     // 推挽输出
    GPO_PU = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPU, // 上拉推挽输出
    GPO_PD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPD, // 下拉推挽输出
    GPO_OD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_OD, // 开漏输出

} gpio_mode;




void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8_t dat, uint32_t mode);

void gpio_set_level(gpio_pin_enum pin, uint8_t dat);

uint8_t gpio_get_level(gpio_pin_enum pin);

void gpio_toggle_level(gpio_pin_enum pin);

void gpio_set_dir(gpio_pin_enum pin, gpio_dir_enum dir);


#endif /* AYANAMI_DRIVER_AYA_DRIVER_GPIO_C_ */
