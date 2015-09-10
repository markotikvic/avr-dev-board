/*
 * timer.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "counter.h"
#include "uart.h"
#include "util.h"

#define PID_CONST ((uint8_t)10)

static PWM_S my_pwm;
static PID_S my_pid;
static volatile uint32_t timer_node_0 = 0;
static volatile uint32_t timer_node_1 = 0;
static volatile uint32_t timer_node_2 = 0;
static volatile uint32_t pause_time = 0;
static uint8_t pwm_flag = PWM_OFF;
static uint8_t pid_flag = 0;
static uint8_t fsm_step = 0;

void init_timer_1(uint8_t prescaler, uint16_t topw_val)
{
	uint16_t temp;

	temp = 0;
	temp |= (1 << WGM10 | 1 << WGM11); /* Fast PWM mode - TOP is OCR1A, auto restart. */
	TCCR1A = temp;

	temp = 0;
	temp |= (1 << WGM12 | 1 << WGM13); /* Fast PWM mode - TOP is OCR1A, auto restart. */
	temp |= (prescaler << CS10); /* CS2:0 = 010 - Prescaler set to 8. */
	TCCR1B = temp;

	temp = 0;
	temp |= (1 << TOIE0);
	TIMSK1 = temp;

	OCR1A = topw_val;

	sei();
}

void init_pwm(PWM_S *pwm)
{
	my_pwm.target_port = pwm->target_port;
	my_pwm.pin = pwm->pin;
	my_pwm.period = (volatile uint32_t) pwm->period;
	my_pwm.top_val = (volatile uint32_t) pwm->top_val;
	my_pwm.pwm_tick = 0;
	pwm_flag = PWM_ON;
}

void pwm_ref_val(uint32_t reff)
{
	if(reff < my_pwm.period ) {
		my_pwm.top_val = reff;
	} else {
		my_pwm.top_val = my_pwm.period;
	}
}

void stop_pwm()
{
	pwm_flag = PWM_OFF;
}

void start_pwm()
{
	pwm_flag = PWM_ON;
}

void pid_setup_params(float kp, float ki, float kd, uint16_t sample_period)
{
	my_pid.kp = kp;
	my_pid.ki = ki;
	my_pid.kd = kd;
	my_pid.sample_period = sample_period;
	my_pid.sample_peiod_opt = 1 / sample_period;
	my_pid.pwm_period = 200; 	//20ms for motors

	enable_counter();
	init_timer_1(PRESCALER8, 100);	/* 0.1 ms */
	enable_h_bridge();
}

void pid_ref_val(uint16_t ref_speed)
{
	my_pid.ref_speed = ref_speed;
	pid_flag = 1;
}

void pid_mot_direction(uint8_t mot_dir)
{
	my_pid.dir = mot_dir;
}

void pid()
{
	long count = get_count();
	int temp = 0;
	/* Speed difference. */
	my_pid.err = my_pid.ref_speed - (uint16_t)(count - my_pid.last_count);

	/* Error sum. */
	my_pid.err_sum += my_pid.err;
	if(my_pid.err_sum > 200) {
		my_pid.err_sum = 200;
	}

	/* Error difference. */
	my_pid.err_diff = my_pid.err - my_pid.last_err;

	/* PID output */
	temp = (my_pid.kp*my_pid.err + my_pid.ki*my_pid.err_sum + my_pid.kd*my_pid.err_diff)/PID_CONST;
	if(temp > my_pid.pwm_period) {
		my_pid.pwm_top_val = my_pid.pwm_period;
	} else {
		my_pid.pwm_top_val = temp;
	}

	/* Memorize last values. */
	my_pid.last_count = count;
	my_pid.last_err = my_pid.err;
}


void stop_pid_control()
{
	pid_flag = PID_OFF;
}

void set_timer(uint8_t timer_num, uint32_t time)
{
	switch(timer_num) {
	case 0:
		timer_node_0 = time;
		break;
	case 1:
		timer_node_1 = time;
		break;
	case 2:
		timer_node_2 = time;
		break;
	}
}

int get_timer(uint8_t timer_num)
{
	switch(timer_num) {
	case 0:
		return timer_node_0;
	case 1:
		return timer_node_1;
	case 2:
		return timer_node_2;
	default: return 1;
	}
}

void pause_loop(uint16_t pause)
{
	pause_time = pause;
	while(pause_time);
}

void therads_loop_example()
{
	switch(fsm_step){
	case 0:
		if(!get_timer(0)) {
			toggle_pin(&PORTD, GPIO_PIN6);
			set_timer(0, 250);
			pause_loop(1);
		}
		fsm_step++;
		break;
	case 1:
		if(!get_timer(1)) {
			toggle_pin(&PORTB, GPIO_PIN0);
			set_timer(1, 500);
			pause_loop(1);
		}
		fsm_step++;
		break;
	case 2:
		if(!get_timer(2)) {
			toggle_pin(&PORTD, GPIO_PIN7);
			set_timer(2, 1000);
			pause_loop(1);
		}
		fsm_step = 0;
		break;
	}
}

ISR(TIMER1_OVF_vect)
{
	if(pwm_flag == PWM_ON) {
		my_pwm.pwm_tick++;
		if(my_pwm.pwm_tick >= my_pwm.period && my_pwm.top_val > 0) {
			write_pin(my_pwm.target_port, my_pwm.pin, HIGH);
			my_pwm.pwm_tick = 0;
		} else if(my_pwm.pwm_tick >= my_pwm.top_val) {
			write_pin(my_pwm.target_port, my_pwm.pin, LOW);
		}
	}

	if(pid_flag == 1) {
		my_pid.pid_tick_sample++;
		if(my_pid.pid_tick_sample >= my_pid.sample_period) {
			my_pid.pid_tick_sample = 0;
			pid();
		}

		my_pid.pid_pwm_tick++;
		if(my_pid.pid_pwm_tick >= my_pid.pwm_period && my_pid.pwm_top_val > 0) {
			my_pid.pid_pwm_tick = 0;

			if(my_pid.dir == CW) {
				MOT_PORT |= (1 << MOT_PIN0);
			} else if(my_pid.dir == CCW) {
				MOT_PORT |= (1 << MOT_PIN1);
			}
		} else if (my_pid.pid_pwm_tick >= my_pid.pwm_top_val) {
			if(my_pid.dir == CW) {
				MOT_PORT &= ~(1 << MOT_PIN0);
			} else if(my_pid.dir == CCW) {
				MOT_PORT &= ~(1 << MOT_PIN1);
			}
		}
	}


	if(timer_node_0 > 0) timer_node_0--;
	if(timer_node_1 > 0) timer_node_1--;
	if(timer_node_2 > 0) timer_node_2--;
	if(pause_time > 0) pause_time--;
}
