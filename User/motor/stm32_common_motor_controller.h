#ifndef PIBOT_COMMON_MOTOR_CONTROLLER_H_
#define PIBOT_COMMON_MOTOR_CONTROLLER_H_

#include "motor_controller.h"

class CommonMotorController:public MotorController{
  public:
    CommonMotorController(int _num, unsigned short _pwm_max, bool _reverse=false);
    ~CommonMotorController(){}
    void init();
    void control(short pwm);
  private:
    int num;
    unsigned short pwm_max;
    short last_pwm;
    bool reverse;
};

#endif