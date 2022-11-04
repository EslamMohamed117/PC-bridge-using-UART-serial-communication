#ifndef MOTOR_H_
#define MOTOR_H_

#include <avr/io.h>
#include <avr/interrupt.h>

//MOTOR
// @[1] mode[1] speed[2 byte] dummy[5] ;[1]

#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1
#define _CMD_SPEED_CNT  2

#define MODE_OFFSET		(_CMD_START_CNT)
#define SPEED_OFFSET (_CMD_START_CNT +  _CMD_MODE_CNT)

extern void Motor_MainFunction(uint8_t *buffer);
extern void Motor_init();



#endif /* MOTOR_H_ */