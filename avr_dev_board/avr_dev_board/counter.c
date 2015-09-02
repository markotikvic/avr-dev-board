/*
 * counter.c
 *
 *  Created on: Jun 29, 2015
 *      Author: marko
 */


#include "counter.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile long counts = 0;

void enable_counter()
{
	set_pin_dir(&EXT_DDR, EXT_CH_A, INPUT);
	set_pin_dir(&EXT_DDR, EXT_CH_B, INPUT);
	EICRA |= (1 << ISC11);	/* Falling edge on INT1. */
	EIMSK |= (1 << INT1);
	sei();
}

long get_count()
{
	return counts;
}

ISR(INT1_vect)
{
	if( EXT_PORT & (1 << EXT_CH_A) ) counts--;			/* CCW */
	else if( !(EXT_PORT & (1 << EXT_CH_A)) ) counts++;	/* CW */
}
