/*****
 * Branden Sherrell, June 22, 2015
 * feedback.c
 *
 * Implementation specifics
 */



#define correctedTemp(temp)	(2.118*temp-1.364)


#include "feedback.h"
#include "../usart/usart.h"
#include <util/delay.h>



/* Related to the temperature solution of the K-type Thermocouple/MAX31855 */
const float g_tResolution = 	0.2500; 	/* Resolution of the thermocople temp */
const float g_cjtResolution = 	0.0625; 	/* Resolution of the cold junction contact */
static const float g_primeTemp = 30;	/* The temperature to warm the system to before starting the PID control loop */ 



/* Transition points for our temperature profile */
const int g_timeBetweenLoops = 		50;											/* The amount of time between loop iterations in mS */
//int g_beginPrepForRampTime = 	(int) ((1000.0/g_timeBetweenLoops)*150);	/* The time at which we begin warming the elements for a steep slope increase to the peak temp */
//int g_beginPeakRampTime = 		(int) ((1000.0/g_timeBetweenLoops)*180);	/* The time at which we begin driving the temperature towards the peak temp */
//int g_peakStoppingTime = 		(int) ((1000.0/g_timeBetweenLoops)*210);	/* At this point we should have reached the peak, stop actively driving the elements */
int g_beginPrepForRampTime = 	(int) ((1000.0/50)*140);	/* The time at which we begin warming the elements for a steep slope increase to the peak temp */
int g_beginPeakRampTime = 		(int) ((1000.0/50)*180);	/* The time at which we begin driving the temperature towards the peak temp */
int g_peakStoppingTime = 		(int) ((1000.0/50)*210);	/* At this point we should have reached the peak, stop actively driving the elements */


void MAX31855_Init(){
	MAX31855_DDR |= 1<<MAX31855_PIN;
	MAX31855_PORT |= 1<<MAX31855_PIN;  /* Device is active-low */
}



float Get_Temperature(){
	uint16_t temperature = 0;
	float computedTemp; 

	MAX31855_PORT &= ~(1<<MAX31855_PIN);
	temperature = Spi_Transfer(0)<<8;
	temperature |= Spi_Transfer(0);
	MAX31855_PORT |= 1<<MAX31855_PIN;

	computedTemp = (temperature>>4) + (((temperature>>2)&0x03)*g_tResolution); 	/* Temperature reading has only 14 bits of resolution */
	return correctedTemp(computedTemp);
}



float Get_Cold_Junction_Temperature(){
	uint16_t temperature = 0;
	float computedTemp;

	MAX31855_PORT ^= 1<<MAX31855_PIN;
	Spi_Transfer(0); /* Shift out temperature data until we get relevant CJT data */
	Spi_Transfer(0);
	temperature = Spi_Transfer(0)<<8;
	temperature |= Spi_Transfer(0);
	MAX31855_PORT ^= 1<<MAX31855_PIN;

	computedTemp = (temperature>>8) + (((temperature>>4)&0x0F)*g_cjtResolution);
	return computedTemp;	/* Cold Junction Temperature has only 12 bits of resolution */
}



int Read_Fault_Bit(){
	uint16_t buf = 0;

	MAX31855_PORT ^= 1<<MAX31855_PIN;
	buf = Spi_Transfer(0)<<8;
	buf |= Spi_Transfer(0);
	MAX31855_PORT ^= 1<<MAX31855_PIN;

	return ( buf & (1<<0) );	/* Bit 16 is the singular fault bit */
}



inline float C_To_F(float temp){
	return ((9.0/5.0)*temp + 32);
}



void System_Prime(){
	float currentTemp; 

	PWM_setDutyCycle(100);
	do{
		/* Wait for the appropriate amount of time to pass before running through another interation */
		while(!Time_Has_Expired())
			;

		currentTemp = Get_Temperature();
		printf("%0.2f\n", (double)Get_Temperature());
	}while(currentTemp < g_primeTemp);
}

