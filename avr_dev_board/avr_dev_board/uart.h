/*
 * uart.h
 *
 *  Created on: Jun 30, 2015
 *      Author: marko
 */

#ifndef _UART_H_
#define _UART_H_

#include <avr/io.h>

#define UART0_SIZE	((uint8_t)255)

#define UART_9600	((uint8_t)51)
#define UART_19200	((uint8_t)25)

void init_uart(uint8_t baud);
void send_char(char c);
void send_string(char *str);
void get_buffer(char *dest, uint8_t size);
void clear_buffer();
void analyze_uart();

#endif /* _UART_H_ */
