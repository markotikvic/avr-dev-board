#ifndef TIMER_H_
#define TIMER_H

#include <avr/io.h>

#define PRESCALER1	((uint8_t)0b00000001)
#define PRESCALER8	((uint8_t)0b00000010)
#define PRESCALER64	((uint8_t)0b00000011)
#define PWM_OFF		((uint8_t)0)
#define PWM_ON		((uint8_t)1)
#define PID_OFF		((uint8_t)0)
#define PID_ON		((uint8_t)1)
#define CW		((uint8_t)0)
#define CCW		((uint8_t)1)

typedef struct pwm_structure{
	volatile uint8_t *target_port;
	uint8_t pin;
	uint32_t period;
	uint32_t top_val;
	uint32_t pwm_tick;
} PWM_S;

typedef struct pid_structure{
	uint8_t dir;
	float kp;
	float ki;
	float kd;
	uint16_t ref_speed;

	uint16_t sample_period;
	float sample_peiod_opt;
	volatile uint16_t pid_tick_sample;

	uint16_t pwm_top_val;
	uint16_t pwm_period;
	volatile uint16_t pid_pwm_tick;

	long err;
	long last_err;
	long err_diff;
	long err_sum;
	long last_count;
} PID_S;

void init_timer_1(uint8_t prescaler, uint16_t top_val);
void set_timer(uint8_t timer_num, uint32_t time);
int get_timer(uint8_t timer_num);
void pause_loop(uint16_t pause);
void init_pwm(PWM_S *pwm);
void stop_pwm();
void start_pwm();
void pwm_ref_val(uint32_t reff);
void therads_loop_example();
void pid_setup_params(float kp, float ki, float kd, uint16_t sample_period);
void pid_ref_val(uint16_t ref_speed);
void stop_pid_control();
void pid_mot_direction(uint8_t mot_dir);

#endif /* TIMER_H_ */
