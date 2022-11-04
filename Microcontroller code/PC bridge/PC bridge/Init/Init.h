#ifndef INIT_H_
#define INIT_H_

//Include
#include <avr/io.h>
#include <util/delay.h>

#include "../Lib/DIO/DIO.h"
#include "../Lib/USART/uart.h"
#include "../Lib/EEPROM/EEPROM.h"
//-----------------

// MODE SELECTION
#include "../Mode/Mode.h"
//-----------------

// LED
#include "../LED/LED.h"

extern void init();


#endif /* INIT_H_ */