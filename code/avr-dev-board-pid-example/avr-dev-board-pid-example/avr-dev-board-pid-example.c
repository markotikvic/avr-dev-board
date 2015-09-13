/*
 * avr_dev_board_pid_example.c
 *
 * Created: 13-Sep-15 14:43:05
 *  Author: marko
 *  
 *  Primer pozicone regulacije upotrebom ISC3806 enkodera i L298N H-mosta.
 *  (Sve unutar while(1) petlje je dodatak kako bi se povratna informacija o
 *  trenutnom stanju pozicije enkodera i nije neophodno za pravilan rad biblioteke.)
 */ 


#include <avr/io.h>
#include <stdio.h>
#include "pid_control.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "counter.h"

int main(void)
{
	init_uart(UART_9600);
	init_timer_1(PRESCALER8, 1000);
	set_pin_dir(&DDRD, GPIO_PIN5, OUTPUT);
	pid_setup_params(1, 0, 0);
	pid_ref_val(1000);
	start_pid_control();
	char s[10] = {'\0'};
	
    while(1){
        send_string("Counts: ");
        sprintf(s, "%li", get_count());
        send_string(s);
        send_string("\n\r");
        s[0] = '\0';
        pause_loop(1000);
    }
}