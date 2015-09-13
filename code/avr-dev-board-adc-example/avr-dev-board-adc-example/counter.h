/*
 * counter.h
 *
 *  Created on: Jun 29, 2015
 *      Author: marko
 */

#ifndef _COUNTER_H_
#define _COUNTER_H_

#define EXT_DDR		DDRD
#define EXT_PORT	PIND
#define EXT_CH_A	GPIO_PIN2
#define EXT_CH_B	GPIO_PIN3

void enable_counter();
long get_count();

#endif /* _COUNTER_H_ */
