#include <avr/io.h>
#include <util/delay.h>
#include "usart/usart.h"
#include "spi/spi.h"
#include "control/feedback.h"
#include "control/pid.h"
#include "pwm/pwm.h"
#include <util/atomic.h>

#define DBG	0

/* TODO: Structure the code such that it is possible to switch to cooling mode
	Also, it is necessary to have a mechanism through which we can update the interface so the device does not have to be reset for another reflow */
void Functional_Loop(){
	static float controlVal;
	static int loopCount = 0;

	/* Wait for the appropriate amount of time to pass before running through another interation */
	loopCount++;
	while(!Time_Has_Expired())
		;

	if(loopCount < g_beginPrepForRampTime)		
		controlVal = PID_Control(Get_Temperature(), 150);	/* Time < 2.5min => Set to 150 */
	else if(loopCount < g_beginPeakRampTime)
		controlVal = 100;									//PID_Control(Get_Temperature(), 210);	/* 2.5min < Time > 3.0min => Set to 190 (maintain temp & prepare for reflow setpoint) */
	else if(loopCount < g_peakStoppingTime)
		controlVal = PID_Control(Get_Temperature(), 225);	/* At this point we give all this oven has to offer to get the greatest temperature slope */
	else{
		//Open oven and control cooldown					
		controlVal = 0;										/* After the temperature has peaked, turn the heat control off and begin controlling cooling */
	}

	PWM_setDutyCycle(controlVal);
	printf("%0.2f\n", (double)Get_Temperature());		/* This is not the exact same temperature as what was send to PID_Control, but it'll be close enough */
}



void System_Init(){
	usart_init(USART_TX, 0, USART_ASYNC, (uint32_t) 57600, CHARSIZE_8, PARITY_EVEN, STOPBIT_1, CLK_POL_TXRISE_RXFALL);
	if(DBG) printf("[STATUS] USART Initialized\n");

	MAX31855_Init();
	if(DBG) printf("[STATUS] MAX31855 Initialized\n");

	Spi_Init();
	if(DBG) printf("[STATUS] SPI Initialized\n");

	PWM_Init();
	if(DBG) printf("[STATUS] PWM Initialized\n");

	Timer_Init();
	if(DBG) printf("[STATUS] Loop Timer Initialized\n");

	DDRC |= 1<<PC0; 

	//_delay_ms(1000);
}



int main(){
	/* Usart, PWM, SPI, ADC Initialization */
	System_Init();

	/* Start the loop timer */
	Timer_Start();


	/* Warm the system/elements so we have a bit of thermal momentum */
	System_Prime();

	/* Start the soldering sequence */
	while(1)
		Functional_Loop();
}
