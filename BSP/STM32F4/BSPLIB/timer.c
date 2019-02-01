

#ifdef __cplusplus
extern "C" {
#endif 

#include "timer.h"
#include "nvic.h"


void PB_Timer_Init(TIM_TypeDef* TIMx , uint32_t Time_us)
{
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIMx);
    if( TIMx == TIM1){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);  /* enable clock */
        TIM1_NVIC_Configuration();                             //enable interruptation
        TIM_TimeBaseStructure.TIM_Prescaler= (168 - 1);        /*prescaler frequency 1M*/
    }
    else if( TIMx == TIM2){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
        TIM2_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM3){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
        TIM3_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM4){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
        TIM4_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM5){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
        TIM5_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM6){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
        TIM6_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM7){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
        TIM7_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM8){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
        TIM8_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (168 - 1);
    }
    else if( TIMx == TIM9){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9 , ENABLE);
        TIM9_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (168 - 1);
    }
    else if( TIMx == TIM10){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10 , ENABLE);
        TIM10_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (168 - 1);
    }
    else if( TIMx == TIM11){
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11 , ENABLE);
        TIM11_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (168 - 1);
    }
    else if( TIMx == TIM12){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 , ENABLE);
        TIM12_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM13){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13 , ENABLE);
        TIM13_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    else if( TIMx == TIM14){
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14 , ENABLE);
        TIM14_NVIC_Configuration();
        TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);
    }
    
    TIM_TimeBaseStructure.TIM_Period= Time_us-1 ;               /* the value of auto reload register  */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ClockDivision*/
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   /* count upwards */
    TIM_TimeBaseInit(TIMx , &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIMx , TIM_FLAG_Update);                      /* clear interrupt flags */
    TIM_ITConfig(TIMx ,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIMx , ENABLE);
}	

#ifdef __cplusplus
}
#endif 

