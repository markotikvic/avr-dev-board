#define F_CPU 8000000UL

#include <avr/io.h>
#include "../../include/timer.h"
#include "../../include/gpio.h"
#include "../../include/motor.h"

int main(void)
{
	init_timer_1(PRESCALER8, 100);
	set_pin_dir(&DDRB, GPIO_PIN0, OUTPUT);
	enable_h_bridge();
	PWM_S pwm_init_struct;
	pwm_init_struct.port = &PORTB;
	pwm_init_struct.pin = GPIO_PIN1;
	pwm_init_struct.period = 200;
	pwm_init_struct.top_val = 0;
	init_pwm(&pwm_init_struct);
		
	uint8_t i = 0;
	for (;;) {
		for (i = 0; i < 200; i++) {
			pwm_ref_val(i);	
			pause_loop(50);
		}
		for (i = 200; i > 0; i--) {
			pwm_ref_val(i);
			pause_loop(50);
		}
	}
}
