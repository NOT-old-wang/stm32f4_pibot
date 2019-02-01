#include "board_stm32.h"

#define ROBOT_PARARMETER_START 256

Board* Board::get()
{
    static Board_STM32 instance;
    return &instance;
 }

Board_STM32::Board_STM32()
{ 
    system_time = 0;
    
    board_call_5ms = board_call_20ms = board_call_1s = board_call_2s =  0 ;
}

void Board_STM32::init()
{
    int i;
    for(i=0;i<0x8fff;i++);

    system_clock_init();
    usart_debug_init();

    DOInit();

    eeprom_init();
}

void Board_STM32::usart_init(unsigned char num, unsigned long buad){
    if (num == USART_0){
        PB_USART_Init(1 , buad , 0);
    }else if (num == USART_2){
        PB_USART_Init(3 , buad , 0);
    }
}

Queue* Board_STM32::usart_getDataQueue(unsigned char num){
    if (num == USART_0){
        return &usart0_queue;
    }else if (num == USART_2){
        
    }
    
    return 0;
}

void Board_STM32::usart_write(unsigned char num, unsigned char ch){
    if (num == USART_0){
        PB_USART_Put_Char(1 , ch);
    }else if (num == USART_2){
        PB_USART_Put_Char(3 , ch);
    }
}

void Board_STM32::usart_write(unsigned char num, unsigned char* data, unsigned char len){
	while(len--){
		usart_write(num, *data++);
	}
}

void Board_STM32::set_config(unsigned char* data, unsigned short len){
    eeprom_write(ROBOT_PARARMETER_START, (char*)data, len);
}

void Board_STM32::get_config(unsigned char* data, unsigned short len){
    eeprom_read(ROBOT_PARARMETER_START, (char*)data, len);
}

unsigned long Board_STM32::get_tick_count(){
	return PB_Get_System_Time()/1000;
}

void Board_STM32::motor_init(unsigned char num, unsigned long period){
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;

    if(num == MOTOR_1){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
        GPIO_Init(GPIOE , &GPIO_InitStructure);
        GPIO_SetBits(GPIOE , GPIO_Pin_8);
        GPIO_SetBits(GPIOE , GPIO_Pin_11);
        
        PB_PWMChannel_Init(TIM1, 1, 2-1, period, 1);
    }
    else if(num == MOTOR_2){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_14 ;
        GPIO_Init(GPIOE , &GPIO_InitStructure);
        GPIO_SetBits(GPIOE , GPIO_Pin_12);
        GPIO_SetBits(GPIOE , GPIO_Pin_14);
        PB_PWMChannel_Init(TIM1, 3, 2-1, period, 1);
    }
    else if(num == MOTOR_3){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
        GPIO_Init(GPIOE , &GPIO_InitStructure);
        GPIO_SetBits(GPIOE , GPIO_Pin_4);
        GPIO_SetBits(GPIOE, GPIO_Pin_6);
        PB_PWMChannel_Init(TIM9 ,1, 2-1, period, 0);
    }
    else if(num == MOTOR_4){
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
        GPIO_Init(GPIOE , &GPIO_InitStructure);
        GPIO_SetBits(GPIOE , GPIO_Pin_15);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
        GPIO_Init(GPIOB , &GPIO_InitStructure);
        GPIO_SetBits(GPIOB , GPIO_Pin_15);
        PB_PWMChannel_Init(TIM12, 1, 0, period, 0);
    }
}

