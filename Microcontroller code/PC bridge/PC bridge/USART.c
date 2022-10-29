#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "STD_MACROS.h"
#include "DIO.h"

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

void USART_SEND_DATA(char data)
{
	while(READ_BIT(UCSRA , UDRE) == 0);
	UDR = data;
}

char USART_RECIEVE_DATA(void)
{
	while(READ_BIT(UCSRA , RXC) == 0);
	return UDR;
}

void USART_SEND_STRING(char * p)
{
	while(*p != '\0')
	{
		USART_SEND_DATA(*p);
		p++;
		_delay_ms(100);
	}
}