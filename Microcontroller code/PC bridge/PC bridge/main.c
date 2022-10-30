#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include "DIO.h"
#include "USART.h"
#include "EEPROM.h"
#define READ_RAM(address) *((char *) (address))
#define WRITE_RAM(address,value) *((char *) (address))=value

void led_blink()
{
	PORTA |=(1<<PINA0);
	_delay_ms(100);
	PORTA &=~(1<<PINA0);
	_delay_ms(100);
	PORTA |=(1<<PINA0);
	_delay_ms(100);
	PORTA &=~(1<<PINA0);
	_delay_ms(100);
}
void init()
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
	sei();
	
}

int main(void)
{
	USART_INIT(9600);
	DIO_SET_PIN_DIR('d', 1, 1);
	DIO_SET_PIN_DIR('d', 2, 1);
	DIO_SET_PIN_DIR('a', 0, 1);
	char instruction = 0;
	unsigned short Address = 0;
	char Rvalue = 0;
	char Wvalue = 0;
	init();
	volatile float duty_cycle =0;

	volatile char x;
	volatile char mode;
    while (1) 
    {
		mode =USART_RECIEVE_DATA();
		if(mode == '@')
		{
			unsigned char addressDigit;
			instruction = USART_RECIEVE_DATA();
			_delay_ms(100);
			unsigned short domin = 0x1000;
			for(int i=0;i<4;i++)
			{
				addressDigit = USART_RECIEVE_DATA();
				if (addressDigit>64)
					Address += (addressDigit-55)*domin;
				else
					Address += (addressDigit-48)*domin;
				domin/=0x10;
			}
			
			if(instruction == 'r')
			{
				Rvalue = EEPROM_u8READ(Address);
				_delay_ms(100);
				if(USART_RECIEVE_DATA()==';')
				{
					USART_SEND_DATA(Rvalue);
					led_blink();
				}
				//DIO_SET_PIN_VAL('d', 2, 1);
				
			}
			else if (instruction == 'w')
			{
				Wvalue = USART_RECIEVE_DATA();
				if(USART_RECIEVE_DATA()==';')
				{	
					EEPROM_WRITE(Address, Wvalue);
					led_blink();
				}
			}
			Address=0;
			mode =' ';
		}
		if(mode == '#')
		{
			unsigned char addressDigit;
			instruction = USART_RECIEVE_DATA();
			_delay_ms(100);
			unsigned short domin = 0x1000;
			for(int i=0;i<4;i++)
			{
				addressDigit = USART_RECIEVE_DATA();
				if (addressDigit>64)
				Address += (addressDigit-55)*domin;
				else
				Address += (addressDigit-48)*domin;
				domin/=0x10;
			}
			
			if(instruction == 'r')
			{
				Rvalue = READ_RAM(Address);
				_delay_ms(100);
				if(USART_RECIEVE_DATA()==';')
				{
					USART_SEND_DATA(Rvalue);
					led_blink();
				}
				//DIO_SET_PIN_VAL('d', 2, 1);
				
			}
			else if (instruction == 'w')
			{
				Wvalue = USART_RECIEVE_DATA();
				if(USART_RECIEVE_DATA()==';')
				{
					WRITE_RAM(Address, Wvalue);
					led_blink();
				}
			}
			Address=0;
			mode =' ';
		}
		else if(mode == 'l')
		{
			x = USART_RECIEVE_DATA();
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
		else if(mode =='f')
		{
			x = USART_RECIEVE_DATA();
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
	USART_SEND_DATA('A');
}
ISR(INT1_vect)
{
	USART_SEND_DATA('Z');
}



