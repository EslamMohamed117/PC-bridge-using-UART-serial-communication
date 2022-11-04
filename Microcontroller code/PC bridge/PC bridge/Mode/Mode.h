#ifndef MODE_H_
#define MODE_H_

#include <avr/io.h>

#define FULL_CMD_CNT 10
#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1

#define MODE_OFFSET		(_CMD_START_CNT)

#define MARKER_START (0)
#define MARKER_END (9)

static uint8_t cmd_buffer[FULL_CMD_CNT];

#include "../Lib/USART/uart.h"


// MOTOR
#include "../Motor/Motor.h"
//-----------------

// MEMORY
#include "../Memory/Memory.h"
//-----------------

// LED
#include "../LED/LED.h"

extern void Mode_Select();

#endif /* MODE_H_ */