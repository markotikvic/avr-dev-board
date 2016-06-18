#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/counter.h"
#include "../include/gpio.h"

static volatile long counts = 0;

// enable_counter enables counter module and configures it for counting falling edges.
void enable_counter()
{
	set_pin_dir(&EXT_DDR, EXT_CH_A, INPUT);
	set_pin_dir(&EXT_DDR, EXT_CH_B, INPUT);
	EICRA |= 1<<ISC11; // Falling edge on INT1
	EIMSK |= 1<<INT1;
	sei();
}

// get_count returns the current number of counts stored in the counter.
long get_count()
{
	return counts;
}

ISR(INT1_vect)
{
	if (EXT_PORT & (1<<EXT_CH_A)) {
		counts--; // CCW
	} else if(!(EXT_PORT & (1<<EXT_CH_A))) {
		counts++; // CW
	}
}
