/*
 * gpio.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
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

	if(tmpPort != 0) {
		return 1;
	} else {
		return 0;
	}
}

void toggle_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	uint8_t tmpPort = (*(target_port)) & (1 << pin_no);

	if(tmpPort != 0) {
		(*target_port) &= ~(1 << pin_no);
	} else {
		(*target_port) |= (1 << pin_no);
	}
}

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
