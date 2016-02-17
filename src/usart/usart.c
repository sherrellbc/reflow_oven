#include "usart.h"

void usart_init(int rx_tx, int interrupt_sel, int sync_async, uint32_t  baudRate, int charSize, int parity, int stopBits, int clkPolarity){
    stdout = &stdoutRedirect;

    /* UCSROB contains RX/TX enable, interrupt enable, and 1 bit of charSize */
    UCSR0B |= (rx_tx | interrupt_sel);

    /* UCSR0C contains SYNC,ASYNC, parity, num stopBits, charSize(2 bits), clock polarity */
    UCSR0C |= (sync_async | parity | stopBits | charSize | clkPolarity);

    /* Set the baud rate */
    UBRR0H = (uint8_t) UBRRVAL(baudRate)>>8;
    UBRR0L = (uint8_t) UBRRVAL(baudRate);
}

void usart_set_doubleSpeed(uint8_t state){
    if(state)
        UCSR0A |= 1<<USART_DBL_SPEED_BIT;
    else
        UCSR0A &= ~(1<<USART_DBL_SPEED_BIT);
}

uint8_t usart_readErrors(){
    uint8_t errorBitField = 0;

    if(UCSR0A & 1<<FE0)
        errorBitField |= 1<<FE0;
    if(UCSR0A & 1<<DOR0)
        errorBitField |= 1<<DOR0;
    if(UCSR0A & 1<<UPE0)
        errorBitField |= 1<<UPE0;

    return errorBitField;
}

uint8_t usart_getc() {
    while (usart_isBusy());
    return UDR0;
}

void usart_puts(char* string){
    while(*string != '\0'){
        while(usart_isBusy());
        usart_putc(*string++);
    }
}

int usart_putc_printf(char data, FILE* stream){
    while(usart_isBusy());
    UDR0 = data;
    return 0;
}
