/*
 * aya_driver_GPIO.c
 *
 *  Created on: 2023��4��2��
 *      Author: KurehaTian
 */

#ifndef AYA_DRIVER_GPIO_C_
#define AYA_DRIVER_GPIO_C_
#include <aya_driver_head.h>
#include <aya_device_head.h>
typedef enum // ö�ٶ˿ں�
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
    C4 = 0x0210, // C0-C3����JTAG
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

typedef enum // ö�ٶ˿ڷ���
{
    GPI = 0x00,  // ����ܽ����뷽��
    GPO = 0x01,  // ����ܽ��������
    GPHW = 0x02, // ����ܽ�ΪĳӲ������
} gpio_dir_enum;

typedef enum // ö�ٶ˿ڵ�ƽ
{
    GPIO_LOW = 0x00,  // ����ܽų�ʼ����ƽΪ��
    GPIO_HIGH = 0x01, // ����ܽų�ʼ����ƽΪ��
} gpio_level_enum;

/*���ò���Ԯ����TI�ٷ�����Ը��������Լ���ϣ����Ը��������Լ����
    ��Ϸ�ʽ��  STRENGTH <<16 | CONFIG ;
    STRENGTH:����ǿ��
    GPIO_STRENGTH_2MA 0x00000001    // 2mA ��������
    GPIO_STRENGTH_4MA 0x00000002    // 4mA ��������
    GPIO_STRENGTH_6MA 0x00000065    // 6mA ��������
    GPIO_STRENGTH_8MA 0x00000066    // 8mA ��������
    GPIO_STRENGTH_8MA_SC 0x0000006E // 8mA ��������,����б������
    GPIO_STRENGTH_10MA 0x00000075   // 10mA ��������
    GPIO_STRENGTH_12MA 0x00000077   // 12mA ��������

    CONFIG:���÷�ʽ
    GPIO_PIN_TYPE_STD       0x00000008  // ����
    GPIO_PIN_TYPE_STD_WPU   0x0000000A  // ���죬������
    GPIO_PIN_TYPE_STD_WPD   0x0000000C  // ���죬������
    GPIO_PIN_TYPE_OD        0x00000009  // ��©
    GPIO_PIN_TYPE_ANALOG    0x00000000  // ģ��Ƚ���
    GPIO_PIN_TYPE_WAKE_HIGH 0x00000208  // �ߵ�ƽ���߻���
    GPIO_PIN_TYPE_WAKE_LOW  0x00000108  // �͵�ƽ���߻���
*/

typedef enum
{
    GPI_PU = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPU, // ��������
    GPI_PD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPD, // ��������
    GPI_FF = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD,     // ��������
    GPI_AIN= GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_ANALOG,  // ģ������

    GPO_PP = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD,     // �������
    GPO_PU = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPU, // �����������
    GPO_PD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_STD_WPD, // �����������
    GPO_OD = GPIO_STRENGTH_4MA << 16 | GPIO_PIN_TYPE_OD, // ��©���

} gpio_mode;




void gpio_init(gpio_pin_enum pin, gpio_dir_enum dir, uint8_t dat, uint32_t mode);

void gpio_set_level(gpio_pin_enum pin, uint8_t dat);

uint8_t gpio_get_level(gpio_pin_enum pin);

void gpio_toggle_level(gpio_pin_enum pin);

void gpio_set_dir(gpio_pin_enum pin, gpio_dir_enum dir);


#endif /* AYANAMI_DRIVER_AYA_DRIVER_GPIO_C_ */
