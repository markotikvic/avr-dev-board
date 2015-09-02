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
	init_uart(UART_9600);
	enable_h_bridge();
	init_timer_1(PRESCALER8, 1000);
	pid_setup_params(1, 0, 1, 1000);	/* pid() loops every 10ms */
	pid_mot_direction(CCW);
	pid_ref_val(0);

	while (1) {
		start_motor_CCW();
		pause_loop(4000);
		stop_motor();
		pause_loop(4000);
		start_motor_CW();
		pause_loop(4000);
		stop_motor();
		pause_loop(4000);
	}
	return 0;
}
