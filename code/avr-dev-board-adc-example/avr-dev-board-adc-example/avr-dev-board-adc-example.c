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
#include "gpio.h"
#include "adc.h"
#include "timer.h"

int main(void)
{
	init_timer_1(PRESCALER8, 1000);
	set_pin_dir(&DDRD, GPIO_PIN5, OUTPUT);
	init_adc();
	
    while(1){
	    start_new_adc();
		pause_loop(50);
		if(get_adc_result() > 300) set_pin(&PORTB, GPIO_PIN5);
		else clear_pin(&PORTB, GPIO_PIN5);
    }
}