#define F_CPU 8000000UL

#include <avr/io.h>
#include "../../include/motor.h"
#include "../../include/timer.h"

int main()
{
	enable_h_bridge();
	init_timer_1(PRESCALER8, 1000);
	for (;;) {	
		start_motor_CCW();
		pause_loop(4000);
		stop_motor();	
		pause_loop(2000);
		start_motor_CW();
		pause_loop(4000);
		stop_motor();
		pause_loop(2000);
	}
	return 0;
}
