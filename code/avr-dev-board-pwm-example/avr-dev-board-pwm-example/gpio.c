/*
 * gpio.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 *
 *  Ova biblioteka je namenjenaa za rad sa digitalnim ulazima i izlazima ATmega328P
 *  mikrokontrolera kompanije Atmel. Bilbioteka pruza sledeci skup mogucnosti korisniku:
 *  (Napomena: sve makro definicije biblioteke se nalaze u datoteci gpio.h)
 *
 *  1. Postavljanje pina kontrolera u rezimu rada ulaza ili izlaza
 *      - Upotreba:
 *
 *        set_pin_dir(DDRx, GPIO_PINx, DIRECTION); gde je:
 *
 *            a) DDRx - port na kojem se nalazi pin
 *               (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da podesi (x = 1...7) 
 *            c) DIRECTION - ovaj parametar treba postaviti na INPUT ukoliko 
 *               pin treba da radi u rezimu digitalnog ulaza, odnosno na OUTPUT
 *               ukoliko pin treba da radi u rezimu digitalnog izlaza.
 *
 *        - Funckija nema povratnu vrednost
 *
 *  2. Postavljanje naponskog nivoa pina kontrolera na visok logicki nivo
 *      - Upotreba:
 *
 *        set_pin(PORTx, GPIO_PINx); gde je:
 *
 *            a) PORTx - port na kojem se nalazi pin (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da podesi (x = 1...7)
 *
 *        - Funckija nema povratnu vrednost
 *
 *  Napomena: da bi ova funkcija ispravno radila ciljani pin kontrolera mora biti
 *  postavljen u izlaznom rezimu rada.
 *
 *  3. Postavljanje naponskog nivoa pina kontrolera na nizak logicki nivo
 *      - Upotreba:
 *
 *        clear_pin(PORTx, GPIO_PINx); gde je:
 *
 *            a) PORTx - port na kojem se nalazi pin (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da podesi (x = 1...7)
 *
 *        - Funckija nema povratnu vrednost
 *
 *  Napomena: da bi ova funkcija ispravno radila ciljani pin kontrolera mora biti
 *  postavljen u izlaznom rezimu rada.
 *
 *  4. Postavljanje naponskog nivoa pina kontrolera na visok ili nizak logicki nivo
 *      - Upotreba:
 *
 *        write_pin(PORTx, GPIO_PINx, VALUE); gde je:
 *
 *            a) PORTx - port na kojem se nalazi pin (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da podesi (x = 1...7)
 *            c) VALUE - u zavisnosti od toga da li je ovaj parametar LOW ili HIGH
 *               pin ce biti postavljen na nizak ili visok logicki nivo.
 *
 *        - Funckija nema povratnu vrednost
 * 
 *  Napomena: da bi ova funkcija ispravno radila ciljani pin kontrolera mora biti
 *  postavljen u izlaznom rezimu rada.
 *
 *  5. Invertovanje naponskog nivoa pina kontrolera
 *      - Upotreba:
 *
 *        toggle_pin(PORTx, GPIO_PINx); gde je:
 *
 *            a) PORTx - port na kojem se nalazi pin (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da podesi (x = 1...7)
 *
 *        - Funckija nema povratnu vrednost
 *
 *  Napomena: da bi ova funkcija ispravno radila ciljani pin kontrolera mora biti
 *  postavljen u izlaznom rezimu rada.
 *
 *  6. Ocitavanje stanja pina kontrolera
 *      - Upotreba:
 *
 *        read_pin(PORTx, GPIO_PINx); gde je:
 *
 *            a) PORTx - port na kojem se nalazi pin (x = B/C/D)
 *            b) GPIO_PINx - pin koji korisnik zeli da procita (x = 1...7)
 *
 *        - Ukoliko je stanje pina logicka "1" funkcija vraca 1
 *        - Ukoliko je stanje pina logicka "0" funkcija vraca 0.
 *  Napomena: da bi ova funkcija ispravno radila ciljani pin kontrolera mora biti
 *  postavljen u ulaznom rezimu rada.
 *
 */

#include "gpio.h"

void set_pin_dir(volatile uint8_t *target_port, uint8_t pin_no, uint8_t dir)
{
	switch(dir){
	case OUTPUT:
		(*target_port) |= (1 << pin_no);
		break;
	case INPUT:
		(*target_port) &= ~(1 << pin_no);
		break;
	default: break;
	}
}

void set_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	(*target_port) |= (1 << pin_no);
}

void clear_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	(*target_port) &= ~(1 << pin_no);
}

void write_pin(volatile uint8_t *target_port, uint8_t pin_no, uint8_t pin_val)
{
	switch(pin_val){
	case HIGH:
		(*target_port) |= (1 << pin_no);
		break;
	case LOW:
		(*target_port) &= ~(1 << pin_no);
		break;
	default: break;
	}
}

int read_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	uint8_t tmpPort = (*target_port) & (1 << pin_no);

	if(tmpPort != 0) return 1;
	else return 0;
}

void toggle_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	uint8_t tmpPort = (*(target_port)) & (1 << pin_no);

	if(tmpPort != 0) (*target_port) &= ~(1 << pin_no);
	else (*target_port) |= (1 << pin_no);
}

