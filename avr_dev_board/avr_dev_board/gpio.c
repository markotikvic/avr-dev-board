/*
 * gpio.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#include "gpio.h"

void setPinDir(volatile uint8_t *targetPort, uint8_t pinNo, uint8_t dir)
{
	switch(dir){
	case OUTPUT:
		(*targetPort) |= (1 << pinNo);
	break;
	case INPUT:
		(*targetPort) &= ~(1 << pinNo);
	break;
	default: break;
	}
}

void writePin(volatile uint8_t *targetPort, uint8_t pinNo, uint8_t pinVal)
{
	switch(pinVal){
	case HIGH:
		(*targetPort) |= (1 << pinNo);
	break;
	case LOW:
		(*targetPort) &= ~(1 << pinNo);
	break;
	default: break;
	}
}

int readPin(volatile uint8_t *targetPort, uint8_t pinNo)
{
	uint8_t tmpPort = (*targetPort) & (1 << pinNo);

	if(tmpPort != 0) {
		return 1;
	} else {
		return 0;
	}
}

void togglePin(volatile uint8_t *targetPort, uint8_t pinNo)
{
	uint8_t tmpPort = (*(targetPort)) & (1 << pinNo);

	if(tmpPort != 0) {
		(*targetPort) &= ~(1 << pinNo);
	} else {
		(*targetPort) |= (1 << pinNo);
	}
}

void enableHbridge()
{
	setPinDir(&MOT_DDR, MOT_EN, OUTPUT);
	setPinDir(&MOT_DDR, MOT_PIN0, OUTPUT);
	setPinDir(&MOT_DDR, MOT_PIN1, OUTPUT);

	writePin(&MOT_PORT, MOT_EN, HIGH);
	writePin(&MOT_PORT, MOT_PIN0, LOW);
	writePin(&MOT_PORT, MOT_PIN1, LOW);
}

void disableHbridge()
{
	setPinDir(&MOT_DDR, MOT_EN, OUTPUT);
	setPinDir(&MOT_DDR, MOT_PIN0, OUTPUT);
	setPinDir(&MOT_DDR, MOT_PIN1, OUTPUT);

	writePin(&MOT_PORT, MOT_EN, LOW);
	writePin(&MOT_PORT, MOT_PIN0, LOW);
	writePin(&MOT_PORT, MOT_PIN1, LOW);
}

void startMotorCW()
{
	writePin(&MOT_PORT, MOT_PIN0, LOW);
	writePin(&MOT_PORT, MOT_PIN1, HIGH);
}

void startMotorCCW()
{
	writePin(&MOT_PORT, MOT_PIN0, HIGH);
	writePin(&MOT_PORT, MOT_PIN1, LOW);
}

void stopMotor()
{
	writePin(&MOT_PORT, MOT_PIN0, LOW);
	writePin(&MOT_PORT, MOT_PIN1, LOW);
}

void haltMotor()
{
	writePin(&MOT_PORT, MOT_PIN0, HIGH);
	writePin(&MOT_PORT, MOT_PIN1, HIGH);
}
