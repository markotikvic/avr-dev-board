/*
 * adc.c
 *
 *  Created on: Jun 21, 2015
 *      Author: marko
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "gpio.h"

static volatile uint8_t adc_done = 0;
static volatile int adc_result = 0;

void init_adc()
{
	ADMUX &= ~(0x0F | 1 << ADLAR);	//Right adjust, ADCO
	ADCSRA |= (1 << ADEN | 1 << ADIE | 1 << ADPS0 | 1 << ADPS1 | 1 << ADSC);
	DIDR0 |= (1 << PC0);

	sei();
}

void start_new_adc()
{
	if(adc_done) {
		adc_done = 0;
		ADMUX &= ~(0x0F | 1 << ADLAR);	//Right adjust, ADCO
		ADCSRA |= (1 << ADSC);
	}
}

int get_adc_result()
{
	/* If more than 8-bit precision is used - read ADCL first and ADCH second. */
	return adc_result;
}

ISR(ADC_vect)
{
	adc_done = 1;
	uint16_t temp = 0;
	temp |= ADCL;
	temp |= (ADCH << 8);
	adc_result = temp;

	//adc_result = (int)(adc_result + (float)((temp - adc_result)*0.20));
}
