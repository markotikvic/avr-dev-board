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
 *  1. Zadavanje koeficijenata PID regulatora (ova funkcija istovremeno vodi racuna
 *		i o podesavanju H-mosta, neophodnog za rad motora, i podesavanju kontrole\
 *		spoljasnjih prekida za rad sad enkoderom)
 *		- Upotreba:
 *			
 *			pid_setup_params(Kp, Ki, Kd); gde je:
 *			
 *				a) Kp - pojacanje propocionalnog dejstva regulatora
 *				b) Ki - pojacanje integralnog dejstva regulatora
 *				c) Kd - pojacanje diferencijalnog dejstva regulatora
 *			
 *			- Funkcija nema povratnu vrednost
 *	
 *	2. Pokretanje i zaustavljanje regulacije
 *		- Upotreba:
 *			
 *			start_pid_control(); pokrece regulaciju
 *			
 *			- Funkcija nema povratnu vrednost
 *			
 *			stop_pid_control(); zaustavlja regulaciju
 *			
 *			- Funkcija nema povratnu vrednost
 *			
 *	3. Zadavanje referente vrednosti regulacije
 *		- Upotreba:
 *		
 *			pid_ref_val(VALUE); gde je:
 *				
 *				a) VALUE - referentna vrednost regulacije
 *				
 *			- Funkcija nema povratnu vrednost
 *			
 *	Napomena: vrsta regulacije realizovana u primeru je poziciona. Ukoliko
 *	regulacija treba da se vrsi na nivou prvog izvoda pozicije potrebno je prepraviti
 *	vrstu greske u pid() funkciji.
 *	
 */ 

#include "pid_control.h"
#include "motor.h"
#include "counter.h"
#include "gpio.h"

#define PID_CONST ((uint8_t)10)

static void pid();
static volatile PID_S _pid;
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
	_pid.kp = kp;
	_pid.ki = ki;
	_pid.kd = kd;
	_pid.sample_period = 500;	//50ms for pid loop
	_pid.sample_peiod_opt = 1 / 500;
	_pid.pwm_period = 200;	//20ms period for motors
	
	_pid.last_count = 0;
	_pid.last_err = 0;
	_pid.ref_value = 0;
	_pid.pid_pwm_tick = 0;
	_pid.pid_tick_sample = 0;
	_pid.mot_dir = CW;

	enable_counter();
	enable_h_bridge();
	init_timer_0();
}

void pid_ref_val(long ref_val)
{
	_pid.ref_value = ref_val;
}

void pid_mot_direction(uint8_t mot_dir)
{
	_pid.mot_dir = mot_dir;
}

//Gets called in timer 0 interrupt routine.
static void pid()
{
	long count = get_count();
	long pid_val = 0;
	_pid.err = _pid.ref_value - count;	/* Position difference. */

	_pid.err_sum += _pid.err;					/* Error sum. */
	
	if(_pid.err_sum > 100)
		_pid.err_sum = 100;	/* Anti wind-up */

	_pid.err_diff = (long)((_pid.err - _pid.last_err)*_pid.sample_peiod_opt);	/* 1st order error differential. */

	/* PID output */
	pid_val = _pid.kp*_pid.err + _pid.ki*_pid.err_sum + _pid.kd*_pid.err_diff;
	
	if(pid_val > 0) {
		_pid.mot_dir = CW;
		if(pid_val > _pid.pwm_period)
			_pid.pwm_top_val = _pid.pwm_period;
		else
			_pid.pwm_top_val = (uint16_t)(pid_val);
	} else {
		_pid.mot_dir = CCW;
		if(pid_val < -_pid.pwm_period)
			_pid.pwm_top_val = _pid.pwm_period;
		else
			_pid.pwm_top_val = (uint16_t)(-pid_val);
	}
		

	/* Memorize last values. */
	_pid.last_count = count;
	_pid.last_err = _pid.err;
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
		_pid.pid_tick_sample++;
		if(_pid.pid_tick_sample >= _pid.sample_period) {
			_pid.pid_tick_sample = 0;
			pid();
		}

		_pid.pid_pwm_tick++;
		if(_pid.pid_pwm_tick >= _pid.pwm_period && _pid.pwm_top_val > 0) {
			_pid.pid_pwm_tick = 0;
			if(_pid.mot_dir == CW)
				MOT_PORT |= (1 << MOT_PIN0);
			else 
				MOT_PORT |= (1 << MOT_PIN1);	
		} else if (_pid.pid_pwm_tick >= _pid.pwm_top_val) {
			MOT_PORT &= ~(1 << MOT_PIN0);
			MOT_PORT &= ~(1 << MOT_PIN1);
		}
	}
}
	