#ifndef SHIFTR_H
#define SHIFTR_H

void shift_init();
void shift_out(uint8_t data);
void shiftregister_setbit(uint8_t bitnr);
void shiftregister_clearbit(uint8_t bitnr);

#endif

