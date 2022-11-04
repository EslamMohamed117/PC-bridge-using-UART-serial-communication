#include "../Lib/DIO/DIO.h"
#include "../Motor/Motor.h"

volatile float duty_cycle =0;

void Motor_MainFunction(uint8_t *buffer)
{
	uint8_t speed = buffer[SPEED_OFFSET];
	if(speed == '+')
		duty_cycle = 1;
	else
		duty_cycle = (speed-0x30)/10.0;
	OCR2 = (uint8_t)(255* (1-duty_cycle));
	
}


void Motor_init()
{
	DIO_SET_PIN_DIR('A',1,1);
	DIO_SET_PIN_VAL('A',1,1);	// Setting direction of motor
	DIO_SET_PIN_DIR('A',2,1);	// Setting first two pins of port A as output for controlling the driver
	DIO_SET_PIN_VAL('A',2,0);	// Setting first two pins of port A as output for controlling the driver
	DIO_SET_PIN_DIR('D',7,1);	// Setting pin 3 of port b as output for PWM signal generated at OC2
	DIO_SET_PIN_VAL('D',7,0);
	

	// TIMER2
	TCCR2=0b01110101;	//Configure Timer2 to run at Phase correct PWM
}