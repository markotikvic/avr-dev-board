#ifndef COUNTER_H_
#define COUNTER_H_

#define EXT_DDR		DDRD
#define EXT_PORT	PIND
#define EXT_CH_A	GPIO_PIN2
#define EXT_CH_B	GPIO_PIN3

void enable_counter();
long get_count();

#endif /* COUNTER_H_ */
