#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

// #define LED_DDR   DDRB
// #define LED_PORT  PORTB
// #define LED_PIN   PB5



// Shift register
#define SR_DDR     DDRB
#define SR_PORT    PORTB

#define SR_DATA_DDR DDRB
#define SR_DATA_PORT PORTB
#define SR_DS     PB3

#define SR_CL_DDR  DDRB
#define SR_CL_PORT PORTB
#define SR_CL      PB5

#define SR_LATCH_DDR  DDRD
#define SR_LATCH_PORT PORTD
#define SR_LATCH      PD2

#define BZR PD3

#define SERVO_PIN PB1


#endif