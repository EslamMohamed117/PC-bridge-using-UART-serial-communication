#include "LED.h"

void LED_On()
{
	DIO_SET_PIN_VAL('A',0,1);
}
void LED_Off()
{
	DIO_SET_PIN_VAL('A',0,0);
}
void LED_Blink()
{
	DDRA |=(1<<PINA0);
	PORTA |=(1<<PINA0);
	_delay_ms(500);
	PORTA &=~(1<<PINA0);
	_delay_ms(500);
	PORTA |=(1<<PINA0);
	_delay_ms(500);
	PORTA &=~(1<<PINA0);
	_delay_ms(500);
	PORTA &=~(1<<PINA0);
	_delay_ms(500);
	PORTA &=~(1<<PINA0);
	_delay_ms(500);
}

void LED_MainFunction(uint8_t *buffer)
{
	mode_t mode;
	uint8_t ModeV = buffer[ONOFF_OFFSET]-0x30; 
	mode = ((ModeV == 0) ? OFF : ON);
	switch(mode)
	{
		case OFF:
			LED_Off();
			break;
		case ON:
			LED_On();
			break;
		default: {}
	}
}
void LED_Init()
{
	DIO_SET_PIN_DIR('A',0,1);
	LED_Off();
}