#include <avr/io.h>
#include "../DIO/STD_MACROS.h"

void EEPROM_WRITE(unsigned short address, unsigned char data)
{
	// EEAR = address;
	// Put Address in Address Register (10-bits)
	EEARL = (char) address;
	EEARH = (char) (address>>8);
	
	// Put Data in Data Register (8-bits)
	EEDR = data;
	
	// Set Write Mode
	SET_BIT(EECR , EEMWE);
	SET_BIT(EECR , EEWE);
	
	//Don't leave Function Until Write Finished (Until EEWE = 0)
	while(READ_BIT(EECR, EEWE) == 1 );
}

char EEPROM_u8READ(unsigned short address)
{
	// Put Address in Address Register (10-bits)
	EEARL = (char) address;
	EEARH = (char) (address>>8);
	
	// Set Read Mode
	SET_BIT(EECR , EERE);
	//Return Data (In Data Register)
	return EEDR;
}