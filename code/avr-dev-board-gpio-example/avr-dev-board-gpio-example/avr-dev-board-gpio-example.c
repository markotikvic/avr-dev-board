/*
 * avr_dev_board_gpio_example.c
 *
 * Created: 10-Sep-15 20:47:33
 *  Author: marko
 *
 * Primer upotrebe gpio.h biblioteke:
 *     U zavisnosti od toga da li je taster 1 na razojnom okruzenju pritisnut ili ne
 *     dioda na pinu PB0 ce se upaliti odnosno ugasiti.
 *     Na svaki pritisak tastera 2 na razvojnom orkuzenju dioda na pinu PB1 ce promeniti
 *     svoje stanje (on -> off; off -> on);
 */ 

#define F_CPU 8000000UL

#include <avr/io.h>
#include "gpio.h"

int main(void)
{
	set_pin_dir(&DDRD, GPIO_PIN6, INPUT);		//Podesi PD6 kao ulaz
	set_pin_dir(&DDRD, GPIO_PIN7, INPUT);		//Podesi PD7 kao ulaz
	set_pin_dir(&DDRB, GPIO_PIN3, OUTPUT);		//Podesi PB3 kao izlaz
	set_pin_dir(&DDRB, GPIO_PIN4, OUTPUT);		//Podesi PB4 kao izlaz
	
	uint8_t toggle_flag = 0;
	
    while(1){
		if(!read_pin(&PORTD, GPIO_PIN6)) set_pin(&PORTB, GPIO_PIN3);	//Ukoliko je pritisnut taster na pinu PD6 upali diodu na pinu PB3
		else clear_pin(&PORTB, GPIO_PIN3);							//u suprotnom je ugasi
		
		if(!read_pin(&PORTD, GPIO_PIN7) && !toggle_flag){			//Svaki put kada je pritisnut taster na pinu PD7 promeni stanje pina PB4
			toggle_pin(&PORTB, GPIO_PIN4);
			toggle_flag = 1;
		} else if(read_pin(&PORTD, GPIO_PIN7)){
			toggle_flag = 0;
		}  
    }
}