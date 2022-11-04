#include <avr/interrupt.h>

#include "Init/Init.h"

int main(void)
{
	init();
    while (1) 
    {
		Mode_Select();
	}
}		



















ISR(INT0_vect)
{
	uint8_t A= 'A';
	UART_SendPayload(&A,1);
}
ISR(INT1_vect)
{
	uint8_t Z= 'Z';
	UART_SendPayload(&Z,1);
}


