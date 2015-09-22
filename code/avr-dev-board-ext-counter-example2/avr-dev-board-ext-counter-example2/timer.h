/*
 * timer.h
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#ifndef _TIMER_H_
#define _TIMER_H

#include <avr/io.h>

#define PRESCALER1 ((uint8_t)0b00000001)
#define PRESCALER8 ((uint8_t)0b00000010)
#define PRESCALER64 ((uint8_t)0b00000011)

#define PWM_OFF ((uint8_t)0)
#define PWM_ON ((uint8_t)1)

typedef struct {
	volatile uint8_t *port;
	uint8_t pin;
	uint32_t period;
	uint32_t top_val;
	uint32_t pwm_tick;
} PWM_S;

void init_timer_1(uint8_t prescaler, uint16_t top_val);
void set_timer(uint8_t timer_num, uint32_t time);
int get_timer(uint8_t timer_num);
void pause_loop(uint16_t pause);

void init_pwm(PWM_S *pwm);
void stop_pwm(void);
void start_pwm(void);
void pwm_ref_val(uint32_t reff);

void threads_loop_example(void);

#endif /* _TIMER_H_ */
