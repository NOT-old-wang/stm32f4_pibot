#include "pid.h"

#include "board.h"
#include <stdio.h>

PID::PID(float* _input, float* _feedback, float _kp, float _ki, float _kd, unsigned short _max_output)
    :input(_input), feedback(_feedback), kp(_kp), ki(_ki), kd(_kd), max_output(_max_output*1.0){

    clear();

	printf("pid=%0.3f %0.3f %0.3f\r\n", kp, ki, kd);
}

void PID::clear(){
    error = integra = derivative = previous_error =0;
}

void PID::update(float _kp, float _ki, float _kd, unsigned short _max_output){
	kp = _kp;
	ki = _ki;
	kd = _kd;
	max_output = _max_output;
}

short PID::compute(float interval){
    error = *input - *feedback;

	integra = integra + error*interval;
	derivative = (error - previous_error) / interval;

	previous_error = error;
	

	if (ki != 0)
	#if PID_DEBUG_OUTPUT
		printf("integra=%0.3f max_output=%0.3f %0.3f ", integra, -max_output/ki, max_output/ki);
	#endif
		if (integra < -(max_output/ki))
		{
			//printf("integra clear-\r\n");
			integra = -(max_output/ki);
		}
		if (integra > max_output/ki)
		{
			//printf("integra clear+\r\n");
			integra = max_output/ki;
		}

	float val = error*kp + integra*ki + derivative*kd;

	if (val < -max_output)
		val = -max_output+1;
	else if (val > max_output)
		val = max_output-1;

#if PID_DEBUG_OUTPUT
	printf("error=%0.3f integra=%0.3f derivative=%0.3f val=%d\r\n", error, integra, derivative, val);
#endif

	return val;
}