/*
 * uart.c
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#include "uart.h"
#include <string.h>
#include <avr/interrupt.h>

static volatile char uart0_buff[UART0_SIZE];
static volatile char uart0_sent;
static volatile char uart0_received;
static volatile uint8_t uart0_index;

void init_uart(uint8_t baud)
{
	uart0_sent = 0;
	uart0_received = 0;
	uart0_index = 0;
	uart0_buff[uart0_index] = '\0';
	UCSR0B |= (1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00 | 1 << UCSZ00);
	UBRR0 = baud;
	
	sei();
}

void send_char(char c)
{
	uart0_sent = 0;
	UDR0 = c;
	while (!(UCSR0A & (1 << UDRE0)));
}

void send_string(char *str)
{
	while (*str != '\0') {
		send_char(*str);
		str++;
	}
}

void get_buffer(char *dest, uint8_t size)
{
	int i = 0;
	while(i < size && uart0_buff[i] != '\0') {
		(*dest) = uart0_buff[i];
		dest++;
		i++;
	}
	*dest = '\0';
}

void clear_buffer()
{
	uart0_index = 0;
	uart0_buff[uart0_index] = '\0';
}

void analyze_uart()
{
	if (uart0_received) {
		char temp[256];

		uart0_received = 0;
		get_buffer(temp, uart0_index);
		send_string(temp);
		clear_buffer();
	}
}

//ISR(USART_TX_vect)
//{
//	uart0_sent = 1;
//}

ISR(USART_RX_vect)
{
	char temp;

	temp = UDR0;
	if (uart0_index < (UART0_SIZE - 2)) {
		/* There is room for more characters. */
		uart0_buff[uart0_index] = temp;
		uart0_buff[uart0_index + 1] = '\0';
		uart0_index++;
		if (temp == '\0' || temp == '\n' || temp == '\r') {
			uart0_received = 1;
		}
	} else {
		/* Wait for buffer to be cleared. */
		clear_buffer();	
	}
}
