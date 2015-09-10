/*
 * main.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */


#define F_CPU 8000000UL

#include <avr/io.h>
#include "gpio.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "counter.h"
#include "util.h"
#include <string.h>

int main()
{
	enable_h_bridge();
	init_uart(UART_9600);
	init_timer_1(PRESCALER8, 1000);

	while (1) {
		start_motor_CCW();
		send_string("Hello World from AVR CCW!\n\r");
		pause_loop(4000);
		stop_motor();
		send_string("Hello World from AVR stop!\n\r");
		pause_loop(2000);
		start_motor_CW();
		send_string("Hello World from AVR CW!\n\r");
		pause_loop(4000);
		stop_motor();
		send_string("Hello World from AVR stop!\n\r");
		pause_loop(2000);
	}
	return 0;
}
