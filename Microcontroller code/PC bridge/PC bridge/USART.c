#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "STD_MACROS.h"


void USART_INIT(unsigned long baud)
{
	//Set Baud Rate
	//URSEL = 0 (Default)
	unsigned short UBRR;
	UBRR = (F_CPU / (16*baud)) - 1;
	UBRRH = (unsigned char) (UBRR >> 8);
	UBRRL=(unsigned char) UBRR;
	
	
	//Enable Sender and Receiver
	SET_BIT(UCSRB,TXEN);
	SET_BIT(UCSRB,RXEN);
	
	//8-bit Data, No Parity Mode, Asynchronous Mode (Default), 1 stop bit (Default), Clock Polarity (default) (TX rising, RX falling)
	//URSEL = 1 (Select UCSRC)
	UCSRC=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}
unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
	;
	/* Get and return received data from buffer */
	return UDR;
}
