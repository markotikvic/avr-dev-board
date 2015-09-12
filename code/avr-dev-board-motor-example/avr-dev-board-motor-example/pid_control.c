/*
 * pid_control.c
 *
 * Created: 12-Sep-15 21:29:24
 *  Author: marko
 */ 

#include "pid_control.h"
#include "motor.h"
#include "counter.h"

#define PID_CONST ((uint8_t)10)

static volatile PID_S my_pid;
static volatile uint8_t pid_flag = 0;

static void init_timer_0()
{
	uint16_t temp;

	temp = 0;
	temp |= (1 << WGM00 | 1 << WGM01);	/* Fast PWM mode - TOP is OCR0A, auto restart. */
	TCCR0A = temp;

	temp = 0;
	temp |= (1 << WGM02);	/* Fast PWM mode - TOP is OCR1A, auto restart. */
	temp |= (0x02 << CS00);				/* CS2:0 = 010 - Prescaler set to 8. */
	TCCR0B = temp;

	temp = 0;
	temp |= (1 << TOIE0);
	TIMSK1 = temp;

	OCR0A = 100;

	sei();
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
	enable_h_bridge();
	init_timer_0();
}

void pid_ref_val(uint16_t ref_speed)
{
	my_pid.ref_speed = ref_speed;
}

void pid_mot_direction(uint8_t mot_dir)
{
	my_pid.dir = mot_dir;
}

//Gets called in timer 0 interrupt routine.
static void pid()
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

void start_pid_control()
{
	pid_flag = PID_ON;
}

ISR(TIMER0_OVF_vect)
{
	if(pid_flag == 1) {
		my_pid.pid_tick_sample++;
		if(my_pid.pid_tick_sample >= my_pid.sample_period) {
			my_pid.pid_tick_sample = 0;
			pid();
		}

		my_pid.pid_pwm_tick++;
		if(my_pid.pid_pwm_tick >= my_pid.pwm_period && my_pid.pwm_top_val > 0) {
			my_pid.pid_pwm_tick = 0;
			if(my_pid.dir == CW) MOT_PORT |= (1 << MOT_PIN0);
			else if(my_pid.dir == CCW) MOT_PORT |= (1 << MOT_PIN1);
			} else if (my_pid.pid_pwm_tick >= my_pid.pwm_top_val) {
			if(my_pid.dir == CW) MOT_PORT &= ~(1 << MOT_PIN0);
			else if(my_pid.dir == CCW) MOT_PORT &= ~(1 << MOT_PIN1);
		}
	}
}
	