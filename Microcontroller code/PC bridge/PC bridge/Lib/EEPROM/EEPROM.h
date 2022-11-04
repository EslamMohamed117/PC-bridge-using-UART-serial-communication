#ifndef EEPROM_H_
#define EEPROM_H_


/*
	Function Name        : EEPROM_WRITE
	Function Return      : void
	function Arguments   : unsigned short address, unsigned char data
	function Description : Write on EEPROM the given Data in the given Address
*/
void EEPROM_WRITE(unsigned short address, unsigned char data);


/*
	Function Name        : EEPROM_u8READ
	Function Return      : void
	function Arguments   : unsigned short address
	function Description : Read EEPROM from the given Address
*/
char EEPROM_u8READ(unsigned short address);

#endif /* EEPROM_H_ */