/*
 * avr_dev_board_adc_example.c
 *
 * Created: 13-Sep-15 22:21:22
 *  Author: marko
 *  
 *  Primer upotrebe adc.h biblioteke:
 *  Ukoliko je vrednost napona na potenciometru sa okruzenja
 *  veca od odredjene vrednost upalice se signalna dioda, u
 *  supotronom dioda ostaje ugasena.
 *  
 */ 


#include <avr/io.h>
#include <stdio.h>
#include "gpio.h"
#include "adc.h"
#include "timer.h"
#include "uart.h"

int main(void)
{
	init_timer_1(PRESCALER8, 1000);
	set_pin_dir(&DDRD, GPIO_PIN5, OUTPUT);
	init_uart(UART_9600);
	init_adc();
	
	char s[30] = {'\0'};
	int res = 0;
	
    while(1){
	    start_new_adc();
		res = get_adc_result();
		if(res > 300){
			set_pin(&PORTD, GPIO_PIN5);
		} else {
			clear_pin(&PORTD, GPIO_PIN5);
		}
		sprintf(s, "ADC: %d \n\r", res);
		send_string(s);
		s[0] = '\0';
		pause_loop(50);
    }
}