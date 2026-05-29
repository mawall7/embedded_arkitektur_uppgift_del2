#include <avr/io.h>
#include "buzzer.h"
#include "pins.h"
#include <util/delay.h>
#include "gpio.h"
#include "pwm.h"

#define TOP1KHZ 1999;


void buzzer_init(){
    
    //gpio_pin_output(&DDRD, BZR);

    
    PORTD &= ~(1 << BZR);
    DDRD |= (1 << BZR);
    
}

void buzzer_on(void)
{
    pwm_init();
    pwm_start();
}

void buzzer_off(void)
{
    pwm_stop();
    PORTD &= ~(1 << BZR);     // säker LOW
}

void access_granted_sound(void)
{
    // pip 1
    buzzer_on();
    _delay_ms(500);

    buzzer_off();
    _delay_ms(100);

}

void access_denied_sound(void)
{
    
    buzzer_on();
    _delay_ms(300);

    buzzer_off();
    _delay_ms(100);
    
    buzzer_off();

}



