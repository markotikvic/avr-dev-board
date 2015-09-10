/*
 * motor.c
 *
 * Created: 10-Sep-15 21:58:21
 *  Author: marko
 *
 * Ova biblioteka je namenjena za rad sa integrisanim H-mostom na
 * razvojnom okruzenju. Bilbioteka pruza sledeci skup mogucnosti korisniku:
 * (Napomena: sve makro definicije biblioteke se nalaze u datoteci motor.h)
 *
 * 1. Postavljanje H-mosta u aktivni rezim rada (H-most ON)
 *    - Upotreba:
 *            
 *        enable_h_bridge();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 *
 * 2. Postavljanje H-mosta u neaktivni rezim rada (H-most OFF)
 *    - Upotreba:
 *
 *        disable_h_bridge();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 *    
 * 3. Pokretanje motora u CW smeru (paljenje izlaza 1/gasenje izlaza 2 na kanalu A H-mosta)
 *    - Upotreba:
 *
 *        start_motor_CW();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 *
 * 4. Pokretanje motora u CW smeru (paljenje izlaza 2/gasenje izlaza 1 na kanalu A H-mosta)
 *    - Upotreba:
 *
 *        start_motor_CCW();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 *
 * 5. Gasenje motora (gasenje izlaza 1/gasenje izlaza 1 na kanalu A H-mosta)
 *    - Upotreba:
 *
 *        stop_motor();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 *
 * 6. Kocenje motora (paljenje izlaza 1/paljenje izlaza 1 na kanalu A H-mosta)
 *    - Upotreba:
 *
 *        halt_motor();
 *
 *    - Funkcija ne prima ulazne parametre i nema povratnu vrednost
 */ 
#include "motor.h"

void enable_h_bridge()
{
	set_pin_dir(&MOT_DDR, MOT_EN, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN0, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN1, OUTPUT);

	write_pin(&MOT_PORT, MOT_EN, HIGH);
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

void disable_h_bridge()
{
	set_pin_dir(&MOT_DDR, MOT_EN, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN0, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN1, OUTPUT);

	write_pin(&MOT_PORT, MOT_EN, LOW);
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

void start_motor_CW()
{
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, HIGH);
}

void start_motor_CCW()
{
	write_pin(&MOT_PORT, MOT_PIN0, HIGH);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

void stop_motor()
{
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

void halt_motor()
{
	write_pin(&MOT_PORT, MOT_PIN0, HIGH);
	write_pin(&MOT_PORT, MOT_PIN1, HIGH);
}