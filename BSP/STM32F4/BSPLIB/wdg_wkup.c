

#ifdef __cplusplus
extern "C" {
#endif 

#include "wdg_wkup.h"
#include "delay.h"

void PB_IWDG_Init(void) 
{	
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 	//Enable access to write register
    IWDG_SetPrescaler(IWDG_Prescaler_64);
    IWDG_SetReload(500);							//Frequency: 40K / 64 =  0.625K, and one cycle is 1ms
    IWDG_ReloadCounter();			//the program to feed dog, if not write 0xAAAA every while, WDG will reset
    IWDG_Enable(); 					//enable
}

void PB_IWDG_Feed(void)
{   
    IWDG_ReloadCounter();
}

static void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;             //WWDG interrupt
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //PreemptionPriority
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	        //SubPriority
    NVIC_Init(&NVIC_InitStructure);//NVIC initialize
}

uint8_t WWDG_CNT=0x7f;

void PB_WWDG_Init(uint8_t tr,uint8_t wr,uint32_t fprer)
{ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);   //enable WWDG clock
    WWDG_SetPrescaler(fprer);   //set IWDG Prescaler
    WWDG_SetWindowValue(wr);    //set window value
    WWDG_Enable(tr);	        //enable WWDG, and set counter
    WWDG_ClearFlag();
    WWDG_NVIC_Init();       //initialize WWDG NVIC
    WWDG_EnableIT();        //enable interrupt of WWDG
} 

void PB_WWDG_Set_Counter(uint8_t cnt)
{
    WWDG_Enable(cnt);
}

void PB_WWDG_IRQHandler(void)
{
    WWDG_SetCounter(0x7F);	  //if annotate this words, WWDG will reset
    WWDG_ClearFlag();	        //clear flag of advance wake up
}


#define WKUP_KD GPIO_ReadInputDataBit(GPIOA  , GPIO_Pin_0)		//PA0 check signal of WKUP Pin

static void Sys_Enter_Standby(void)
{		
    //  while(WKUP_KD);			//waiting for free WKUP KEY
    RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//RESET all IO
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//enable PWR clock
    PWR_BackupAccessCmd(ENABLE);//enable access to backup register
    
    RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//disable RTC interrupt
    RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//clear RTC interrupt flag
    
    PWR_ClearFlag(PWR_FLAG_WU);//clear Wake-up flag
    PWR_WakeUpPinCmd(ENABLE);  //set WKUP key to wake-up system
    PWR_EnterSTANDBYMode();	   //enter standby mode
}

uint8_t Check_WKUP(void) 
{
    uint8_t t=0;
    uint8_t tx=0;//recording number of free key
    while(1)
    {
        if(WKUP_KD)//have pressed
        {
            t++;
            tx=0;
        }
        else
        {
            tx++;
            if(tx>3)    //no WKUP signal over 90ms
            {
                return 0; //error press, number of press not enough
            }
        }
        delay_ms(30);
        if(t>=100)//having been pressed over 3s
        {
            return 1; //having been pressed over 3s
        }
    }
} 


//void EXTI0_IRQHandler(void)
//{ 		    		    				     		    
//    EXTI_ClearITPendingBit(EXTI_Line0); // clear interrupt flag of Interrupt Line10 
//    if( Check_WKUP() )//power-off
//    {
//        PB_Sys_Enter_Standby(); //enter standby mode
//    }
//} 

void PB_WKUP_Init(void)
{	  
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//input mode
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//pull down mode
    GPIO_Init(GPIOA, &GPIO_InitStructure);//initialize
    
    //check if power on normally
    if(Check_WKUP()==0)
    {
        Sys_Enter_Standby();	//not power-on, enter standby
    }
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 connect to interrupt line0
    EXTI_InitStructure.EXTI_Line = EXTI_Line0; //LINE0
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //interrupt event
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //touch off rising
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; //enable interrupt LINE0
    EXTI_Init(&EXTI_InitStructure); //config
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //external interrupt 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02; //enable external interrupt channel
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); //config NVIC
}

#ifdef __cplusplus
}
#endif 


