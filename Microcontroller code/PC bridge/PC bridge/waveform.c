#include <avr/io.h>
#include <stdlib.h>

#include <util/delay.h>

#include "waveform.h"
#include "uart.h"
//LED
// @ mode on/off[1 byte] ;

//MOTOR
// @ mode number[1 byte] ;



//Waveform
// @ mode waveform[1 byte] amplitude[3 byte] frequency[3 byte] ;

#define _CMD_START_CNT 1
#define _CMD_END_CNT   1
#define _CMD_WAVE_CNT  1
#define _CMD_AMP_CNT   3
#define _CMD_FRQ_CNT   3

#define FULL_CMD_CNT (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT + _CMD_END_CNT)
#define WAVE_OFFSET  (_CMD_START_CNT)
#define AMP_OFFSET   (_CMD_START_CNT +  _CMD_WAVE_CNT)
#define FREQ_OFFSET  (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT)
#define MARKER_END   (_CMD_START_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT)
#define MARKER_START (0)

#define WAVEFORM_NUM 4

#define DAC_DDR  DDRB
#define DAC_PORT PORTB

typedef enum {GENERATE_WAVE, UPDATE_WAVE} states_t;

static uint8_t cmd_buffer[FULL_CMD_CNT];
static void (*waveform[WAVEFORM_NUM])(uint8_t amp, uint8_t freq);
static states_t currentState = GENERATE_WAVE;
static uint8_t amp_value = 0;
static uint8_t freq_value = 0;
static uint8_t waveform_index = WAVEFORM_NUM;


void squareWave(uint8_t amp, uint8_t freq)
{
    // TODO: Place ur code here
    DAC_DDR = 255;
    DAC_PORT = 1; 
}

void staircaseWave(uint8_t amp, uint8_t freq)
{
    // Refresh DAC DDR to be output.
    DAC_DDR = 255;

    // Generate waveform.
    DAC_PORT = 0x00;
    _delay_us(200);
    DAC_PORT = 0x33;
    _delay_us(200);
    DAC_PORT = 0x66;
    _delay_us(200);
    DAC_PORT = 0x99;
    _delay_us(200);
    DAC_PORT = 0xCC;
    _delay_us(200);
    DAC_PORT = 0xFF;
    _delay_us(200);
}

void triangleWave(uint8_t amp, uint8_t freq)
{
    // TODO: Place ur code here
    DAC_DDR = 255;
    DAC_PORT = 3;
}

void sineWave(uint8_t amp, uint8_t freq)
{
    // TODO: Place ur code here
    DAC_DDR = 255;
    DAC_PORT = 4;
}

void WAVE_Init(void)
{
    uint8_t i;
    
    
    /* Clear cmd_buffer. */
    for(i = 0; i < FULL_CMD_CNT; i += 1)
    {
        cmd_buffer[i] = 0;
    }
    
    /* Initialize waveform array. */
    waveform[0] = squareWave;
    waveform[1] = staircaseWave;
    waveform[2] = triangleWave;
    waveform[3] = sineWave;

    /* Start with getting which wave to generate. */ 
    currentState = UPDATE_WAVE;    
}

void WAVE_MainFunction(void)
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
}

