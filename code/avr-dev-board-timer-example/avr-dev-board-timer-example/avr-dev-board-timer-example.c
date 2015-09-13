/*
 * avr_dev_board_timer_example.c
 *
 * Created: 13-Sep-15 22:01:29
 *  Author: marko
 *  
 *  Primer za demonstraciju pseudo paralelnog izvrsavanja
 *  koda koji je uslovljen vremenskim kasnjenjem. Funkcionalnost
 *  f-ja u timer.h biblioteci je primarno fokusirana na sto manje
 *  sekvencijalno izvrsavanje programa bez nepotrebnog zadrzavanja.
 *  Kod se unutar kontrolera izvrsava sekvencijalno i sinhrono dok
 *  odaje utisak da poseduje mogucnost izvrsavanja asinhronih i
 *  event-driven naredbi.
 */ 

#include <avr/io.h>
#include "timer.h"
#include "gpio.h"

int main(void)
{
	init_timer_1(PRESCALER8, 100);
	set_pin_dir(&DDRD, GPIO_PIN5, OUTPUT);
	set_pin_dir(&DDRD, GPIO_PIN6, OUTPUT);
	set_pin_dir(&DDRD, GPIO_PIN7, OUTPUT);
	
    while(1){
		threads_loop_example();
    }
}