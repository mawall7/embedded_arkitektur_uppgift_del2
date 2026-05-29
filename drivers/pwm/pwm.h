#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init();
void pwm_start();
void pwm_stop();

void timer1_init();
int set_pulsetime(uint16_t OCR1_val);

#endif