#include <avr/io.h>
#include <avr/interrupt.h>
#include "../include/adc.h"
#include "../include/gpio.h"

static volatile uint8_t adc_done = 0;
static volatile int adc_result = 0;

// init_adc initializes ADC modul.
void init_adc()
{
	ADMUX &= ~(0x0F | 1<<ADLAR); // Right align, ADCO
	ADCSRA |= (1<<ADEN | 1<<ADIE | 1<<ADPS0 | 1<<ADPS1 | 1<<ADSC);
	DIDR0 |= 1<<PC0;
	sei();
}

// start_new_adc starts a new conversion cycle.
void start_new_adc()
{
	if (adc_done) {
		adc_done = 0;
		ADMUX &= ~(0x0F | 1<<ADLAR); // Right align, ADCO
		ADCSRA |= 1<<ADSC;
	}
}

// get_adc_result returns the ineger value of last completed conversion cycle.
int get_adc_result()
{
	return adc_result;
}

ISR(ADC_vect)
{
	adc_done = 1;
	uint16_t temp = 0;
	temp |= ADCL;
	temp |= (ADCH<<8);
	adc_result = temp;
}
