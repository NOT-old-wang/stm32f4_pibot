#ifndef BSPLIB_H
#define	BSPLIB_H

#include "BSPLIB/nvic.h"
#include "BSPLIB/delay.h"
#include "BSPLIB/rtc.h"	 	
#include "BSPLIB/wdg_wkup.h"
#include "BSPLIB/flash.h"
#include "BSPLIB/usart.h"
#include "BSPLIB/adc_dac.h"
#include "BSPLIB/timer.h"
#include "BSPLIB/pwm_in.h" 
#include "BSPLIB/pwm_out.h"
#include "BSPLIB/encoder.h"
#include "BSPLIB/i2c.h"
#include "BSPLIB/can.h"
#include "BSPLIB/spi.h"

/**********************************************************************************************************************/
//compilation function
//__asm void WFI_SET(void)
//{
//    WFI;
//}
////close all interruption
//__asm void INTX_DISABLE(void)
//{
//    CPSID   I
//            BX      LR
//}
////enable all interruption
//__asm void INTX_ENABLE(void)
//{
//    CPSIE   I
//            BX      LR
//}
////set stack address
////addr:address of stack top
//__asm void MSR_MSP(u32 addr)
//{
//    MSR MSP, r0 			//set Main Stack value
//            BX r14
//}
/**********************************************************************************************************************/

#endif //#ifndef BSPLIB_H



