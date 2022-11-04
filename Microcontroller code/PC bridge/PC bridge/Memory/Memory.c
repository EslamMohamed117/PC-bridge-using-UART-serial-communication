//RAM & ROM
// @ mode read/write[1 byte] RAM/ROM[1 byte] address[4 byte] value[1 byte] ;
#include "../Lib/USART/uart.h"
#include "../Lib/EEPROM/EEPROM.h"
#include <avr/io.h>
#include "Memory.h"


void Memory_MainFunction(uint8_t *buffer)
{

	memory_t memory;
	operations_t operation;
	uint8_t memorySelection = buffer[RAMROM_OFFSET]-0x30;
	memory = ((memorySelection == 0) ? RAM : ROM);
	operation = buffer[READWRITE_OFFSET] == 'r' ? READ_M : WRTIE_M;
	unsigned short domin = 0x1000;
	unsigned short Address = 0;
	unsigned char addressDigit;
	uint8_t Rvalue='0';
	uint8_t Wvalue='0';
	
	for(int i=0;i<4;i++)
	{
		addressDigit = buffer[ADDRESS_OFFSET+i];
		if (addressDigit>64)
		Address += (addressDigit-55)*domin;
		else
		Address += (addressDigit-48)*domin;
		domin/=0x10;
	}
	
	//led_blink();
	switch(memory)
	{
		case ROM:
		{
			if (operation == READ_M)
			{
				Rvalue = EEPROM_u8READ(Address);
				_delay_ms(100);
				UART_SendPayload(&Rvalue,1);
				
			}
			else if (operation == WRTIE_M)
			{
				Wvalue = buffer[VALUE_OFFSET];
				EEPROM_WRITE(Address, Wvalue);
			}
			break;
		}
		case RAM:
		{
			if (operation == READ_M)
			{
				Rvalue = READ_RAM(Address);
				_delay_ms(100);
				UART_SendPayload(&Rvalue,1);
			}
			else if (operation == WRTIE_M)
			{
				Wvalue = buffer[VALUE_OFFSET];
				WRITE_RAM(Address, Wvalue);		
			}			
			break;
		}
		default: {/* Do nothing.*/}
	}
	LED_Blink();	
}