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

long counts = 0;

void enableCounter()
{
	setPinDir(&EXT_DDR, EXT_CH_A, INPUT);
	setPinDir(&EXT_DDR, EXT_CH_B, INPUT);
	EICRA |= (1 << ISC11);	/* Falling edge on INT1. */
	EIMSK |= (1 << INT1);
	sei();
}

long getCount()
{
	return counts;
}

ISR(INT1_vect)
{
	/* CCW */
	if( EXT_PORT & (1 << EXT_CH_A) ) {
		counts--;
	} else if( !(EXT_PORT & (1 << EXT_CH_A)) ) {
	/* CW */
		counts++;
	}
}
