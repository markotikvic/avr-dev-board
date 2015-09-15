/*
 * avr_dev_board_ext_counter_example.c
 *
 * Created: 12-Sep-15 22:57:54
 *  Author: marko
 *  
 *  Primer upotrebe counter.h biblioteke:
 *		Na svaku sekundu kontroler putem UART komunikacije salje trenutno stanje
 *		brojaca. Pre slanja, podatak moramo pretvoriti iz tipa long u tip char*
 *		upotrebom sprintf funkcije koja se nalazi u stdio.h biblioteci.
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "counter.h"
#include "uart.h"
#include "timer.h"

int main(void)
{
	init_uart(UART_9600);
	init_timer_1(PRESCALER8, 1000);
	enable_counter();
	char s[10] = {'\0'};
	
    while(1){
        send_string("Hello World from AVR!\n\r");
		send_string("Counts: ");
		sprintf(s, "%li", get_count());
		send_string(s);
		send_string("\n\r");
		s[0] = '\0';
		pause_loop(1000);
    }
}