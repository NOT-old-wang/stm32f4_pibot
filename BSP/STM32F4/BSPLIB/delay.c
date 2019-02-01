#ifdef __cplusplus
extern "C" {
#endif

#include "delay.h"

float count_time = 0;
float count_us = 0;
float count_ms = 0;
float reload = 0;

#if SYSTEM_SUPPORT_OS > 0u

#include "includes.h"

//support UCOSII
#ifdef 	OS_CRITICAL_METHOD
#define delay_ostickspersec	OS_TICKS_PER_SEC
#endif

//support UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD
#define delay_ostickspersec	OSCfg_TickRate_Hz
#endif

void PB_System_Timer_Init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    count_us=SystemCoreClock / 8000000;
    count_ms=(uint16_t)count_us * 1000;

    reload = SystemCoreClock / 8000000;
    reload *= 1000000/delay_ostickspersec;

    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD=reload; 						 
    //SysTick->VAL = reload;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

#else  //not using OS

void PB_System_Timer_Init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    count_us=SystemCoreClock / 8000000;
    count_ms=(uint16_t)count_us * 1000;
    reload = 16777215;

    SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD=reload; 						
    SysTick->VAL = reload;
    SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
}

#endif

void SysTick_Handler(void)
{
    count_time++;
    if(count_time >= 0xffffffff)
    {
        count_time=0;
    }
#if SYSTEM_SUPPORT_OS > 0u
    if(OSRunning ==1)
    {
        OSIntEnter();
        OSTimeTick();
        OSIntExit();
    }
#endif
}

float PB_Get_System_Time(void)
{
    float count , time;
    count = (float)( (reload + 1 - SysTick->VAL) + (float)(reload + 1) * count_time );
    time = count/count_us;
    return time;
}

float PB_Get_Dtime(void)
{
    static float lasttime ;
    float temp1,temp2;
    temp1 = PB_Get_System_Time();
    temp2 = temp1 - lasttime;
    if(temp2 < 0)
    {
        temp2 = ( ( (float)(reload + 1) * (float)0xffffffff) / count_us) - lasttime  + temp1;
    }
    lasttime = temp1;
    return temp2;
}

void delay_us(uint16_t t)
{
    int i;
    for( i=0 ; i<t ; i++)
    {
        int a = 25 ;
        while(a--) asm("nop");
    }
}

void delay_ms(uint16_t t)
{
    int i;
    for( i=0 ; i<t ; i++)
    {
        int a = 33568;
        while(a--) asm("nop");
    }
}

#ifdef __cplusplus
}
#endif

