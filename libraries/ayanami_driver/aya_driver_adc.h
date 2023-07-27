/*
 * aya_driver_adc.h
 *
 *  Created on: 2023Äê4ÔÂ2ÈÕ
 *      Author: KurehaTian
 */

#ifndef AYA_DRIVER_ADC_H_
#define AYA_DRIVER_ADC_H_
#include <aya_driver_head.h>

typedef enum
{
    adc_0,
    adc_1,
} adc_index_t;

void adc_init();
uint32_t adc_read_ch(uint8_t ch);
void adc_read(uint32_t *data_out);
float adc_convert_ch(uint32_t raw_adc);
void adc_convert(uint32_t *data_in, float *data_out);

#endif /* AYANAMI_DRIVER_AYA_DRIVER_ADC_H_ */
