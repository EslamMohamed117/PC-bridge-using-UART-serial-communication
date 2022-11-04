#include "Mode.h"

void Mode_Select()
{		
	uint8_t ack = '>';
	UART_ReceivePayload(cmd_buffer, FULL_CMD_CNT);
	
	/* Pull unitl reception is complete. */
	while(0 == UART_IsRxComplete());
	if((cmd_buffer[MARKER_START] == '@') && (cmd_buffer[MARKER_END] == ';'))
	{
		uint8_t mode = cmd_buffer[MODE_OFFSET]-0x30;
		
		switch (mode)
		{
			case 0:
				LED_MainFunction(cmd_buffer);
				break;
			case 1:
				Motor_MainFunction(cmd_buffer);
				break;
			case 2:
				Memory_MainFunction(cmd_buffer);
				break;
			case 3:
				WAVE_MainFunction(cmd_buffer);
				break;
		}
	}
	for(uint8_t i = 0; i<FULL_CMD_CNT ; i++) {cmd_buffer[i] = 0;}
		
}
