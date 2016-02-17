/*****
 * Branden Sherrell, July 9, 2015
 * pid.h 
 *
 * Function prototypes and associated definitions required by the implementation 
 *  that provides PID control
 */

#ifndef _PID_H
#define _PID_H

#include "feedback.h" /* Get_Temperature() */



/*   Takes the current process variable and the desired setpoint and outputs a drive value between 0 and 100
 *   
 * 	 @param pVar	 : The process variable of the current process
 *   @param setpoint : The desired set-point for the current process
 *   @return		 : Floating point value representing the drive value of the controller
 */
float PID_Control(float pVar, float setPoint);


#endif