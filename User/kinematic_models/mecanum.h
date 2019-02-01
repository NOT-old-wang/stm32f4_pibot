#ifndef PIBOT_MECANUM_H_
#define PIBOT_MECANUM_H_

#include "model.h"
#include "math.h"
#define MOTOR_COUNT 4
/*
		  x
          ^
          |
          |
          |
y<--------
3-------------------2
-                   -
-                   -
-                   -
-                   -
-                   -
-                   -
-                   -
-                   -
-                   -
-                   -
4-------------------1
*/
class Mecanum:public Model{
public:
	Mecanum() {}
	Mecanum(float _wheel_radius, float _body_radius) : Model(_wheel_radius, _body_radius) {}
	
	void motion_solver(float* robot_speed, float* motor_speed){
		//	robot_speed[0] x	robot_speed[1] y	robot_speed[2] z
		motor_speed[2] = (-robot_speed[0] + robot_speed[1]*0.5 + robot_speed[2] * body_radius) / wheel_radius;//front-left motor
		motor_speed[1] = ( robot_speed[0] + robot_speed[1]*0.5 + robot_speed[2] * body_radius) / wheel_radius;//front-right motor
		motor_speed[3] = (-robot_speed[0] - robot_speed[1]*0.5 + robot_speed[2] * body_radius) / wheel_radius;//rear-left motor
		motor_speed[0] = ( robot_speed[0] - robot_speed[1]*0.5 + robot_speed[2] * body_radius) / wheel_radius;//rear-right motor
    }

	void get_odom(struct Odom* odom, float* motor_dis, unsigned long interval){
		if (motor_dis[0]!=0 || motor_dis[1]!=0 || motor_dis[2]!=0 || motor_dis[3]!=0){
			//speed
			float dvx = (-motor_dis[2]+motor_dis[1]-motor_dis[3]+motor_dis[0])/4.0f;
			float dvy = (motor_dis[2]+motor_dis[1]-motor_dis[3]-motor_dis[0])/4.0f;
			float dvth = (motor_dis[2]+motor_dis[1]+motor_dis[3]+motor_dis[0])/ (4 * body_radius);
			
			odom->vel_x = 1000*dvx / interval; 
			odom->vel_y = 1000*dvy / interval; 
			odom->vel_z = 1000*dvth / interval; 

			float th = odom->z;

			//odometry
			float dx = dvx * cos(th) - dvy * sin(th);
			float dy = dvx * sin(th) + dvy * cos(th);
			float dth = dvth;
			
			odom->x += dx;
			odom->y += dy;
			odom->z += dth;
		}
	}
};

#endif
