#ifndef _SPI_H
#define _SPI_H

#include <avr/io.h>

#define SPI_SCK		PB5
#define SPI_MISO	PB4
#define SPI_MOSI	PB3
#define SPI_SS 		PB2
#define SPI_DDR 	DDRB

#define SPI_WAIT_TRANS_COMPLETE() 		while( !(SPSR & (1<<SPIF)) )
#define SPI_SELECT_SLAVE(port, dev) 	(port &= ~(1<<dev))
#define SPI_DESELECT_SLAVE(port, dev)	(port |= (1<<dev))


/*
 * Initializes the SPI hardware
 */
void Spi_Init();

/*
* Gets updated information from SPI slave specified by device 
*
* @param sendData	: Data to send to the currently selected SPI device
* @return			: Shifted data returned to use during the transfer of MSTR -> Slave
*/
uint8_t Spi_Transfer(uint8_t sendData);


#endif // _SPI_H
