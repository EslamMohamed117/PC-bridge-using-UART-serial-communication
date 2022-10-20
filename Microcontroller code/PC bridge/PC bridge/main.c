#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#define F_CPU 8000000UL

volatile char x= 'x';
int main()
{
	PORTD=0b00001100;	// Enable Pull-up for INT 0 and INT 1
	MCUCR = 0b00001010; // INT 0 and INT 1 will be triggered when falling edge signal arrives
	GICR = 0b11000000;  // Enable INT 0 and INT 1
		
	DDRA = 1;
	PORTA=0;
	
	USART_INIT(9600);
	sei();
	
	while(1)
	{
		x = USART_Receive();
		if(x == '0')
		{
			// turn off LED.
			DDRA = 255;
			PORTA = 0;
		}
		else if(x =='1')
		{
			// turn on LED.
			DDRA = 255;
			PORTA = 1;
		}
	}
}
ISR(USART_RXC_vect)
{
}
ISR(INT0_vect)
{
	USART_Transmit('A');
}
ISR(INT1_vect)
{
	USART_Transmit('Z');
}
