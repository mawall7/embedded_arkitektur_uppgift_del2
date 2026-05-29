#include "led.h"
#include <avr/io.h>
#include "74hc595.h"

void led_init(){
 shift_out(0b00000000); 
}

void green_led_on(){
 shiftregister_setbit(6);
 
}
void green_led_off(){
  shiftregister_clearbit(6);
};

void red_led_on(){
    shiftregister_setbit(5);
};

void red_led_off(){
    shiftregister_clearbit(5);
};

void blue_led_on(){
    shiftregister_setbit(7);
};

void blue_led_off(){
    shiftregister_clearbit(7);
};