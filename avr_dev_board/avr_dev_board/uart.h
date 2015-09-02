/*
 * uart.h
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#ifndef HEADERS_UART_H_
#define HEADERS_UART_H_

#include <avr/io.h>

#define UART0_SIZE	((uint8_t)255)

#define UART_9600	((uint8_t)51)
#define UART_19200	((uint8_t)25)

void initUart(uint8_t baud);
void sendChar(char c);
void sendString(char *str);
void getBuffer(char *dest, uint8_t size);
void clearBuffer();
void analizeUart0();

#endif /* HEADERS_UART_H_ */
