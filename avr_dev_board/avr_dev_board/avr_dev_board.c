/*
 * main.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */


#define F_CPU 8000000UL

#include <avr/io.h>
#include "gpio.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "counter.h"
#include "util.h"
#include <string.h>

int main()
{
	initUart(UART_9600);
	enableHbridge();
	initTimer1(PRESCALER8, 1000);
//	pidSetup(1, 0, 1, 1000);	/* pid() loops every 10ms */
//	pidMotDir(CCW);
//	pidReff(0);

	while (1) {
		startMotorCCW();
		pauseLoop(4000);
		stopMotor();
		pauseLoop(4000);
		startMotorCW();
		pauseLoop(4000);
		stopMotor();
		pauseLoop(4000);
	}
	return 0;
}
