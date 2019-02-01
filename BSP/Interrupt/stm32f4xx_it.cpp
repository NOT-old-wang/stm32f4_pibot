#include "board.h"
#include "os_include.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_it.h"
#include "queue.h"

void USART1_IRQHandler(void)
{
    unsigned char data =0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(USART1->SR&(1<<5))
    {
        data=USART1->DR;
        
        Board::get()->usart_getDataQueue(USART_0)->put(data);
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);   // clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void USART2_IRQHandler(void)
{
    unsigned char data = 0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(USART2);
        /*if( Board::get()->getUsartQueue(2)->fullCheck() ==0 ){
            Board::get()->getUsartQueue(2)->putData(data);
        }*/
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);  // clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void USART3_IRQHandler(void)
{
    unsigned char data = 0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        data=USART_ReceiveData(USART3);
        /*if( Board::get()->getUsartQueue(3)->fullCheck() ==0 ){
            Board::get()->getUsartQueue(3)->putData(data);
        }*/
        USART_ClearITPendingBit(USART3,USART_IT_RXNE);  //clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void UART4_IRQHandler(void)
{
    unsigned char data = 0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(UART4);
        /*if( Board::get()->getUsartQueue(4)->fullCheck() ==0 ){
            Board::get()->getUsartQueue(4)->putData(data);
        }*/
        USART_ClearITPendingBit(UART4, USART_IT_RXNE);     //clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void UART5_IRQHandler(void)
{
    unsigned char data = 0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(UART5);
        /*if( Board::get()->getUsartQueue(5)->fullCheck() ==0 ){
            Board::get()->getUsartQueue(5)->putData(data);
        }*/
        USART_ClearITPendingBit(UART5 , USART_IT_RXNE);   //clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void USART6_IRQHandler(void)
{
    unsigned char data = 0;
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(USART6);
        /*if( Board::get()->getUsartQueue(6)->fullCheck() ==0 ){
            Board::get()->getUsartQueue(6)->putData(data);
        }*/
        USART_ClearITPendingBit(USART6 , USART_IT_RXNE);   //clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void TIM6_DAC_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
    if(TIM_GetITStatus(TIM6 , TIM_IT_Update)== SET  )
    {
        /*Board::get()->cnt_1ms++;
        Board::get()->cnt_2ms++;
        Board::get()->cnt_5ms++;
        Board::get()->cnt_10ms++;
        Board::get()->cnt_20ms++;
        Board::get()->cnt_50ms++;*/
        TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);     // clear interrupt flag
    }
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void HardFault_Handler(void)
{
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif

    //Board::get()->setBeepModel(3); // 10hz

    //	__disable_fault_irq();  //reset
    //	NVIC_SystemReset();
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

void EXTI9_5_IRQHandler(void)
{     
#if SYSTEM_SUPPORT_OS == 1
    OSIntEnter();
#endif
	/*延时消抖*/
    
#ifdef UKEY_USE_EX_INT
  	/*if(EXTI_GetITStatus(EXTI_Line7) != RESET)	  
    {	  
        printf("emergency key pressed\r\n");
        Data_holder::get()->robot_key_output._emergency_key = 1;
        //GPIO_WriteBit(GPIOF, GPIO_Pin_10, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_10))));
    }

      if(EXTI_GetITStatus(EXTI_Line8) != RESET)	  
    {	  
        printf("power key pressed\r\n");
        Data_holder::get()->robot_key_output._power_key = 1-Data_holder::get()->robot_key_output._power_key;
        //GPIO_WriteBit(GPIOF, GPIO_Pin_10, (BitAction)(1-(GPIO_ReadOutputDataBit(GPIOF, GPIO_Pin_10))));
    }*/
	/*清除EXTI0线路挂起位*/
	EXTI_ClearITPendingBit(EXTI_Line7);  
    EXTI_ClearITPendingBit(EXTI_Line8);  
#endif
#if SYSTEM_SUPPORT_OS == 1
    OSIntExit();
#endif
}

#ifdef __cplusplus
}
#endif
