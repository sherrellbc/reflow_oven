#ifndef	USART_H
#define USART_H

#ifndef IO_H
#include <avr/io.h>
#define IO_H
#endif

#ifndef	MATH_H
#include <math.h>
#define MATH_H
#endif

#ifndef F_CPU //does not work? 
#warning "F_CPU is not defined for uart.h"
#endif

#include <stdio.h>
#include <stdint.h>

#define USART_BUSY			0xFF
#define USART_WRITE_SUCCESS 0x00
#define USART_DBL_SPEED_BIT 0x01
#define USART_DBL_SPEED_ON	0x01
#define USART_DBL_SPEED_OFF	0x00

/* Baud rate */
#define UBRRVAL(baudRate)	(F_CPU/16/baudRate-1) //((uint16_t) round((F_CPU/16/(baudRate))-1))

/* Type of device (RX/TX) */
#define USART_RX		0x10
#define USART_TX		0x08

/* Which type? */
#define USART_ASYNC		0x00
#define USART_SYNC		0x40
//#define MASTER_SPI	0xC0
//#define SYNC_CLK_PIN	PBx

/* Interrupt enables */
#define USART_INT_UDRE	0x10 		//Data register empty (UDRn)
#define USART_INT_TXC	0x40 		//TX complete
#define USART_INT_RXC	0x80 		//RX complete


/* Character size */
#define CHARSIZE_5		0x00
#define CHARSIZE_6		0x02
#define CHARSIZE_7		0x04
#define CHARSIZE_8		0x06
//#define CHARSIZE_9		//not supported 

/* Parity */
#define PARITY_NONE		0x00
#define PARITY_EVEN		0x20
#define PARITY_ODD		0x30

/* Stop bits */
#define STOPBIT_1		0x00
#define STOPBIT_2		0x08

/* Clock Polarity - SYNC only */
#define CLK_POL_TXRISE_RXFALL	0x00
#define CLK_POL_TXFALL_RXRISE	0x01

/* Error flags */
#define USART_FRAME_ERROR			0x10
#define USART_DATA_OVERRUN_ERROR	0x08
#define USART_PARITY_ERROR			0x04


/** FUNCTION: usart_readErrors
*
*   Initialize usart hardware given parameters indicating the transmission/reception framing structure
*   @param  rx_tx           Indicates whether this entity will receive, transmit, or both
*   @param  interrupt_sel   Selects which interrupts will be used, if any
*   @param  sync_async      Synchronous uses a shared clock, asynchronous does not
*   @param  baudRate        Transfer rate
*   @param  charSize        Transfer size (how many bits)
*   @param  parity          Error checking mechanism, even or add
*   @param  stopBits        Number of stops bits to use
*   @param  clkPolarity     On which edge shall the information be received or transmitted
*/
void usart_init(int rx_tx, int interrupt_sel, int sync_async, uint32_t baudRate, int charSize, int parity, int stopBits, int clkPolarity);

/** FUNCTION: usart_readErrors
*
*   Sets state (on/off) of usart double speed transmission
*/
void usart_set_doubleSpeed(uint8_t state);

/** FUNCTION: usart_readErrors
*
*   Reads the necessary error registers and reports back results
*   @return     A single bitfield with information regarding any errors read (0 indicates no error)
*/
uint8_t usart_readErrors();

/** FUNCTION: usart_getc
*
*   Reads a character from the I/O stream
*   @return     Returns the character read from the stream
*/
uint8_t usart_getc();

/** FUNCTION: usart_puts
*
*   Writes a character string using usart
*   @param  char* string    Pointer to the first character of a NULL-terminated string
*/
void usart_puts(char* string);

/** FUNCTION: usart_isBusy
*
*   Checks for the status of the usart register - used to determine is we can read data or write data
*   @return     1 if UDRE is cleared (usart no busy), 0 otherwise
*/
inline uint8_t usart_isBusy(){
    return !(UCSR0A & 1<<UDRE0);
}

/** FUNCTION: usart_putc
*
*   Checks for the status of the usart register - used to determine is we can read data or write data
*   @params     char data   Data to be written over usart
*/
inline void usart_putc(char data){
    while(usart_isBusy());
    UDR0 = data;
}

/** FUNCTION: usart_putc_printf
*
*   write a byte to UDR0 for transmission - same UDRE handling assumptions as above
*   Writes a character to the I/O stream. For use with printf()
*   @param  char data       Character to be written to the I/O stream
*   @param  FILE* stream    The I/O stream to write to (likely stdout)
*/
int usart_putc_printf(char data, FILE* stream);

/* Setup an I/O stream */
static FILE stdoutRedirect __attribute((unused)) = FDEV_SETUP_STREAM(usart_putc_printf, NULL, _FDEV_SETUP_WRITE);

#endif
