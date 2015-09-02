/*
 * gpio.h
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#ifndef _GPIO_H_
#define _GPIO_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define INPUT ((uint8_t)0)
#define OUTPUT ((uint8_t)1)

#define HIGH ((uint8_t)1)
#define LOW ((uint8_t)0)

#define MOT_DDR		DDRB
#define MOT_PORT	PORTB
#define MOT_EN		GPIO_PIN0
#define MOT_PIN0	GPIO_PIN1
#define MOT_PIN1 	GPIO_PIN2

#define GPIO_PIN0 ((uint8_t)0)
#define GPIO_PIN1 ((uint8_t)1)
#define GPIO_PIN2 ((uint8_t)2)
#define GPIO_PIN3 ((uint8_t)3)
#define GPIO_PIN4 ((uint8_t)4)
#define GPIO_PIN5 ((uint8_t)5)
#define GPIO_PIN6 ((uint8_t)6)
#define GPIO_PIN7 ((uint8_t)7)

void set_pin_dir(volatile uint8_t *target_port, uint8_t pin_no, uint8_t dir);
void set_pin(volatile uint8_t *target_port, uint8_t pin_no);
void clear_pin(volatile uint8_t *target_port, uint8_t pin_no);
void write_pin(volatile uint8_t *target_port, uint8_t pin_no, uint8_t pin_val);
int read_pin(volatile uint8_t *target_port, uint8_t pin_no);
void toggle_pin(volatile uint8_t *target_port, uint8_t pin_no);
void enable_h_bridge();
void start_motor_CW();
void start_motor_CCW();
void stop_motor();
void halt_motor();
void disable_h_bridge();

#endif /* _GPIO_H_ */
