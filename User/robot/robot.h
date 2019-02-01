#ifndef PIBOT_ROBOT_H_
#define PIBOT_ROBOT_H_

#include "dataframe.h"

#if ROBOT_MODEL == ROBOT_DIFF_2WD
#include "differential2wd.h"
#endif

#if ROBOT_MODEL == ROBOT_DIFF_4WD
#include "differential4wd.h"
#endif

#if ROBOT_MODEL == ROBOT_OMNI_3
#include "omni3.h"
#endif

#if ROBOT_MODEL == ROBOT_MECANUM
#include "mecanum.h"
#endif

class MotorController;
class Encoder;
class PID;
class Transport;
class Dataframe;
class Model;
class IMU;

class Robot:public Notify{
    public:
        static Robot* get(){
            static Robot robot;
            return &robot;
        }
        void init();

        void check_command();
        void do_kinmatics();
        void calc_odom();

        void get_imu_data();
        void update(const MESSAGE_ID id, void* data);
    private:
        Robot(){}
        void init_motor();
        void init_trans();
        void init_imu();
        void init_joystick();
    private:
        void clear_odom();
        void update_velocity();
    private:
        MotorController* motor[MOTOR_COUNT];
        Encoder*        encoder[MOTOR_COUNT];
        PID*            pid[MOTOR_COUNT];
        float           input[MOTOR_COUNT];
        float           feedback[MOTOR_COUNT];

        Transport*      trans;
        Dataframe*      frame;
        Model*          model;
        bool            do_kinmatics_flag;

        Odom            odom;

        unsigned long   last_velocity_command_time;
#if IMU_ENABLE
        IMU* imu;
#endif
};

#endif