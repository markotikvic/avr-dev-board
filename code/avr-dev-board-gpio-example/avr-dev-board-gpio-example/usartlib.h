/*
 * usartlib.h
 *
 * Created: 6/16/2015 11:55:05 PM
 *  Author: Lazar
 */ 


#ifndef USARTLIB_H_
#define USARTLIB_H_

#define _BAUD			9600					// Baud rate (9600 is default)
#define _DATA			0x03					// frame format 8n1
#define _UBRR			(F_CPU/16)/_BAUD - 1	// Used for UBRRL and UBRRH

/*	macros	*/
#define TX_START()		SBIT(UCSR0B, TXEN0)		// Enable TX
#define TX_STOP()		CBIT(UCSR0B, TXEN0)		// Disable TX
#define RX_START()		SBIT(UCSR0B, RXEN0)		// Enable RX
#define RX_STOP()		CBIT(UCSR0B, RXEN0)		// Disable RX
#define COMM_START()	TX_START(); RX_START()	// Enable communications

void init_usart();
void USART_send_char(unsigned char charToSend);
void USART_send_string(char* stringToSend);
void flush_input_buffer();
void USART_send_one_character(char a);


#endif /* USARTLIB_H_ */