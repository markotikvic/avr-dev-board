/*
 * timer.c
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#include <avr/interrupt.h>
#include "gpio.h"
#include "timer.h"
#include "counter.h"
#include "uart.h"
#include "util.h"

#define PID_CONST ((uint8_t)10)

PWM_S myPwm;
PID_S myPid;
volatile uint32_t timerNode0 = 0;
volatile uint32_t timerNode1 = 0;
volatile uint32_t timerNode2 = 0;
volatile uint32_t pauseTime = 0;
uint8_t pwmFlag = PWM_OFF;
uint8_t pidFlag = 0;
uint8_t fsmStep = 0;

void initTimer1(uint8_t prescaler, uint16_t topVal)
{
	uint16_t temp;

	temp = 0;
	temp |= (1 << WGM10 | 1 << WGM11); /* Fast PWM mode - TOP is OCR1A, auto restart. */
	TCCR1A = temp;

	temp = 0;
	temp |= (1 << WGM12 | 1 << WGM13); /* Fast PWM mode - TOP is OCR1A, auto restart. */
	temp |= (prescaler << CS10); /* CS2:0 = 010 - Prescaler set to 8. */
	TCCR1B = temp;

	temp = 0;
	temp |= (1 << TOIE0);
	TIMSK1 = temp;

	OCR1A = topVal;

	sei();
}

void enablePwm(PWM_S *pwm)
{
	myPwm.targetPort = pwm->targetPort;
	myPwm.pinNo = pwm->pinNo;
	myPwm.period = (volatile uint32_t) pwm->period;
	myPwm.topVal = (volatile uint32_t) pwm->topVal;
	myPwm.pwmTick = 0;
	pwmFlag = PWM_ON;
}

void pwmReff(uint32_t reff)
{
	if(reff < myPwm.period ) {
		myPwm.topVal = reff;
	} else {
		myPwm.topVal = myPwm.period;
	}
}

void stopPwm()
{
	pwmFlag = PWM_OFF;
}

void pidSetup(float kp, float ki, float kd, uint16_t samplePeriod)
{
	myPid.kp = kp;
	myPid.ki = ki;
	myPid.kd = kd;
	myPid.samplePeriod = samplePeriod;
	myPid.samplePeriodOpt = 1 / samplePeriod;
	myPid.pwmPeriod = 200; 	//20ms for motors

	enableCounter();
	initTimer1(PRESCALER8, 100);	/* 0.1 ms */
	enableHbridge();
}

void pidReff(uint16_t refSpeed)
{
	myPid.refSpeed = refSpeed;
	pidFlag = 1;
}

void pidMotDir(uint8_t motDir)
{
	myPid.motDir = motDir;
}

void pid()
{
	long count = getCount();
	int temp = 0;
	/* Speed difference. */
	myPid.err = myPid.refSpeed - (uint16_t)(count - myPid.lastCount);

	/* Error sum. */
	myPid.errSum += myPid.err;
	if(myPid.errSum > 200) {
		myPid.errSum = 200;
	}

	/* Error difference. */
	myPid.errDiff = myPid.err - myPid.lastErr;

	/* PID output */
	temp = (myPid.kp*myPid.err + myPid.ki*myPid.errSum + myPid.kd*myPid.errDiff)/PID_CONST;
	if(temp > myPid.pwmPeriod) {
		myPid.pwmTopVal = myPid.pwmPeriod;
	} else {
		myPid.pwmTopVal = temp;
	}

	/* Memorize last values. */
	myPid.lastCount = count;
	myPid.lastErr = myPid.err;
}


void stopPid()
{
	pidFlag = PID_OFF;
}

void setTimer(uint8_t timerNum, uint32_t time)
{
	switch(timerNum) {
	case 0:
		timerNode0 = time;
		break;
	case 1:
		timerNode1 = time;
		break;
	case 2:
		timerNode2 = time;
		break;
	}
}

int getTimer(uint8_t timerNum)
{
	switch(timerNum) {
	case 0:
		return timerNode0;
	case 1:
		return timerNode1;
	case 2:
		return timerNode2;
	default: return 1;
	}
}

void pauseLoop(uint16_t pause)
{
	pauseTime = pause;
	while(pauseTime);
}

void threadsLoop()
{
	switch(fsmStep){
	case 0:
		if(!getTimer(0)) {
			togglePin(&PORTD, GPIO_PIN6);
			setTimer(0, 250);
			pauseLoop(1);
		}
		fsmStep++;
		break;
	case 1:
		if(!getTimer(1)) {
			togglePin(&PORTB, GPIO_PIN0);
			setTimer(1, 500);
			pauseLoop(1);
		}
		fsmStep++;
		break;
	case 2:
		if(!getTimer(2)) {
			togglePin(&PORTD, GPIO_PIN7);
			setTimer(2, 1000);
			pauseLoop(1);
		}
		fsmStep = 0;
		break;
	}
}

ISR(TIMER1_OVF_vect)
{
	if(pwmFlag == PWM_ON) {
		myPwm.pwmTick++;
		if(myPwm.pwmTick >= myPwm.period && myPwm.topVal > 0) {
			writePin(myPwm.targetPort, myPwm.pinNo, HIGH);
			myPwm.pwmTick = 0;
		} else if(myPwm.pwmTick >= myPwm.topVal) {
			writePin(myPwm.targetPort, myPwm.pinNo, LOW);
		}
	}

	if(pidFlag == 1) {
		myPid.pidTickSample++;
		if(myPid.pidTickSample >= myPid.samplePeriod) {
			myPid.pidTickSample = 0;
			pid();
		}

		myPid.pidTickPwm++;
		if(myPid.pidTickPwm >= myPid.pwmPeriod && myPid.pwmTopVal > 0) {
			myPid.pidTickPwm = 0;

			if(myPid.motDir == CW) {
				MOT_PORT |= (1 << MOT_PIN0);
			} else if(myPid.motDir == CCW) {
				MOT_PORT |= (1 << MOT_PIN1);
			}
		} else if (myPid.pidTickPwm >= myPid.pwmTopVal) {
			if(myPid.motDir == CW) {
				MOT_PORT &= ~(1 << MOT_PIN0);
			} else if(myPid.motDir == CCW) {
				MOT_PORT &= ~(1 << MOT_PIN1);
			}
		}
	}


	if(timerNode0 > 0) timerNode0--;
	if(timerNode1 > 0) timerNode1--;
	if(timerNode2 > 0) timerNode2--;
	if(pauseTime > 0) pauseTime--;
}
