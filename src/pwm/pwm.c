/*****
 * Branden Sherrell, July 9, 2015
 * pwm.c
 *
 * Function prototypes and associated definitions required by the PWM implementation 
 */



#include "pwm.h"
#include "../usart/usart.h"



volatile int g_timeElapsed = 0;		/* Indication for all who are concerned regarding whether or not we can proceed with the next loop interation - set asynchronously */
static int g_countBeforeSettingCTC; 	/* The number of times the OVF interrupt will run before enabling 'Compare Match'ing */



int Time_Has_Expired(){
	int local_timeElapsed = 0; 
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		if(1 == g_timeElapsed){
			local_timeElapsed = 1;
			g_timeElapsed = 0;
		}
	}

	return local_timeElapsed;
}



void Timer_Init(){
	TCCR1A = 0;
	TCCR1B = 0;
	g_timeElapsed = 0;
	TIMSK1 |= (1<<TOIE1);
	g_countBeforeSettingCTC = (int) (((float)g_timeBetweenLoops/(65535.0/16000000.0))/1000.0);				/* Whole number portion */
	OCR1A = (((float)g_timeBetweenLoops/1000.0/(65535.0/16000000.0)) - g_countBeforeSettingCTC)*65535;		/* The fractional portion scaled for the size of the timer (16 bits) */
	sei();
}



inline void Timer_Start(){
	TCCR1B = (1<<CS10);
}



inline void Timer_Stop(){
	TCCR1A &= ~(1<<CS10);
}



void PWM_Init(){
 	DDRD |= 1<<PD6; 	/* Set the PWM pin as output so the waveform is visible */
 	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00); 
 	TCCR0B |= (1<<CS02) | (1<<CS00);
 	OCR0A = 184;
}



void PWM_setDutyCycle(float setPoint){
 	if(100 < setPoint)
 		OCR0A = 0xFF; 
	else if(0 > setPoint)
 		OCR0A = 1;
 	else
 		OCR0A = (uint8_t) (0xFF*(setPoint/100.0));
 
 	return; 
}



/* This subroutine is run each time the timer overflows */
ISR(TIMER1_OVF_vect){
	static int count = 0;
	if(g_countBeforeSettingCTC == ++count){
		TCCR1B |= (1<<WGM12);	/* Enables 'Compare Match'ing */
		TIMSK1 |= (1<<OCIE1A);
		count = 0;
	}
}



/* When appropriate, this subroutine will run when the timer matches against OCR1A */
ISR(TIMER1_COMPA_vect){
	TCNT1 = 0;					/* Reset the timer */
	TCCR1B &= ~(1<<WGM12);		/* Disable 'Compare Match'ing */
	TIMSK1 &= ~(1<<OCIE1A);
	g_timeElapsed = 1;
}