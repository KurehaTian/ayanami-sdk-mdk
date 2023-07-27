#include "aya_driver_adc.h"

/**     ͨ�������Ŷ�Ӧ��ϵ
 *      AIN0    PE3
 *      AIN1    PE2
 *      AIN2    PE1
 *      AIN3    PE0
 * ------------------Ŀǰֻ�����AIN0-3������,AIN0������CCDռ��(ADC0)��AIN1-3�ڴ˳����н���������ȡ(ADC1)
 *      AIN4    PD3
 *      AIN5    PD2
 *      AIN6    PD1
 *      AIN7    PD0
 *      AIN8    PE5
 *      AIN9    PE4
 *      AIN10   PB4
 *      AIN11   PB5
 */

/**
 * @brief ADC��ʼ��
 * @note Ŀǰֻ�����AIN0-3������,AIN0������CCDռ��(ADC0)��AIN1-3�ڴ˳����н���������ȡ(ADC1)
 */
void adc_init()
{
    // ������ʱ�ӽ��г�ʼ��
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // ����GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

    // Full Speed:250k
    // 2-125k ,4-62.5k ,8-31.25k ,16-15.625k ,32-7.8125k ,64-3.90k

    // ADCHardwareOversampleConfigure(ADC1_BASE, 64); // ��������Ϊϵͳʱ��Ƶ�ʵ�1/64,�������Ͳ����ٶȻ�ȡ����

    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 2, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceDisable(ADC1_BASE, 0);
}

/**
 * @brief ��ȡADC��ͨ������
 *
 * @param ch [1..3]
 * @return uint32_t
 */
uint32_t adc_read_ch(uint8_t ch)
{
    if (ch < 1 || ch > 3)
        return -1;
    uint32_t data_buff[3];
    adc_read(data_buff);
    return data_buff[ch - 1];
}

/**
 * @brief ��ȡADC
 *
 * @param data_out
 */
void adc_read(uint32_t *data_out)
{
    // ����ת������
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 2, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    // ����ADCת������
    ADCSequenceEnable(ADC1_BASE, 0);
    // ��ʼ����ADCת��
    ADCProcessorTrigger(ADC1_BASE, 0);
    // �ȴ�ת�����
    while (!ADCIntStatus(ADC1_BASE, 0, false))
    {
    }
    // ��ȡת�����
    ADCSequenceDataGet(ADC1_BASE, 0, data_out);
    // ����жϱ�־
    ADCIntClear(ADC1_BASE, 0);
}

/**
 * @brief �Ե�ͨ�����ݽ���ת��
 *
 * @param raw_adc ԭʼadc��ֵ��12λ[0..4095]
 * @return float
 */
float adc_convert_ch(uint32_t raw_adc)
{
    float float_raw = raw_adc * 3.00f / 4096;
    // �˴��ɽ��з����Ա任�ȴ���
    return float_raw;
}

/**
 * @brief ������3��ͨ�������ݽ���ת��
 *
 * @param data_in
 * @param data_out
 */
void adc_convert(uint32_t *data_in, float *data_out)
{
    for (int i = 0; i < 3; i++)
    {
        *(data_out + i) = adc_convert_ch(*(data_in + i));
    }
}