//RAM & ROM
// @ mode read/write[1 byte] RAM/ROM[1 byte] address[4 byte] value[1 byte] ;

#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1
#define _CMD_READWRITE_CNT  1
#define _CMD_RAMROM_CNT   1
#define _CMD_ADDRESS_CNT   4
#define _CMD_VALUE_CNT   1
#define _CMD_END_CNT   1

#define FULL_CMD_CNT	(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_READWRITE_CNT + _CMD_RAMROM_CNT+ _CMD_ADDRESS_CNT+ _CMD_VALUE_CNT + _CMD_END_CNT)
#define MODE_OFFSET		(_CMD_START_CNT)
#define READWRITE_OFFSET(_CMD_START_CNT +  _CMD_MODE_CNT)
#define RAMROM_OFFSET	(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_READWRITE_CNT)
#define ADDRESS_OFFSET  (_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_READWRITE_CNT + _CMD_RAMROM_CNT)
#define VALUE_OFFSET	(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_READWRITE_CNT + _CMD_RAMROM_CNT + _CMD_ADDRESS_CNT)
#define MARKER_END		(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_READWRITE_CNT + _CMD_RAMROM_CNT + _CMD_ADDRESS_CNT + _CMD_VALUE_CNT)
#define MARKER_START (0)

#define WAVEFORM_NUM 4


void Memory_MainFunction(void)
{

	// Main function must have two states,
	// First state is command parsing and waveform selection.
	// second state is waveform executing.
	switch(currentState)
	{
		case UPDATE_WAVE:
		{
			UART_SendPayload((uint8_t *)">", 1);
			while (0 == UART_IsTxComplete());

			/* Receive the full buffer command. */
			UART_ReceivePayload(cmd_buffer, FULL_CMD_CNT);
			
			/* Pull unitl reception is complete. */
			while(0 == UART_IsRxComplete());

			/* Check if the cmd is valid. */
			if((cmd_buffer[MARKER_START] == '@') && (cmd_buffer[MARKER_END] == ';'))
			{
				// Extract amplitude and freq values before sending them to the waveform generator.
				/* Compute amplitude. */
				{
					char _buffer[_CMD_AMP_CNT];
					for(uint8_t i = 0; i < _CMD_AMP_CNT; ++i) { _buffer[i] = cmd_buffer[AMP_OFFSET+i]; }
					amp_value = atoi(_buffer);
				}

				/* Compute frequency. */
				{
					char _buffer[_CMD_FRQ_CNT];
					for(uint8_t i = 0; i < _CMD_FRQ_CNT; ++i) { _buffer[i] = cmd_buffer[FREQ_OFFSET+i]; }
					freq_value = atoi(_buffer);
				}

				/* Compute waveform. */
				{
					waveform_index = cmd_buffer[WAVE_OFFSET] - '0';
				}
			}
			else
			{
				/* Clear cmd_buffer. */
				for(uint8_t i = 0; i < FULL_CMD_CNT; i += 1)
				{
					cmd_buffer[i] = 0;
				}
			}

			// Trigger a new reception.
			UART_ReceivePayload(cmd_buffer, FULL_CMD_CNT);

			UART_SendPayload((uint8_t *)"\r>", 2);
			while (0 == UART_IsTxComplete());
		}
		case GENERATE_WAVE:
		{
			// Execute waveform..
			if(waveform_index < WAVEFORM_NUM)
			{
				waveform[waveform_index](amp_value, freq_value);
			}
			// Keep in generate wave if no command it received.
			currentState = (1 == UART_IsRxComplete()) ? UPDATE_WAVE : GENERATE_WAVE;
			break;
		}
		default: {/* Do nothing.*/}
	}