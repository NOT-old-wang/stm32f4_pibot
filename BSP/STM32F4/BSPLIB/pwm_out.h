#ifndef PWM_OUT_H
#define PWM_OUT_H

#ifdef __cplusplus
extern "C" {
#endif 
	
#include "stm32f4xx.h"
	
//PWM initialization, parameter TIMx=TIM1~12  Channel=1,2,3,4
//default Prescaler :72-1  frequency:1MHZ

void PB_PWMChannel_Init(TIM_TypeDef* TIMx , uint8_t Channel , uint16_t Prescaler, uint16_t Period, uint8_t GPIO_AF);

void PB_Set_PWM(TIM_TypeDef* TIMx , uint8_t Channel , uint16_t Pwm_Value);

#ifdef __cplusplus
}
#endif 

#endif // #ifndef PWM_OUT_H


