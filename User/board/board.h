#ifndef BOARD_H_
#define BOARD_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "bsplib.h"
#ifdef __cplusplus
}
#endif

class Queue;

enum USART_NUMBER
{
    USART_0 = 0,
    USART_1,
    USART_2,
    USART_3,
    USART_4,
    USART_5,
    USART_6,
};


enum MOTOR_ID
{
    MOTOR_1 = 0,
    MOTOR_2,
    MOTOR_3,
    MOTOR_4
};

#define AT24XCC_ADDR_ 0xA0
#define MAX_EEPROM_SIZE 1024

class Board
{
public:

    virtual void init()=0;
    virtual void usart_init(unsigned char num, unsigned long buad)=0;
    virtual Queue* usart_getDataQueue(unsigned char num)=0;
    virtual void usart_write(unsigned char num, unsigned char ch)=0;
    virtual void usart_write(unsigned char num, unsigned char* data, unsigned char len)=0;
    virtual void update(void)=0;
    virtual void set_config(unsigned char* data, unsigned short len)=0;
    virtual void get_config(unsigned char* data, unsigned short len)=0;

    virtual unsigned long get_tick_count()=0;

    virtual void motor_init(unsigned char num, unsigned long period)=0;
    virtual void motor_pwm(unsigned char num, long pwm_value)=0;
public:
    virtual void setDO_state(uint8_t led_id, uint8_t operation) = 0;

    virtual void motor_enable(uint8_t mode , uint8_t motor_id ) = 0;
    virtual void motor_disable(uint8_t mode , uint8_t motor_id ) = 0;

    virtual void encoder_init(unsigned char num) = 0;
    virtual long get_encoder_count(unsigned char num) = 0;
public:
    float getClock(void)
    {
        return PB_Get_System_Time();
    }
    void usart_debug_init(void)
    {
        usart_init(USART_DEBUG , USART_DEBUG_BAUD);  //debug USART init
    }
    void usart_debug_putchar(uint8_t tx_byte)
    {
        usart_write(USART_DEBUG , tx_byte);
    }

    virtual void i2c_init(unsigned char num) = 0;
    virtual unsigned char i2c_write_byte(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char pt_char)=0;
    virtual unsigned char i2c_write_buf(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char , unsigned char size) = 0;
    virtual unsigned char i2c_read_byte(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char) = 0;
    virtual unsigned char i2c_read_buf(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char , unsigned char size) = 0;

    unsigned char i2c_write_bit(unsigned char num, unsigned char devAddr, unsigned char regAddr, unsigned char bitNum, unsigned char data){
        unsigned char b=0;
        i2c_read_byte(num, devAddr, regAddr, &b);
        b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
        unsigned char r = i2c_write_byte(num, devAddr, regAddr, b);

        i2c_read_byte(num, devAddr, regAddr, &b);

        return r;
    }

    unsigned char i2c_write_bits(unsigned char num, unsigned char devAddr, unsigned char regAddr, unsigned char bitStart, unsigned char length, unsigned char data) {
        unsigned char b;
        if (i2c_read_byte(num, devAddr, regAddr, &b) != 0) {
            unsigned char mask = ((1 << length) - 1) << (bitStart - length + 1);
            data <<= (bitStart - length + 1); // shift data into correct position
            data &= mask; // zero all non-important bits in data
            b &= ~(mask); // zero all important bits in existing byte
            b |= data; // combine data with existing byte
            unsigned char r =  i2c_write_byte(num, devAddr, regAddr, b);

            i2c_read_byte(num, devAddr, regAddr, &b);

            return r;
        } else {
            return 0;
        }
    }

    unsigned char i2c_read_bits(unsigned char num, unsigned char devAddr, unsigned char regAddr, unsigned char bitStart, unsigned char length, unsigned char *data) {
        unsigned char count, b;
        if ((count = i2c_read_byte(num, devAddr, regAddr, &b)) != 0) {
            unsigned char mask = ((1 << length) - 1) << (bitStart - length + 1);
            b &= mask;
            b >>= (bitStart - length + 1);
            *data = b;
        }
        return count;
    }
public:
    static Board* get();
};

#endif // BOARD_H
