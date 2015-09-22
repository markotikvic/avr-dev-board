/*
 * avr_dev_board_uart_example.c
 *
 * Created: 9/10/2015 7:13:55 PM
 *  Author: Lazar
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/delay.h>
#include "gpio.h"
#include "uart.h"
#include "util.h"

int main(void)
{
	init_uart(UART_9600);	// Inicijalizacija serijske komunikacije

	send_string("Hello World from AVR-dev-board!\n\r");	// Pozdravna poruka
    
	// Inicijalizacija izlaza
	set_pin_dir(&DDRC, 0, OUTPUT);
	set_pin_dir(&DDRC, 1, OUTPUT);
	set_pin_dir(&DDRC, 3, OUTPUT);

	while(1)
    {
		analyze_uart();		// Analiziranje pristiglih komandi
		_delay_ms(10);		// Ubaceno da se ne bi zakocio procesor
	}
	
	return 0;
}