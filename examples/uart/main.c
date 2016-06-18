#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include "../../include/uart.h"
#include "../../include/util.h"

int main(void)
{
	init_uart(UART_9600);
	send_string("Hello World from AVR-dev-board!\n\r");
    
	for (;;) {
		analyze_uart();
		_delay_ms(10);
	}
	return 0;
}
