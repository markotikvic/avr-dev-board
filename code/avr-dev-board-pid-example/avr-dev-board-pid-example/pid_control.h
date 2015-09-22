/*
 * pid_control.h
 *
 * Created: 12-Sep-15 21:29:33
 *  Author: marko
 */ 

#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_

#include <avr/io.h>

#define PID_OFF ((uint8_t)0)
#define PID_ON ((uint8_t)1)

#define CW ((uint8_t)0)
#define CCW ((uint8_t)1)

typedef struct {
	uint8_t mot_dir;
	float kp;
	float ki;
	float kd;
	long ref_value;

	long sample_period;
	float sample_peiod_opt;
	long pid_tick_sample;

	long pwm_top_val;
	long pwm_period;
	long pid_pwm_tick;

	long err;
	long last_err;
	long err_diff;
	long err_sum;
	long last_count;
} PID_S;

void pid_setup_params(float kp, float ki, float kd);
void pid_ref_val(long ref_val);
void stop_pid_control(void);
void start_pid_control(void);
void pid_mot_direction(uint8_t mot_dir);

#endif /* PID_CONTROL_H_ */