void Board_STM32::motor_pwm(unsigned char num, long pwm_value){
    if (num == MOTOR_1){
        if (pwm_value > 5) {
            GPIO_SetBits(GPIOE , GPIO_Pin_11);
            GPIO_ResetBits(GPIOE , GPIO_Pin_8);
            PB_Set_PWM(TIM1 , 1 , (uint16_t)pwm_value);
            return;
        }
        else if(pwm_value < -5){
            GPIO_ResetBits(GPIOE, GPIO_Pin_11);
            GPIO_SetBits(GPIOE , GPIO_Pin_8);

            PB_Set_PWM(TIM1 , 1 , (uint16_t)-pwm_value);
            return;
        }
        else{
            GPIO_SetBits(GPIOE , GPIO_Pin_11);
            GPIO_SetBits(GPIOE , GPIO_Pin_8);
            PB_Set_PWM(TIM1 , 1 , 0);
            return;
        }
    }
    else if (num == MOTOR_2){
        if (pwm_value > 5) {
            GPIO_SetBits(GPIOE , GPIO_Pin_12);
            GPIO_ResetBits(GPIOE , GPIO_Pin_14);
            PB_Set_PWM(TIM1 , 3 , (uint16_t)pwm_value);
            return;
        }
        else if(pwm_value < -5){
            GPIO_ResetBits(GPIOE , GPIO_Pin_12);
            GPIO_SetBits(GPIOE , GPIO_Pin_14);
            PB_Set_PWM(TIM1 , 3, (uint16_t)-pwm_value);
            return;
        }
        else {
            PB_Set_PWM(TIM1 , 3 , 0);
            GPIO_SetBits(GPIOE , GPIO_Pin_12);
            GPIO_SetBits(GPIOE , GPIO_Pin_14);
            return;
        }
    }

    else if (num == MOTOR_3){
        if( pwm_value > 5) {
            GPIO_SetBits(GPIOE , GPIO_Pin_4);
            GPIO_ResetBits(GPIOE , GPIO_Pin_6);
            PB_Set_PWM(TIM9 , 1 , (uint16_t)pwm_value);
            return;
        }
        else if(pwm_value < -5){
            GPIO_ResetBits(GPIOE , GPIO_Pin_4);
            GPIO_SetBits(GPIOE , GPIO_Pin_6);
            PB_Set_PWM(TIM9 , 1 , (uint16_t)-pwm_value);
            return;
        }
        else{
            GPIO_SetBits(GPIOE , GPIO_Pin_4);
            GPIO_SetBits(GPIOE , GPIO_Pin_6);
            PB_Set_PWM(TIM9 , 1 , 0);
            return;
        }
    }
    else if(num == MOTOR_4){
        if( pwm_value > 5) {
            GPIO_SetBits(GPIOE , GPIO_Pin_15);
            GPIO_ResetBits(GPIOB , GPIO_Pin_15);
            PB_Set_PWM(TIM12 , 1 , (uint16_t)pwm_value);
            return;
        }
        else if(pwm_value < -5){
            GPIO_ResetBits(GPIOE , GPIO_Pin_15);
            GPIO_SetBits(GPIOB , GPIO_Pin_15);
            PB_Set_PWM(TIM12 , 1 , (uint16_t)-pwm_value);
            return;
        }
        else{
            GPIO_SetBits(GPIOE , GPIO_Pin_15);
            GPIO_SetBits(GPIOB , GPIO_Pin_15);
            PB_Set_PWM(TIM12 , 1 , 0);
            return;
        }
    }
}

void Board_STM32::DOInit(void){
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15 ;
    GPIO_Init(GPIOG , &GPIO_InitStruct);
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_Init(GPIOE , &GPIO_InitStruct);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC , &GPIO_InitStruct);
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_9;
    GPIO_Init(GPIOF , &GPIO_InitStruct);
}

