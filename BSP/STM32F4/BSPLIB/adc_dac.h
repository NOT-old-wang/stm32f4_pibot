#ifndef ADC_DAC_H
#define ADC_DAC_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"

void PB_ADC_Init(uint16_t adc_channel , uint8_t adc_num , float Reference_Voltage_);
float PB_Get_ADC_Output(uint8_t n);

void PB_DAC_Init(uint8_t channel_x);
void PB_DAC_Set_Vol(uint8_t channel_x , uint16_t vol);

#ifdef __cplusplus
}
#endif 

#endif // #ifndef ADC_DAC_H

