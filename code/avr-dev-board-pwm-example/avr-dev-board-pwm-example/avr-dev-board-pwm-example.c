/*
 * avr_dev_board_pwm_example.c
 *
 * Created: 10-Sep-15 23:26:06
 *  Author: marko
 *
 * Primer realizacije PWM (Pulse Width Modulation) signala
 * na nekom od pinova kontrolera. Za realizaciju PWM signala se
 * treba koristi timer.h biblioteku.
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include "timer.h"
#include "gpio.h"

int main(void)
{
	init_timer_1(PRESCALER8, 100);	//100us interrupt
	
	set_pin_dir(&DDRB, GPIO_PIN0, OUTPUT);	//Postavljanje pina na koji zelimo da prosledimo PWM signal
											//u izlazni rezim rada
	
	PWM_S pwm_init_struct;					//Stvaranje strukture za inicijalizaciju PWM-a
	pwm_init_struct.port = &PORTB;			//Port (mora biti isti kao i u set_pin_dir funkciji u prethodnom koraku)
	pwm_init_struct.pin = GPIO_PIN0;		//Pin (mora biti isti kao i u set_pin_dir funkciji u prethodnom koraku)
	pwm_init_struct.period = 200;			//20ms perioda
	pwm_init_struct.top_val = 0;			//Inicijalni faktor ispune
	init_pwm(&pwm_init_struct);				//Inicijalizacija PWM-a
		
	uint8_t i = 0;
	
    while(1){
		for(i = 0; i < 200; i++){			//Naizmenicno ispunjavanje faktora ispune od minimalne do maksimalne vrednosti
			pwm_ref_val(i);
			pause_loop(50);
		}
		for(i = 200; i > 0; i--){
			pwm_ref_val(i);
			pause_loop(50);
		}	
    }
}