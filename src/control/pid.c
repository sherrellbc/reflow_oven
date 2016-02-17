/*****
 * Branden Sherrell, July 9, 2015
 * pid.c
 *
 * Implementation of the PID control
 */

 #include "pid.h"



static const float epsilon = 	1;		/* The point at which difference is negligible */
static const float Kd = 		0.1;	/* Derivative gain */
static const float Ki = 		0;		/* Integral gain */
static const float Kp = 		5.5; 	/* Proportional gain */



float PID_Control(float pVal, float setPoint){
	static float integral = 0, previous_error = 0; 
	float output, error, derivative; 

	error = setPoint - pVal; 
	derivative = (error - previous_error)/(g_timeBetweenLoops/1000.0);	/* static int, divisor is computed at compile-time */
	integral += error*g_timeBetweenLoops; 	
	output = Kp*error + Ki*integral + Kd*derivative; 

	/* Saturation protection */
	if(output > 100)
		output = 100;
	else if(output < 0)
		output = 0; 

	previous_error = error;
	return output;
}