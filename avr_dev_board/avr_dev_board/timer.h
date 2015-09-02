/*
 * timer.h
 *
 *  Created on: Jun 17, 2015
 *      Author: marko
 */

#ifndef HEADERS_TIMER_H_
#define HEADERS_TIMER_H

#include <avr/io.h>

#define PRESCALER1 ((uint8_t)0b00000001)
#define PRESCALER8 ((uint8_t)0b00000010)
#define PRESCALER64 ((uint8_t)0b00000011)

#define PWM_OFF ((uint8_t)0)
#define PWM_ON ((uint8_t)1)

#define PID_OFF ((uint8_t)0)
#define PID_ON ((uint8_t)1)

#define CW ((uint8_t)0)
#define CCW ((uint8_t)1)

typedef struct {
	volatile uint8_t *targetPort;
	uint8_t pinNo;
	uint32_t period;
	uint32_t topVal;
	uint32_t pwmTick;
} PWM_S;

typedef struct {
	uint8_t motDir;
	float kp;
	float ki;
	float kd;
	uint16_t refSpeed;

	uint16_t samplePeriod;
	float samplePeriodOpt;
	volatile uint16_t pidTickSample;

	uint16_t pwmTopVal;
	uint16_t pwmPeriod;
	volatile uint16_t pidTickPwm;

	long err;
	long lastErr;
	long errDiff;
	long errSum;
	long lastCount;
} PID_S;

void initTimer1(uint8_t prescaler, uint16_t topVal);
void setTimer(uint8_t timerNum, uint32_t time);
int getTimer(uint8_t timerNum);
void pauseLoop(uint16_t pause);

void enablePwm(PWM_S *pwm);
void stopPwm();

void threadsLoop();

void pidSetup(float kp, float ki, float kd, uint16_t samplePeriod);
void pidReff(uint16_t refSpeed);
//void pid();	/* Gets called in timer 1 interrupt routine. */
void stopPid();
void pidMotDir(uint8_t motDir);

#endif /* HEADERS_TIMER_H_ */
