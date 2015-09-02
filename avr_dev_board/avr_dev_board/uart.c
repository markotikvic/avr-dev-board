/*
 * uart.c
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#include "uart.h"
#include <string.h>
#include <avr/interrupt.h>

volatile char uart0[UART0_SIZE];
volatile char uart0_sent;
volatile char uart0_received;
volatile uint8_t uart0Index;

void initUart(uint8_t baud)
{
	uart0_sent = 0;
	uart0_received = 0;
	uart0Index = 0;
	uart0[uart0Index] = '\0';
	UCSR0B |= (1 << RXEN0 | 1 << TXEN0 | 1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00 | 1 << UCSZ00);
	UBRR0 = baud;
	
	sei();
}

void sendChar(char c)
{
	uart0_sent = 0;
	UDR0 = c;
	while (!(UCSR0A & (1 << UDRE0)));
}

void sendString(char *str)
{
	while (*str != '\0') {
		sendChar(*str);
		str++;
	}
}

void getBuffer(char *dest, uint8_t size)
{
	int i = 0;
	while(i < size && uart0[i] != '\0') {
		(*dest) = uart0[i];
		dest++;
		i++;
	}
	*dest = '\0';
}

void clearBuffer()
{
	uart0Index = 0;
	uart0[uart0Index] = '\0';
}

void analizeUart0()
{
	if (uart0_received) {
		char temp[256];

		uart0_received = 0;
		getBuffer(temp, uart0Index);
		sendString(temp);
		clearBuffer();
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
	if (uart0Index < (UART0_SIZE - 2)) {
		/* There is room for more characters. */
		uart0[uart0Index] = temp;
		uart0[uart0Index + 1] = '\0';
		uart0Index++;
		if (temp == '\0' || temp == '\n' || temp == '\r') {
			uart0_received = 1;
		}
	} else {
		/* Wait for buffer to be cleared. */
		clearBuffer();	
	}
}
