#include "aya_device_encoder.h"

void encoder_init(encoder_index_t encoder_n, uint32_t max_position, encoder_dir_t dir)
{
    // ע�⣬Ӳ���Ա�������˫�߶����м��������Զ������������������λ����������4��(AB��˫����)

    // ���������DRL P445,
    // �ź�Դ��QEI_CONFIG_CAPTURE_A # QEI_CONFIG_CAPTURE_A_B
    // �Ƿ�λλ�ã�QEI_CONFIG_NO_RESET # QEI_CONFIG_RESET_IDX s
    // ����/����QEI_CONFIG_QUADRATURE # QEI_CONFIG_CLOCK_DIR s
    uint32_t configure = QEI_CONFIG_CAPTURE_A_B | QEI_CONFIG_NO_RESET | QEI_CONFIG_QUADRATURE;
    if (dir == encoder_dir_AB)
        configure |= QEI_CONFIG_NO_SWAP;
    else
        configure |= QEI_CONFIG_SWAP;

    if (encoder_n == encoder_0)
    {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI0);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

        // PD7����GPIO��������ǰҪ�Ƚ���
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= GPIO_PIN_7;
        // HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

        GPIOPinConfigure(GPIO_PD3_IDX0);
        GPIOPinConfigure(GPIO_PD6_PHA0);
        GPIOPinConfigure(GPIO_PD7_PHB0);
        GPIOPinTypeQEI(GPIO_PORTD_BASE, GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7);
        QEIDisable(QEI0_BASE);
        QEIConfigure(QEI0_BASE, configure, max_position);
        QEIPositionSet(QEI0_BASE, 0);
        QEIEnable(QEI0_BASE);
    }

    if (encoder_n == encoder_1)
    {
        SysCtlPeripheralEnable(SYSCTL_PERIPH_QEI1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

        GPIOPinConfigure(GPIO_PC4_IDX1);
        GPIOPinConfigure(GPIO_PC5_PHA1);
        GPIOPinConfigure(GPIO_PC6_PHB1);

        GPIOPinTypeQEI(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

        QEIDisable(QEI1_BASE);
        QEIConfigure(QEI1_BASE, configure, max_position);
        QEIPositionSet(QEI1_BASE, 0);
        QEIEnable(QEI1_BASE);
    }
}

void encoder_set_period(encoder_index_t encoder_n, uint32_t period_ms)
{

    if (encoder_n == encoder_0)
    {
        QEIVelocityDisable(QEI0_BASE);
        QEIVelocityConfigure(QEI0_BASE, QEI_VELDIV_1, SysCtlClockGet() * period_ms / 1000);
        QEIVelocityEnable(QEI0_BASE);
    }
    if (encoder_n == encoder_1)
    {
        QEIVelocityDisable(QEI1_BASE);
        QEIVelocityConfigure(QEI1_BASE, QEI_VELDIV_1, SysCtlClockGet() * period_ms / 1000);
        QEIVelocityEnable(QEI1_BASE);
    }
}

int32_t encoder_read_speed(encoder_index_t encoder_n)
{
    uint8_t dir;
    uint32_t speed;
    if (encoder_n == encoder_0)
    {
        dir = QEIDirectionGet(QEI0_BASE);
        speed = QEIVelocityGet(QEI0_BASE);
    }
    if (encoder_n == encoder_1)
    {
        dir = QEIDirectionGet(QEI1_BASE);
        speed = QEIVelocityGet(QEI1_BASE);
    }
    if (dir == 1)
        return speed;
    else
        return -speed;
}