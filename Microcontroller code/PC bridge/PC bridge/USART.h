#ifndef USART_H_
#define USART_H_




/*
	Function Name        : USART_INIT
	Function Return      : void
	function Arguments   : unsigned long
	function Description : Initialize USART Protocol Using USART Protocol
*/
void USART_INIT(unsigned long baud);

void USART_Transmit( unsigned char data );
unsigned char USART_Receive( void );

/*
	Function Name        : USART_SEND_DATA
	Function Return      : void
	function Arguments   : char
	function Description : Transmit data to another Module Using USART Protocol
*/
void USART_SEND_DATA(char data);





/*
	Function Name        : USART_RECIEVE_DATA
	Function Return      : void
	function Arguments   : void
	function Description : Receive data from another Module Using USART Protocol
*/
char USART_RECIEVE_DATA(void);




/*
	Function Name        : USART_SEND_STRING
	Function Return      : void
	function Arguments   : char
	function Description : Transmit String to another Module Using USART Protocol
*/
void USART_SEND_STRING(char *p);


#endif /* USART_H_ */