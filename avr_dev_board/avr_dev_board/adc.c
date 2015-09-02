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

volatile uint8_t adcDone = 0;
volatile int adcResult = 0;

void initAdc()
{
	ADMUX &= ~(0x0F | 1 << ADLAR);	//Right adjust, ADCO
	ADCSRA |= (1 << ADEN | 1 << ADIE | 1 << ADPS0 | 1 << ADPS1 | 1 << ADSC);
	DIDR0 |= (1 << PC0);

	sei();
}

void startNewAdc()
{
	if(adcDone) {
		adcDone = 0;
		ADMUX &= ~(0x0F | 1 << ADLAR);	//Right adjust, ADCO
		ADCSRA |= (1 << ADSC);
	}
}

int getAdcResult()
{
	/* If more than 8-bit precision is used - read ADCL first and ADCH second. */
	return adcResult;
}

ISR(ADC_vect)
{
	adcDone = 1;
	uint16_t adcResultTemp = 0;
	adcResultTemp |= ADCL;
	adcResultTemp |= (ADCH << 8);
	adcResult = adcResultTemp;

	//adcResult = (int)(adcResult + (float)((adcResultTemp - adcResult)*0.20));
}
