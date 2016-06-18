#include "../include/gpio.h"

// set_pin_dir configures the direction of I/O pin.
// It takes 3 parameters: PORT ADDRESS, PIN NUMBER, DIRECTION
void set_pin_dir(volatile uint8_t *target_port, uint8_t pin_no, uint8_t dir)
{
	switch (dir){
	case OUTPUT:
		(*target_port) |= 1<<pin_no;
		break;
	case INPUT:
		(*target_port) &= ~(1<<pin_no);
		break;
	default:
		break;
	}
}

// set_pin pulls the I/O pin high.
// It takes 2 parameters: PORT ADDRESS, PIN NUMBER
void set_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	(*target_port) |= 1<<pin_no;
}

// clear_pin pulls the I/O pin low.
// It takes 2 parameters: PORT ADDRESS, PIN NUMBER
void clear_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	(*target_port) &= ~(1<<pin_no);
}

// write_pin pulls the I/O pin high or low.
// It takes 3 parameters: PORT ADDRESS, PIN NUMBER, PIN VALUE.
void write_pin(volatile uint8_t *target_port, uint8_t pin_no, uint8_t pin_val)
{
	switch (pin_val){
	case HIGH:
		(*target_port) |= 1<<pin_no;
		break;
	case LOW:
		(*target_port) &= ~(1<<pin_no);
		break;
	default:
		break;
	}
}

// read_pin returns the current state od an I/O pin (1 - high, 0 - low).
// It takes 2 parameters: PORT ADDRESS, PIN NUMBER
int read_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	uint8_t tmpPort = (*target_port) & (1<<pin_no);
	if (tmpPort != 0) {
		return 1;
	} else {
		return 0;
	}
}

// toggle_pin inverts the current state of an I/O pin.
// It takes 2 parameters: PORT ADDRESS, PIN NUMBER
void toggle_pin(volatile uint8_t *target_port, uint8_t pin_no)
{
	uint8_t tmpPort = (*(target_port)) & (1<<pin_no);
	if (tmpPort != 0) {
		(*target_port) &= ~(1<<pin_no);
	} else {
		(*target_port) |= (1<<pin_no);
	}
}

// enagle_h_bridge enables the H bridge on the board.
void enable_h_bridge()
{
	set_pin_dir(&MOT_DDR, MOT_EN, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN0, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN1, OUTPUT);
	write_pin(&MOT_PORT, MOT_EN, HIGH);
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

// disable_h_bridge disables the H bridge on the board.
void disable_h_bridge()
{
	set_pin_dir(&MOT_DDR, MOT_EN, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN0, OUTPUT);
	set_pin_dir(&MOT_DDR, MOT_PIN1, OUTPUT);
	write_pin(&MOT_PORT, MOT_EN, LOW);
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

// start_motor_CW makes H bridge pass current in direction 1.
void start_motor_CW()
{
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, HIGH);
}

// start_motor_CCW makes H bridge pass current in direction 2.
void start_motor_CCW()
{
	write_pin(&MOT_PORT, MOT_PIN0, HIGH);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

// stop_motor stops H bridge from conducting in any direction.
void stop_motor()
{
	write_pin(&MOT_PORT, MOT_PIN0, LOW);
	write_pin(&MOT_PORT, MOT_PIN1, LOW);
}

// halt_motor makes H bridge conduct in both directions.
void halt_motor()
{
	write_pin(&MOT_PORT, MOT_PIN0, HIGH);
	write_pin(&MOT_PORT, MOT_PIN1, HIGH);
}
