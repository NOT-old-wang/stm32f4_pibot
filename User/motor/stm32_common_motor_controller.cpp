#include "stm32_common_motor_controller.h"
#include "board.h"
#include "data_holder.h"

CommonMotorController::CommonMotorController(int _num, unsigned short _pwm_max, bool _reverse): 
                num(_num), pwm_max(_pwm_max), reverse(_reverse){
}

void CommonMotorController::init(){
    Board::get()->motor_init(num, pwm_max);
    last_pwm = 0;
}

void CommonMotorController::control(short pwm){
    if (last_pwm == pwm)
        return;
    last_pwm = pwm;

    if (!reverse)
        Board::get()->motor_pwm(num, pwm);
    else
        Board::get()->motor_pwm(num, -pwm);
}