#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#define DIDR0 _SFR_MEM8(0x7E)	// Took it from <avr/iom328p.h> since <avr/io.h> doesn't have it.

#define ADC_MAX_VAL ((uint16_t)1023)

void init_adc();
void start_new_adc();
int get_adc_result();

#endif /* ADC_H_ */
