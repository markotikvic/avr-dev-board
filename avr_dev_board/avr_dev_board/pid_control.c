/*
 * pid_control.c
 *
 * Created: 12-Sep-15 21:29:24
 *  Author: marko
 *  
 *  Ova biblioteka je namenja za pozicionu PID regulaciju motora sa enkoderom.
 *  Bilbioteka pruza sledeci skup mogucnosti korisniku:
 *  (Napomena: sve makro definicije biblioteke se nalaze u datoteci pid_control.h)
 *  
 */ 

#include "pid_control.h"
#include "motor.h"
#include "counter.h"
#include "gpio.h"

#define PID_CONST ((uint8_t)10)

static void pid();
static volatile PID_S my_pid;
static volatile uint8_t pid_flag = 0;

static void init_timer_0()
{
	uint16_t temp;

	temp = 0;
	temp |= (1 << WGM00 | 1 << WGM01);	/* Fast PWM mode - TOP is OCR0A, auto restart. */
	TCCR0A = temp;

	temp = 0;
	temp |= (1 << WGM02);				/* Fast PWM mode - TOP is OCR1A, auto restart. */
	temp |= (0x02 << CS00);				/* CS2:0 = 010 - Prescaler set to 8. */
	TCCR0B = temp;

	temp = 0;
	temp |= (1 << TOIE0);
	TIMSK0 = temp;

	OCR0A = 100;						/* 100us interrupts */

	sei();
}

void pid_setup_params(float kp, float ki, float kd)
{
	my_pid.kp = kp;
	my_pid.ki = ki;
	my_pid.kd = kd;
	my_pid.sample_period = 500;	//50ms for pid loop
	my_pid.sample_peiod_opt = 1 / 500;
	my_pid.pwm_period = 200;	//20ms period for motors
	
	my_pid.last_count = 0;
	my_pid.last_err = 0;
	my_pid.ref_value = 0;
	my_pid.pid_pwm_tick = 0;
	my_pid.pid_tick_sample = 0;
	my_pid.dir = CW;

	enable_counter();
	enable_h_bridge();
	init_timer_0();
}

void pid_ref_val(long ref_val)
{
	my_pid.ref_value = ref_val;
}

void pid_mot_direction(uint8_t mot_dir)
{
	my_pid.dir = mot_dir;
}

//Gets called in timer 0 interrupt routine.
static void pid()
{
	long count = get_count();
	long temp = 0;
	my_pid.err = my_pid.ref_value - count;	/* Position difference. */

	my_pid.err_sum += my_pid.err;					/* Error sum. */
	if(my_pid.err_sum > 200) my_pid.err_sum = 200;	/* Anti wind-up */

	my_pid.err_diff = (long)((my_pid.err - my_pid.last_err)*my_pid.sample_peiod_opt);	/* 1st order error differential. */

	/* PID output */
	temp = my_pid.kp*my_pid.err + my_pid.ki*my_pid.err_sum + my_pid.kd*my_pid.err_diff;
	if(temp > 0) {
		my_pid.dir = CW;
		if(temp > my_pid.pwm_period) my_pid.pwm_top_val = my_pid.pwm_period;
		else my_pid.pwm_top_val = (uint16_t)(temp);
	} else {
		my_pid.dir = CCW;
		if(temp < -my_pid.pwm_period) my_pid.pwm_top_val = my_pid.pwm_period;
		else my_pid.pwm_top_val = (uint16_t)(-temp);
	}
		

	/* Memorize last values. */
	my_pid.last_count = count;
	my_pid.last_err = my_pid.err;
}

void stop_pid_control()
{
	pid_flag = PID_OFF;
}

void start_pid_control()
{
	pid_flag = PID_ON;
}

ISR(TIMER0_OVF_vect)
{
	if(pid_flag == PID_ON) {
		my_pid.pid_tick_sample++;
		if(my_pid.pid_tick_sample >= my_pid.sample_period) {
			my_pid.pid_tick_sample = 0;
			pid();
		}

		my_pid.pid_pwm_tick++;
		if(my_pid.pid_pwm_tick >= my_pid.pwm_period && my_pid.pwm_top_val > 0) {
			my_pid.pid_pwm_tick = 0;
			if(my_pid.dir == CW){
				MOT_PORT |= (1 << MOT_PIN0);
				MOT_PORT &= ~(1 << MOT_PIN1);
			} else {
				MOT_PORT |= (1 << MOT_PIN1);
				MOT_PORT &= ~(1 << MOT_PIN0);	
			}
		} else if (my_pid.pid_pwm_tick >= my_pid.pwm_top_val) {
			MOT_PORT &= ~(1 << MOT_PIN0);
			MOT_PORT &= ~(1 << MOT_PIN1);
		}
	}
}
	