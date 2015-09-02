/*
 * adc.h
 *
 *  Created on: Jun 21, 2015
 *      Author: marko
 */

#ifndef HEADERS_ADC_H_
#define HEADERS_ADC_H_

#include <avr/io.h>

#define DIDR0 _SFR_MEM8(0x7E)	/* Took it from <avr/iom328p.h> since <avr/io.h> doesn't have it. */

#define ADC_MAX_VAL ((uint16_t)1023)

void initAdc();
void startNewAdc();
int getAdcResult();

#endif /* HEADERS_ADC_H_ */
