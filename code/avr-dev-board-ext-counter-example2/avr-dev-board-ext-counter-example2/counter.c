/*
 * counter.c
 *
 *  Created on: Jun 29, 2015
 *      Author: marko
 *  
 *  Ova biblioteka je namenja za rad sa optickim rotacionim
 *  enkoderom ISC3806. Bilbioteka pruza sledeci skup mogucnosti korisniku:
 *  (Napomena: sve makro definicije biblioteke se nalaze u datoteci counter.h)
 *  
 *  1. Omogucavanje spoljasnjih perkida na kontroleru na predefinisanim pinovima
 *     za rad sa ISC3806 enkoderom:
 *		- Upotreba:
 *			
 *			enable_counter();
 *			
 *			- Funkcija nema povratnu vrednost
 *			
 *	2. Pribavljanje informacije o sumi koraka koje je enkoder napravio
 *		- Upotreba:
 *			
 *			get_count();
 *			
 *			- Funkcija vraca sumu (long) koraka koje je enkoder napravio.
 *			  Suma se uvecava za CW smer, a umanjuje za CCW smer.
 *	
 *	Napomena: da bi ova funkcija ispravno radila, pre njene upotrebe u kodu mora postojati
 *	poziv f-je enable_counter.
 *	
 */


#include "counter.h"
#include "gpio.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define ENCODER_FULL_CIRCLE ((uint16_t)2048)

static volatile long counts = 0;
static volatile uint8_t dir_change_caught = 0;

void enable_counter()
{
	set_pin_dir(&EXT_DDR, EXT_CH_A, INPUT);
	set_pin_dir(&EXT_DDR, EXT_CH_B, INPUT);
	EICRA |= (1 << ISC11);	/* Falling edge on INT1. */
	EIMSK |= (1 << INT1);
	sei();
}

long get_count()
{
	return counts;
}

ISR(INT1_vect)
{
	if( EXT_PORT & (1 << EXT_CH_A) ){
		counts--;
		if(counts < 0)
			counts = ENCODER_FULL_CIRCLE - 1;
	} else {
		counts++;
		if(counts > ENCODER_FULL_CIRCLE - 1)
			counts = 0;
	}							
}
