/*****
 * Branden Sherrell, June 22, 2015
 * feedback.h 
 *
 * Function prototypes and associated definitions required by the implementation 
 *  that reads data from the MAX31855 device
 */

#ifndef _FEEDBACK_H
#define _FEEDBACK_H



/* Device Definitions */
#define MAX31855_PORT		PORTB
#define MAX31855_DDR		DDRB
#define MAX31855_PIN		PB0		



#include <avr/io.h> 
#include "../pwm/pwm.h"
#include "../spi/spi.h"

 

/* Timing for our temperature transition points */
extern int g_beginPrepForRampTime;
extern int g_beginPeakRampTime;
extern int g_peakStoppingTime;



/*   
 *    Initialize the MAX31855 for use (PORTS, DDR, etc)
 */ 
void MAX31855_Init();



/*   Get the current temperature by reading the MAX31855 (B31:B18)
 *   
 *   @return	: Floating point result of the current thermocouple temperature in C
 */
float Get_Temperature();



/*   Get the current cold-junction temperature by reading the MAX31855 (B15:B4)
 *   
 *   @return	: Floating point result of the current junction temperature in C
 */
float Get_Cold_Junction_Temperature();



/*   Get the Fault-bit by reading the MAX31855
 *   
 *   @return	: Value of the bit indicating a thermocouple fault as seen by the MAX31855
 */
int Read_Fault_Bit();



/*   Convert the temperature in C to F. This function is needed if the result is desired in F
 *   since the output of the MAX31855 gives temperature in C directly
 *   
 *   @param		: Temperature in F
 *   @return	: Corresponding temperature in F
 */
float C_To_F(float temp);



/*
*	Prime the system by header it to 30 C before starting the control loop
*/
void System_Prime();




#endif
