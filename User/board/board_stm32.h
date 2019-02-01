#ifndef BOARD_H
#define	BOARD_H

#include "board.h"
#include "variable_queue.h"

#define _RUN_LED          0
#define _5_CONTROL          1
#define _12_CONTROL          2
#define _19_CONTROL          3
#define _24_CONTROL          4
#define _WIFI_RST          5
#define _48_CONTROL          6
#define _IMU_RST          7

class Board_STM32 : public Board{
public:
    Board_STM32();

    void init();
    void usart_init(unsigned char num, unsigned long buad);
    Queue* usart_getDataQueue(unsigned char num);
    void usart_write(unsigned char num, unsigned char ch);
    void usart_write(unsigned char num, unsigned char* data, unsigned char len);
    void update(void);
    
    void set_config(unsigned char* data, unsigned short len);
    void get_config(unsigned char* data, unsigned short len);

    unsigned long get_tick_count();

    void motor_init(unsigned char num, unsigned long period);
    void motor_pwm(unsigned char num, long pwm_value);
public:
    void setDO_state(uint8_t _id, uint8_t operation);
    void motor_enable(uint8_t mode , uint8_t motor_id);
    void motor_disable(uint8_t mode , uint8_t motor_id);

    void encoder_init(unsigned char num);
    long get_encoder_count(unsigned char num);

    void i2c_init(unsigned char num);
    unsigned char i2c_write_byte(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char pt_char);
    unsigned char i2c_write_buf(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char , unsigned char size);
    unsigned char i2c_read_byte(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char);
    unsigned char i2c_read_buf(unsigned char num, unsigned char equipment_address, unsigned char reg_address , unsigned char* pt_char, unsigned char size);
public:
    
    void DOInit(void);
    
    void system_clock_init(void);

    void eeprom_init();
    void eeprom_read(unsigned short offset, char* buff, unsigned short len);
    void eeprom_write(unsigned short offset, char* buff, unsigned short len);
public:
    float system_time;
    uint16_t board_call_5ms , board_call_20ms , board_call_1s , board_call_2s;

    VQueue<256> usart0_queue;
};

#endif // #ifndef BOARD_H

