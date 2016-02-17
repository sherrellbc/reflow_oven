#include "spi.h"
#include <util/delay.h>
#include "../usart/usart.h"
#include <avr/interrupt.h>

/* ------------------------------------------------------------------------------------------------------------------ */
void Spi_Init(){
    /* Set SCK as output */
	/* Note: MOSI Left Uninitialized here due to not needing it yet */
    SPI_DDR |= (1<<SPI_SCK) | (1<<SPI_SS);

    /* Spi Master, Enable Spi, */
    SPCR = 0;
    SPCR |= (1<<MSTR) | (1<<SPE) | (1<<SPR0) | (1<<SPR1) | (1<<CPOL);
    cli();
}



uint8_t Spi_Transfer(uint8_t sendData){
	SPDR = sendData; 
	while( !(SPSR & (1<<SPIF)) )
		;
	return SPDR; 
}