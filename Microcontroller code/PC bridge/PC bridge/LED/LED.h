#ifndef LED_H_
#define LED_H_

#include <avr/io.h>
#include <util/delay.h>
//LED
// @[1] mode[1] on/off[1 byte] dummy[1] dummy[4] dummy[1] ;[1]

#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1
#define _CMD_ONOFF_CNT  1

#define MODE_OFFSET		(_CMD_START_CNT)
#define ONOFF_OFFSET (_CMD_START_CNT +  _CMD_MODE_CNT)

typedef enum {OFF, ON} mode_t;


extern void LED_On();
extern void LED_Off();
extern void LED_Blink();
extern void LED_MainFunction(uint8_t *buffer);
extern void LED_Init();

#endif /* LED_H_ */