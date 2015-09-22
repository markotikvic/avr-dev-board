/*
 * timer.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 *      
 *  Ova biblioteka je namenjena za rad sa 16-bitnim tajmerom 1 ATmega328P
 *  mikrokontrolera kompanije Atmel. Bilbioteka pruza sledeci skup mogucnosti korisniku:
 *  (Napomena: sve makro definicije biblioteke se nalaze u datoteci timer.h)
 *  
 *  1. Incijalizacija tajmera 1
 *	    - Upotreba:
 *	   
 *			init_timer_1(PRESCALER, TOP_VAL); gde je:
 *			
 *				a) PRESCALER - preskaler osnovne frekvencie kontrolera (F_CPU)
 *				b) TOP_VAL - broj broj ciklusa koji tajmer treba da odbroji pre
 *				   nego sto udje u prekidnu rutinu ISR(TIMER1_OVF_vect)
 *			
 *			- Funckija nema povratnu vrednost
 *      
 *  2. Prosledjivanje PWM signala na neki od pinova mikrokontrolera
 *      - Upotreba:
 *		
 *			init_pwm(PWM_INIT_STRUCT); gde je:
 *			
 *				a) PWM_INIT_STRUCT - struktura tipa PWM_S (definisano u timer.h) sa sledecim poljima
 *					a.1) port (volatile uint8_t *) - port na kojem se nalazi pin na koji zelimo da prosledimo signal.
 *							Moze imati sledece vrednosti: &PORTB, &PORTC, &PORTD.
 *					a.2) pin (uint8_t) - pin na koji zelimo da prosledimo signal.
 *							Moze imati sledece vrednosti: GPIO_PINx (x = 0...7)
 *					a.3) period (uint32_t) - zeljena perioda PWM signala (zadata u broju ciklusa tajmera 1)
 *					a.4) top_val (uint32_t) - inicijalni faktor ispune PWM signala (zadat u broju ciklusa tajmera 1)
 *					a.5) pwm_tick (uint32_t) - biblioteka ignorise vrednost ovog parametra
 *					
 *			- Funckija nema povratnu vrednost
 *      
 *  3. Promena faktora ispune PWM signala
 *		- Upotreba:
 *			
 *			pwm_ref_val(REFF); gde je:
 *				
 *				a) REFF - faktor ispune PWM signala (zadat u broju ciklusa tajmera 1)
 *				
 *			- Funckija nema povratnu vrednost
 *      
 *  4. Paljenje/gasenje PWM signala:
 *		- Upotreba:
 *			
 *			start_pwm(); pokrece prosledjivanje PWM signala na prethodno podesen pin kontroleras
 *			- Funckija nema povratnu vrednost
 *			
 *			stop_pwm(); zaustavlja prosledjivanje PWM signala
 *			- Funckija nema povratnu vrednost
 *
 *      
 *  5. Proveru stanja vremenskog brojaca    
 *		- Upotreba:
 *		
 *			set_timer(N); gde je:
 *			
 *				a) N - indeks brojaca koji zelimo da podesimo
 *			
 *			- Funckija nema povratnu vrednost
 *			
 *			get_timer(N); gde je:
 *			
 *				a) N - indeks brojaca koji zelimo da podesimo
 *				
 *			- Funckija vraca trenutnu vrednost brojaca (uint32_t)
 *			
 *	6. Unos kasnjenja u kod
 *		- Upotreba:
 *			
 *			pause_loop(TIME); gde je:
 *			
 *				a) TIME - zeljeno kasnjenje (zadato u broju ciklusa tajmera 1)
 */

#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "counter.h"
#include "uart.h"
#include "util.h"
#include "motor.h"

static volatile PWM_S my_pwm;
static volatile uint32_t timer_node_0 = 0;
static volatile uint32_t timer_node_1 = 0;
static volatile uint32_t timer_node_2 = 0;
static volatile uint32_t pause_time = 0;
static volatile uint8_t pwm_flag = PWM_OFF;
static volatile uint8_t fsm_step = 0;

void init_timer_1(uint8_t prescaler, uint16_t topw_val)
{
	uint16_t temp;

	temp = 0;
	temp |= (1 << WGM10 | 1 << WGM11);	/* Fast PWM mode - TOP is OCR1A, auto restart. */
	TCCR1A = temp;

	temp = 0;
	temp |= (1 << WGM12 | 1 << WGM13);	/* Fast PWM mode - TOP is OCR1A, auto restart. */
	temp |= (prescaler << CS10);		/* CS2:0 = 010 - Prescaler set to 8. */
	TCCR1B = temp;

	temp = 0;
	temp |= (1 << TOIE0);
	TIMSK1 = temp;

	OCR1A = topw_val;

	sei();
}

void init_pwm(PWM_S *pwm)
{
	my_pwm.port = pwm->port;
	my_pwm.pin = pwm->pin;
	my_pwm.period = (volatile uint32_t) pwm->period;
	my_pwm.top_val = (volatile uint32_t) pwm->top_val;
	my_pwm.pwm_tick = 0;
	pwm_flag = PWM_ON;
}

void pwm_ref_val(uint32_t reff)
{
	if(reff < my_pwm.period ) my_pwm.top_val = reff;
	else my_pwm.top_val = my_pwm.period;
}

void stop_pwm()
{
	pwm_flag = PWM_OFF;
}

void start_pwm()
{
	pwm_flag = PWM_ON;
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

void threads_loop_example()
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
			set_pin(my_pwm.port, my_pwm.pin);
			my_pwm.pwm_tick = 0;
		} else if(my_pwm.pwm_tick >= my_pwm.top_val) {
			clear_pin(my_pwm.port, my_pwm.pin);
		}
	}

	if(timer_node_0 > 0) timer_node_0--;
	if(timer_node_1 > 0) timer_node_1--;
	if(timer_node_2 > 0) timer_node_2--;
	if(pause_time > 0) pause_time--;
}
