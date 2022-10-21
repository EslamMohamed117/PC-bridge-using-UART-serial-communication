#include <avr/io.h>
#include <avr/interrupt.h>
#include "USART.h"
#define F_CPU 8000000UL

volatile float duty_cycle =0;

volatile char x;
volatile char mode;
int main()
{
	PORTD=0b00001100;	// Enable Pull-up for INT 0 and INT 1
	MCUCR = 0b00001010; // INT 0 and INT 1 will be triggered when falling edge signal arrives
	GICR = 0b11000000;  // Enable INT 0 and INT 1
		
	DDRA = 1;
	PORTA=0b00000010; // Setting direction of motor
	
	DDRA= 0x07;	// Setting first two pins of port A as output for controlling the driver
	DDRB |= (1<<PINB3);	// Setting pin 3 of port b as output for PWM signal generated at OC0

	// TIMER0
	TCCR0=0b01110101;	//Configure Timer0 to run at Phase correct PWM
	
	USART_INIT(9600);
	sei();
	while(1)
	{
		x = USART_Receive();
		if(x == 'l')
			mode = x;
		else if(x=='f')
			mode = x;
		if(mode == 'l')
		{
			if(x == '0')
			{
				// turn off LED.
				PORTA &= ~(1<<PINA0);
			}
			else if(x =='1')
			{
				// turn on LED.
				PORTA |= 1<<PINA0;
			}
		}
		else
		{
			if(x=='+') // Maximum speed
				duty_cycle = 1;
			else
				duty_cycle = ((unsigned int)x-0x30)/10.0;
		}
		OCR0 = (uint8_t)(255* (1-duty_cycle));
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
