#include <avr/io.h>
#include "pins.h"
#include <stdint.h>

#define T1MAX_TOP 39999

void pwm_init(){  //egentligen CTC fyrkantsvåg

   
    DDRD |= (1 << BZR);

    
    TCCR2A = (1 << WGM21); // CTC mode
    TCCR2B = 0;
    int freq = 1000;
    OCR2A = (16000000 / (64 * freq)) - 1;


}

void pwm_start(){
   
    
    TCCR2A |= (1 << COM2B0); 
    TCCR2B = (1 << CS22);    // prescaler 64
   
}
    
void pwm_stop(){
    
    // koppla bort PWM från pin
     TCCR2A &= ~(1 << COM2B0);
   
     TCCR2B = 0;
}

//servo Fast pwm
void timer1_init() {

    //DDRB |= (1 << PB1);
   
    TCCR1A = 0;
    TCCR1B = 0;
    
    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13);

   
     //aktivera timer 1 via intern koppling till PB9
    TCCR1A |= (1 << COM1A1);

    //prescaler 8
    TCCR1B |= (1 << CS11);

    ICR1 = T1MAX_TOP;   // 20 ms exakt (50 Hz)

    // OCR1A = 3000;   // start 90° 
}


int set_pulsetime(uint16_t OCR1_val){

  if(OCR1_val <= T1MAX_TOP ){
     OCR1A = OCR1_val;
     return 1;
  }
  else{
    OCR1A = T1MAX_TOP;
  }
  return -1;
  
}
    





