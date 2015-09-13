/*
 * motor.h
 *
 * Created: 10-Sep-15 21:58:37
 *  Author: marko
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "gpio.h"

#define MOT_DDR		DDRB
#define MOT_PORT	PORTB
#define MOT_EN		GPIO_PIN0
#define MOT_PIN0	GPIO_PIN1
#define MOT_PIN1 	GPIO_PIN2

void enable_h_bridge(void);
void start_motor_CW(void);
void start_motor_CCW(void);
void stop_motor(void);
void halt_motor(void);
void disable_h_bridge(void);



#endif /* MOTOR_H_ */