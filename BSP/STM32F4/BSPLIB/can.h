#ifndef CAN_H
#define CAN_H

#ifdef __cplusplus
extern "C" {
#endif 

#include "stm32f4xx.h"

void PB_CAN_Init(uint8_t CAN_Channel , uint8_t GPIO_AF);
void PB_CANTX_Message(uint8_t CAN_Channel , uint8_t MY_ID, uint8_t Receiver_ID, uint8_t *TxBuf, uint8_t Length);

#ifdef __cplusplus
}
#endif 

#endif // #ifndef CAN_H