void Board_STM32::setDO_state(uint8_t _id, uint8_t operation){
    if ( _id == _RUN_LED){
        if(operation == 0){ GPIO_SetBits(GPIOG , GPIO_Pin_15);}
        else if(operation == 1) { GPIO_ResetBits(GPIOG , GPIO_Pin_15);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOG , GPIO_Pin_15);}
    }
    else if ( _id == _5_CONTROL){
        if(operation == 0){ GPIO_SetBits(GPIOE , GPIO_Pin_1);}
        else if(operation == 1) { GPIO_ResetBits(GPIOE , GPIO_Pin_1);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOE , GPIO_Pin_1);}
    }
    else if ( _id == _12_CONTROL){
        if(operation == 0){ GPIO_SetBits(GPIOE , GPIO_Pin_2);}
        else if(operation == 1) { GPIO_ResetBits(GPIOE , GPIO_Pin_2);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOE , GPIO_Pin_2);}
    }
    else if(_id == _19_CONTROL){
        if(operation == 0){ GPIO_SetBits(GPIOE , GPIO_Pin_3);}
        else if(operation == 1) { GPIO_ResetBits(GPIOE , GPIO_Pin_3);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOE , GPIO_Pin_3);}
    }
    else if(_id == _24_CONTROL){
        if(operation == 0){ GPIO_SetBits(GPIOC , GPIO_Pin_13);}
        else if(operation == 1) { GPIO_ResetBits(GPIOC , GPIO_Pin_13);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOC , GPIO_Pin_13);}
    }
    else if(_id == _WIFI_RST){
        if(operation == 0){ GPIO_SetBits(GPIOE , GPIO_Pin_0);}
        else if(operation == 1) { GPIO_ResetBits(GPIOE , GPIO_Pin_0);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOE , GPIO_Pin_0);}
    }
    else if(_id == _48_CONTROL){
        if(operation == 0){ GPIO_SetBits(GPIOF , GPIO_Pin_6);}
        else if(operation == 1) { GPIO_ResetBits(GPIOF , GPIO_Pin_6);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOF , GPIO_Pin_6);}
    }
    else if(_id == _IMU_RST){
        if(operation == 0){ GPIO_SetBits(GPIOF , GPIO_Pin_9);}
        else if(operation == 1) { GPIO_ResetBits(GPIOF , GPIO_Pin_9);}
        else if(operation == 2) { GPIO_ToggleBits(GPIOF , GPIO_Pin_9);}
    }
}

void Board_STM32::motor_enable(uint8_t mode , uint8_t motor_id)
{
    if(mode == 0)
    {
        if(motor_id == 1 ){
            GPIO_SetBits(GPIOE , GPIO_Pin_8);
        }
        else if(motor_id == 2){
            GPIO_SetBits(GPIOE , GPIO_Pin_12);
        }
        else if(motor_id == 3){
            GPIO_SetBits(GPIOE , GPIO_Pin_4);
        }
        else if(motor_id == 4){
            GPIO_SetBits(GPIOE , GPIO_Pin_15);
        }
    }
    else if(mode == 1)
    {

    }
}

void Board_STM32::motor_disable(uint8_t mode , uint8_t motor_id)
{
    if(mode == 0)
    {
        if(motor_id == 1 ){
            GPIO_ResetBits(GPIOE , GPIO_Pin_8);
        }
        else if(motor_id == 2){
            GPIO_ResetBits(GPIOE , GPIO_Pin_12);
        }
        else if(motor_id == 3){
            GPIO_ResetBits(GPIOE , GPIO_Pin_4);
        }
        else if(motor_id == 4){
            GPIO_ResetBits(GPIOE , GPIO_Pin_15);
        }
    }
    else if(mode == 1)
    {
        if(motor_id == 1 ){
            GPIO_ResetBits(GPIOE , GPIO_Pin_8);
            GPIO_ResetBits(GPIOE , GPIO_Pin_11);
        }
        else if(motor_id == 2){
            GPIO_ResetBits(GPIOE , GPIO_Pin_12);
            GPIO_ResetBits(GPIOE , GPIO_Pin_14);
        }
        else if(motor_id == 3){
            GPIO_ResetBits(GPIOE , GPIO_Pin_4);
            GPIO_ResetBits(GPIOE , GPIO_Pin_6);
        }
        else if(motor_id == 4){
            GPIO_ResetBits(GPIOE , GPIO_Pin_15);
            GPIO_ResetBits(GPIOB , GPIO_Pin_15);
        }
    }
}

