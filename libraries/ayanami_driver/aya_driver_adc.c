#include "aya_driver_adc.h"

/**     通道与引脚对应关系
 *      AIN0    PE3
 *      AIN1    PE2
 *      AIN2    PE1
 *      AIN3    PE0
 * ------------------目前只设计了AIN0-3的配置,AIN0被线性CCD占用(ADC0)，AIN1-3在此程序中进行连续读取(ADC1)
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
 * @brief ADC初始化
 * @note 目前只设计了AIN0-3的配置,AIN0被线性CCD占用(ADC0)，AIN1-3在此程序中进行连续读取(ADC1)
 */
void adc_init()
{
    // 对外设时钟进行初始化
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // 设置GPIO
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_2);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_0);

    // Full Speed:250k
    // 2-125k ,4-62.5k ,8-31.25k ,16-15.625k ,32-7.8125k ,64-3.90k

    // ADCHardwareOversampleConfigure(ADC1_BASE, 64); // 采样速率为系统时钟频率的1/64,用来降低采样速度换取精度

    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 2, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceDisable(ADC1_BASE, 0);
}

/**
 * @brief 读取ADC单通道数据
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
 * @brief 读取ADC
 *
 * @param data_out
 */
void adc_read(uint32_t *data_out)
{
    // 设置转换序列
    ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 0, ADC_CTL_CH2);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 1, ADC_CTL_CH1);
    ADCSequenceStepConfigure(ADC1_BASE, 0, 2, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);
    // 启用ADC转换序列
    ADCSequenceEnable(ADC1_BASE, 0);
    // 开始单次ADC转换
    ADCProcessorTrigger(ADC1_BASE, 0);
    // 等待转换完成
    while (!ADCIntStatus(ADC1_BASE, 0, false))
    {
    }
    // 读取转换结果
    ADCSequenceDataGet(ADC1_BASE, 0, data_out);
    // 清除中断标志
    ADCIntClear(ADC1_BASE, 0);
}

/**
 * @brief 对单通道数据进行转换
 *
 * @param raw_adc 原始adc数值，12位[0..4095]
 * @return float
 */
float adc_convert_ch(uint32_t raw_adc)
{
    float float_raw = raw_adc * 3.00f / 4096;
    // 此处可进行非线性变换等处理
    return float_raw;
}

/**
 * @brief 对所有3个通道的数据进行转换
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