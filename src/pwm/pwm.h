/*****
 * Branden Sherrell, July 9, 2015
 * pwm.h 
 *
 * Function prototypes and associated definitions required by the implementation 
 *  that controls the PWM hardware of the controller
 */

#ifndef _PWM_H
#define _PWM_H



 #include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>


extern const int g_timeBetweenLoops;	/* The amount of time between loop iterations in mS */
extern volatile int g_timeElapsed;		/* Indication for all who are concerned regarding whether or not we can proceed with the next loop interation - set asynchronously */



/* 
*	Check whether or not we can continue with the next loop interation
*/
int Time_Has_Expired();



/*
*	Initializes the timer and associated registers to precisely set g_timeElapsed every g_timeBetweenLoops mS
*/
void Timer_Init();



/*
*	Start the 16-bit time for use as a precision non-blocking loop timer
*/
void Timer_Start();



/*
*	Stop the 16-bit timer
*/
void Timer_Start();



/*   Initializes the PWM hardware (using the 8-bit timer) for a few used pins
 *   
 */
 void PWM_Init();



/*   Sets the appropriate register to control the PWM duty cycle
 *   
 * 	 @param setPoint	: The duty cycle to set the PWM output to. This parameter will be boxed to [0, 100]
 */
 void PWM_setDutyCycle(float setPoint);



 #endif