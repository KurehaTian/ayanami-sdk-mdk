#include "aya_driver_pwm.h"

/*  ���Mainboard:      ����ʹ��M1,
 *  0    MOTOR1      PE5     M0P5/M1P3  Gen2/Gen1
 *  1    MOTOR2      PF1     M1P5            Gen2
 *  2    MOTOR3      PF2     M1P6            Gen3
 *  3    MOTOR4      PF3     M1P7            Gen3
 *  4    SERVO1      PD0     M0P6/M1P0  Gen3/Gen0
 *  5    SERVO2      PD1     M0P7/M1P1  Gen3/Gen0
 *  6    SERVO3      PE4     M0P4/M1P2  Gen2/Gen1
 *       PWM��2������Ϊ���η�������ţ�ͬһ�����������Ĳ���Ƶ����ͬ��
 */

void pwm_init(pwm_ch_t ch, uint32_t freq, uint32_t duty)
{
    // ����PWMģ��ʱ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    // ����PWMʱ��Ƶ��Ϊϵͳʱ�ӵ�1/64
    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);

    uint32_t pwmPeriod = SysCtlClockGet() / 4 / freq;
    uint32_t dutyCycle = duty * pwmPeriod / 10000;

    switch (ch)
    {
    case pwm_motor1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinConfigure(GPIO_PE5_M1PWM3);
        GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_3, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_3_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_1);
        break;
    case pwm_motor2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinConfigure(GPIO_PF1_M1PWM5);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_2);
        break;
    case pwm_motor3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinConfigure(GPIO_PF2_M1PWM6);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_2);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_3);
        break;
    case pwm_motor4:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinConfigure(GPIO_PF3_M1PWM7);
        GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_3);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_3);
        break;
    case pwm_servo1:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinConfigure(GPIO_PD0_M1PWM0);
        GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_0);
        break;
    case pwm_servo2:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
        GPIOPinConfigure(GPIO_PD1_M1PWM1);
        GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
        PWMGenEnable(PWM1_BASE, PWM_GEN_0);
        break;
    case pwm_servo3:
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
        GPIOPinConfigure(GPIO_PE4_M1PWM2);
        GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
        PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN);
        PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, pwmPeriod);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, dutyCycle);
        PWMOutputState(PWM1_BASE, PWM_OUT_2_BIT, true);
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
        uint32_t newPeriod = SysCtlClockGet() / 4 / newFrequency;
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