/*
 * usartlib.c
 *
 * Created: 6/16/2015 11:54:41 PM
 *  Author: Lazar
 */ 

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h> 
#include "globals.h"
#include "util.h"
#include "usartlib.h"

char data;
volatile char usart_sent;
volatile char usart_buff[USART_BUFFER_SIZE];
volatile int curr_buffer_size;


void init_usart()
{
	// Set pin direction
	SBIT(DDRD, 1);
	CBIT(DDRD, 0);

	// Set baud rate = 9600; lower byte and top nibble
	UBRR0H = ((_UBRR) & 0xF00);
	UBRR0L = (uint8_t) ((_UBRR) & 0xFF);
	
	// Interrupt enable
	SBIT(UCSR0B, TXCIE0);
	SBIT(UCSR0B, RXCIE0);

	// Start communication modules
	COMM_START();

	// Set frame format = 8-N-1
	UCSR0C = (_DATA << UCSZ00);
}

void USART_send_char(unsigned char charToSend)
{
	usart_sent = 0;
	UDR0 = charToSend;
	while (usart_sent == 0);	//TODO: add timeout
}

void USART_send_string(char* stringToSend)
{
	while (*stringToSend != '\0')
	{
		USART_send_char(*stringToSend);
		stringToSend++;
	}
}

/*
//USART  receive complete interrupt routine
ISR(USART_RX_vect)
{	
	char temp;
	temp = UDR0;
	if (curr_buffer_size < USART_BUFFER_SIZE-1)
	{
		usart_buff[curr_buffer_size] = temp;
		curr_buffer_size++;
		usart_buff[curr_buffer_size] = '\0';
	}
	else
	{
		flush_input_buffer();
	}
}

//USART  transmit complete interrupt routine
ISR(USART_TX_vect)
{
	usart_sent = 1;
}

//Flashing input buffer for USART 
void flush_input_buffer()
{
	curr_buffer_size = 0;
	for (int i=0; i<USART_BUFFER_SIZE; i++)
	{
		usart_buff[i] = '\0';
	}
}*/

void USART_send_one_character(char a)
{
	char temp_answer [4];
	char temp_str[4];
	
	temp_answer[0] = '\0';
	itoa(a, temp_str, 10);
	strcat(temp_answer, temp_str);
	strcat(temp_answer, "\r\n");

	USART_send_string((char*) temp_answer);
}