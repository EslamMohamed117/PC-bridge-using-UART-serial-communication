#include "Init.h"

void init()
{
	// Enable int 0 , 1
	DDRD|=(1<<PIND2)|(1<<PIND3);
	PORTD |=(1<<PIND2)|(1<<PIND3);	// Enable Pull-up for INT 0 and INT 1
	MCUCR = 0b00001010; // INT 0 and INT 1 will be triggered when falling edge signal arrives
	GICR = 0b11000000;  // Enable INT 0 and INT 1
	//------------------
	
	UART_Config();
	
	LED_Init();
	
	Motor_init();
	for(uint8_t i = 0; i<FULL_CMD_CNT ; i++) {cmd_buffer[i] = 0;}
	WAVE_Init();
	sei();
}