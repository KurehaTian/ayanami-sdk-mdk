#include "aya_driver_pwm.h"

/*  ���Mainboard:
 *  0    MOTOR1      PE5     M0P5/M1P3
 *  1    MOTOR2      PF1     M1P5
 *  2    MOTOR3      PF2     M1P6
 *  3    MOTOR4      PF3     M1P7
 *  4    SERVO1      PD0     M0D6/M1P0
 *  5    SERVO2      PD1     M0P7/M1P1
 *  6    SERVO3      PE4     M0P4/M1P2
 */

void pwm_init(pwm_ch_t ch)
{
    switch (ch)
    {
    case pwm_motor1:
        // 1. ����PWMģ��ʱ��
        SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

        // 2. ����GPIO����
        GPIOPinConfigure(GPIO_PE5_M1PWM3);
        GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);

        // 3. ����PWMģ��ʱ�ӷ�Ƶ��Ƶ��
        SysCtlPWMClockSet(SYSCTL_PWMDIV_1); // ����PWMʱ��Ƶ��Ϊϵͳʱ�ӵ�1/64

        // 4. ����PWMģ��
        PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);

        // 5. ����PWM���ڣ�Ƶ�ʣ�
        uint32_t pwmPeriod = SysCtlClockGet()/ 333; // 1ms������
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, pwmPeriod);

        // 6. ���ó�ʼռ�ձ�
        uint32_t dutyCycle = pwmPeriod / 2; // 50%��ռ�ձ�
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, dutyCycle);

        // 7. ʹ��PWM���
        PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);

        // 8. ʹ��PWM������
        PWMGenEnable(PWM1_BASE, PWM_GEN_1);
        break;
    }
}

void pwm_set_freq(pwm_ch_t ch, uint32_t freq_hz)
{
    switch (ch)
    {
    case pwm_motor1:
        uint32_t oldPeriod = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1);
        uint32_t oldDutyCycle = PWMPulseWidthGet(PWM1_BASE, PWM_OUT_3);
        uint32_t newFrequency = freq_hz;
        uint32_t newPeriod = SysCtlClockGet() / 64 / newFrequency;
        uint32_t newDutyCycle = oldDutyCycle * newPeriod / oldDutyCycle;
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, newPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, newDutyCycle);
        break;
    }
}

void pwm_set_duty(pwm_ch_t ch, uint32_t duty_div_10k)
{
    switch (ch)
    {
    case pwm_motor1:
        uint32_t oldPeriod = PWMGenPeriodGet(PWM1_BASE, PWM_GEN_1);
        uint32_t newDutyCycle = oldPeriod * duty_div_10k / 10000;
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, newDutyCycle);
        break;
    }
}