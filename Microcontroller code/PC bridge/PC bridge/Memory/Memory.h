/*
 * Memory.h
 *
 * Created: 11/3/2022 12:22:14 PM
 *  Author: Eslam M.Ashour
 */ 


#ifndef MEMORY_H_
#define MEMORY_H_

#define READ_RAM(address) *((char *) (address))
#define WRITE_RAM(address,value) *((char *) (address))=value

#define _CMD_START_CNT 1
#define _CMD_MODE_CNT 1
#define _CMD_READWRITE_CNT  1
#define _CMD_RAMROM_CNT   1
#define _CMD_ADDRESS_CNT   4
#define _CMD_VALUE_CNT   1

#define MODE_OFFSET		(_CMD_START_CNT)
#define RAMROM_OFFSET (_CMD_START_CNT +  _CMD_MODE_CNT)
#define READWRITE_OFFSET	(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_RAMROM_CNT)
#define ADDRESS_OFFSET  (_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_RAMROM_CNT + _CMD_READWRITE_CNT)
#define VALUE_OFFSET	(_CMD_START_CNT +  _CMD_MODE_CNT + _CMD_RAMROM_CNT + _CMD_READWRITE_CNT + _CMD_ADDRESS_CNT)

typedef enum {RAM, ROM} memory_t;
typedef enum {READ_M, WRTIE_M} operations_t;



#include "../LED/LED.h"
extern void Memory_MainFunction(uint8_t *buffer);


#endif /* MEMORY_H_ */