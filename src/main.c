#include "app.h"
#include <avr/interrupt.h>

int main(void)
{
    app_init();
    sei();

    while (1)
    {
       app_run();
    // DDRD |= (1 << PD3);

    // TCCR2A = (1 << WGM21) | (1 << WGM20);
    // TCCR2A |= (1 << COM2B1);
    // TCCR2B = (1 << CS21);

    // OCR2B = 127;
    }
}