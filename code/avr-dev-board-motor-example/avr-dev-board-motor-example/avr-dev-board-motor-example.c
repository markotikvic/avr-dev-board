/*
 * avr_dev_board_motor_example.c
 *
 * Created: 10-Sep-15 22:34:45
 *  Author: marko
 *
 * Primer upotrebe motor.h biblioteke:
 *    Naizmenicno pokretanje motora u CW i CCW smeru.
 *
 * Napomena: u ovom primeru je upotrebljena i biblioteka timer.h ciji se detaljan opis
 * nalazi u avr-dev-board-timer-example projektu.
 */ 
#define F_CPU 8000000UL

#include <avr/io.h>
#include "motor.h"
#include "timer.h"

int main()
{
	enable_h_bridge();					//Pali H-most
	init_timer_1(PRESCALER8, 1000);		//Inicijalizuj tajmer 1 sa interrupt periodom od 1ms

	while(1){
		start_motor_CCW();				//Pokreni motor u CCW smeru
		pause_loop(4000);				//Pauziraj 4 sekunde
		
		stop_motor();					//Zaustavi motor
		pause_loop(2000);				//Pauziraj 2 sekunde
		
		start_motor_CW();				//Pokreni motor u CW smeru
		pause_loop(4000);				//Pauziraj 4 sekunde
		
		stop_motor();					//Zaustavi motor
		pause_loop(2000);				//Pauziraj 2 sekunde
	}
	return 0;
}