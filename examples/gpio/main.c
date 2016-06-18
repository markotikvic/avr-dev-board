#define F_CPU 8000000UL

#include <avr/io.h>
#include "../../include/gpio.h"

int main(void)
{
	set_pin_dir(&DDRD, GPIO_PIN6, INPUT);
	set_pin_dir(&DDRD, GPIO_PIN7, INPUT);
	set_pin_dir(&DDRB, GPIO_PIN3, OUTPUT);
	set_pin_dir(&DDRB, GPIO_PIN4, OUTPUT);
	
	uint8_t toggle_flag = 0;
	for (;;) {
		if (read_pin(&PORTD, GPIO_PIN6)) {
			set_pin(&PORTB, GPIO_PIN3);
		} else {
			clear_pin(&PORTB, GPIO_PIN3);
		}

		if (read_pin(&PORTD, GPIO_PIN7) && !toggle_flag) {
			toggle_pin(&PORTB, GPIO_PIN4);
			toggle_flag = 1;
		} else {
			toggle_flag = 0;
		}  
	}
}
