#include <avr/io.h>
#include <stdio.h>
#include "../../include/pid_control.h"
#include "../../include/gpio.h"
#include "../../include/uart.h"
#include "../../include/timer.h"
#include "../../include/counter.h"

int main(void)
{
	init_uart(UART_9600);
	init_timer_1(PRESCALER8, 1000);
	set_pin_dir(&DDRD, GPIO_PIN5, OUTPUT);
	pid_setup_params(1, 0, 0);
	pid_ref_val(1000);
	start_pid_control();
	char s[10] = {'\0'};
	
	for (;;) {
		send_string("Counts: ");
		sprintf(s, "%li", get_count());
		send_string(s);
		send_string("\n\r");
		s[0] = '\0';
		pause_loop(1000);
	}
}
