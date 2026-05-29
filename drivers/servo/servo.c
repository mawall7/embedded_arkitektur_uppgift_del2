#include "pwm.h"
#include <avr/io.h>
#include "pins.h"
#include <stdint.h>


void servo_init(){
    DDRB |= (1 << SERVO_PIN);
    timer1_init();
    int result = set_pulsetime(2000);
}
    
  
void servo_open(){
 set_pulsetime((uint16_t)4000);
}

void servo_close(){
  set_pulsetime((uint16_t)3000);
}




