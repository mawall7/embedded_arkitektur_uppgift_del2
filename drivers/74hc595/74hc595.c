#include <avr/io.h>
#include <util/delay.h>
#include "74hc595.h"
#include "pins.h"

// #define DS   PB3
// #define SHCP PB5
// #define STCP PD2
uint8_t static bitstate;

//todo init och testa sedan enkel shift_out med LEDS 
void shift_init(){
    SR_DATA_DDR |= (1 << SR_DS);  //är input för SR men, output på arduiono som ska styra SR 
    SR_CL_DDR |= (1 << SR_CL);
    SR_LATCH_DDR |= (1 << SR_LATCH);
    bitstate = 0b00000000; //Q0-Q7;
    shift_out(bitstate); //nollställ initialt
}


void shift_out(uint8_t data)
{
    for (int i = 7; i >= 0; i--)
    {
        PORTB &= ~(1 << SR_CL);

        if (data & (1 << i))
            PORTB |= (1 << SR_DS);
        else
            PORTB &= ~(1 << SR_DS);

        PORTB |= (1 << SR_CL);
    }

    PORTD &= ~(1 << SR_LATCH);
    PORTD |= (1 << SR_LATCH);
    PORTD &= ~(1 << SR_LATCH);
}


void shiftregister_setbit(uint8_t bitnr){ //val = 1 eller 0 bitnr = bit plats

    bitstate |= (1 << bitnr );
    
    shift_out(bitstate);
        
}
    
    

void shiftregister_clearbit(uint8_t bitnr){
       
    bitstate &= ~(1 << bitnr); // 00000001 och flytta alla bitar 7 steg åt vänster och invertera (11111110)
     shift_out(bitstate);
}  
