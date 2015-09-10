/*
 * motor.c
 *
 * Created: 10-Sep-15 21:58:21
 *  Author: marko
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