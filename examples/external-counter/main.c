#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../include/counter.h"
#include "../../include/uart.h"
#include "../../include/timer.h"

int main(void)
{
	init_uart(UART_9600);
	init_timer_1(PRESCALER8, 1000);
	enable_counter();
	char s[10] = {'\0'};
	
	for (;;) {
		send_string("Hello World from AVR!\n\r");
		send_string("Counts: ");
		sprintf(s, "%li", get_count());
		send_string(s);
		send_string("\n\r");
		s[0] = '\0';
		pause_loop(1000);
	}
}