void Board_STM32::encoder_init(unsigned char num){
    if(num == MOTOR_1){
        PB_Encoder_Init(TIM2,1);
    }
    else if(num == MOTOR_2){
        PB_Encoder_Init(TIM3,1);
    }
    else if(num == MOTOR_3){
        PB_Encoder_Init(TIM4,1);
    }
    else if(num == MOTOR_4){
        PB_Encoder_Init(TIM5,0);
    }
}
            
long Board_STM32::get_encoder_count(unsigned char num){
    if(num == MOTOR_1){
        return PB_Get_Encode_TIM2();
    }
    else if(num == MOTOR_2){
        return PB_Get_Encode_TIM3();
    }
    else if(num == MOTOR_3){
        return PB_Get_Encode_TIM4();
    }
    else if(num == MOTOR_4){
        return PB_Get_Encode_TIM5();
    }
    return 0;
}

void Board_STM32::update(void)   //100HZ
{
    board_call_5ms++;
    board_call_20ms++;
    board_call_1s++;
    board_call_2s++;

    if(board_call_5ms >= 5) //20hz
    {
        board_call_5ms = 0;
        system_time = getClock();
    }
    if(board_call_20ms >= 20) //5HZ
    {
        board_call_20ms = 0;
    }
    if(board_call_1s >= 100) //1hz
    {
        board_call_1s=0;
    }
    if(board_call_2s >= 200) //wait 2s for a stable battery_voltage
    {
        board_call_2s = 0;
    }
}

void Board_STM32::system_clock_init(void)
{
    PB_System_Timer_Init();
}

void Board_STM32::eeprom_init()
{
    i2c_init(IIC_AT24CXX);
}

void Board_STM32::eeprom_read(unsigned short offset, char* buff, unsigned short len)
{
    if (offset>=MAX_EEPROM_SIZE || len>=MAX_EEPROM_SIZE || offset+len>MAX_EEPROM_SIZE)
        return;

    for(int i=offset; i<offset+len; i++)
    {
        i2c_read_byte(IIC_AT24CXX, AT24XCC_ADDR_+((i/256)<<1), i%256, (unsigned char*)(&buff[i-offset]));
    }
}

void Board_STM32::eeprom_write(unsigned short offset, char* buff, unsigned short len)
{
    if (offset>=MAX_EEPROM_SIZE || len>=MAX_EEPROM_SIZE || offset+len>MAX_EEPROM_SIZE)
        return;

    for(int i=offset; i<offset+len; i++)
    {
        i2c_write_byte(IIC_AT24CXX,AT24XCC_ADDR_+((i/256)<<1),i%256,buff[i-offset]);
    }
}

void Board_STM32::i2c_init(unsigned char num)
{
    PB_I2C_Init(num) ;
}

unsigned char Board_STM32::i2c_write_byte(unsigned char num, unsigned char addr, unsigned char reg , unsigned char pt_char)
{
    PB_I2C_Write_Byte(num, addr, reg, pt_char);
    return 1;
}


unsigned char Board_STM32::i2c_write_buf(unsigned char num, unsigned char addr, unsigned char reg , unsigned char* pt_char , unsigned char size)
{
    PB_I2C_Write_Buf(num, addr, reg, (uint8_t*)pt_char, size);
    return 1;
}

unsigned char Board_STM32::i2c_read_byte(unsigned char num, unsigned char addr, unsigned char reg , unsigned char* pt_char)
{
    return PB_I2C_Read_Byte(num, addr, reg, pt_char) ;
}

unsigned char Board_STM32::i2c_read_buf(unsigned char num, unsigned char addr, unsigned char reg , unsigned char* pt_char , unsigned char size)
{
    return PB_I2C_Read_Buf(num, addr, reg, pt_char, size);
}
