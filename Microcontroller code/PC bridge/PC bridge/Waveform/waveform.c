#include <avr/io.h>
#include <stdlib.h>

#include <util/delay.h>

#include "../Waveform/waveform.h"
#include "../Lib/USART/uart.h"


//Waveform
// @[1 byte] mode[1 byte] waveform[1 byte] amplitude[3 byte] frequency[3 byte] ;[1 byte]

#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1
#define _CMD_END_CNT   1
#define _CMD_WAVE_CNT  1
#define _CMD_AMP_CNT   3
#define _CMD_FRQ_CNT   3

#define FULL_CMD_CNT (_CMD_START_CNT + _CMD_MODE_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT + _CMD_END_CNT)
#define WAVE_OFFSET  (_CMD_START_CNT + _CMD_MODE_CNT)
#define AMP_OFFSET   (_CMD_START_CNT + _CMD_MODE_CNT +  _CMD_WAVE_CNT)
#define FREQ_OFFSET  (_CMD_START_CNT + _CMD_MODE_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT)
#define MARKER_END   (_CMD_START_CNT + _CMD_MODE_CNT +  _CMD_WAVE_CNT + _CMD_AMP_CNT + _CMD_FRQ_CNT)
#define MARKER_START (0)

#define WAVEFORM_NUM 4

#define DAC_DDR  DDRB
#define DAC_PORT PORTB

typedef enum {GENERATE_WAVE, UPDATE_WAVE} states_t;

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
    
    /* Initialize waveform array. */
    waveform[0] = squareWave;
    waveform[1] = staircaseWave;
    waveform[2] = triangleWave;
    waveform[3] = sineWave;

    /* Start with getting which wave to generate. */ 
    currentState = UPDATE_WAVE;    
}

void WAVE_MainFunction(uint8_t *buffer)
{    
	{
		char amp_buffer[_CMD_AMP_CNT];
        for(uint8_t i = 0; i < _CMD_AMP_CNT; ++i) { amp_buffer[i] = buffer[AMP_OFFSET+i]; }
        amp_value = atoi(amp_buffer);
	}

    /* Compute frequency. */
    {
		char freq_buffer[_CMD_FRQ_CNT];
        for(uint8_t i = 0; i < _CMD_FRQ_CNT; ++i) { freq_buffer[i] = buffer[FREQ_OFFSET+i]; }
        freq_value = atoi(freq_buffer);
	}

    /* Compute waveform. */
    {
		waveform_index = buffer[WAVE_OFFSET] - 0x30;
    }
    waveform[waveform_index](amp_value, freq_value);
}

