#ifndef _WAVEFORM_GEN_H_
#define _WAVEFORM_GEN_H_


extern void WAVE_Init(void);
extern void WAVE_MainFunction(uint8_t *buffer);
extern void squareWave(uint8_t amp, uint8_t freq);
extern void staircaseWave(uint8_t amp, uint8_t freq);
extern void triangleWave(uint8_t amp, uint8_t freq);
extern void sineWave(uint8_t amp, uint8_t freq);
extern void rampWave(uint8_t amp, uint8_t freq);
extern void sawtoothWave(uint8_t amp, uint8_t freq);


#endif
