#include "uart.h"
#include "twi.h"
#include <stdlib.h>
#include "rtc.h"
#include <ctype.h>

#define TIMEBUFFER_SIZE 7

char c;
int converttobcd(uint8_t *bcd, int val1, int val2);

bool validate_time(uint8_t time_v, uint8_t rtc_reg){

    uint8_t time_max = 0;
    
    switch(rtc_reg)
    {
    case 0x00:
        time_max = S_MAX;    
        break;
    case 0x01:
        time_max = M_MAX;
    case 0x02:
        time_max = HR_MAX;
    default:
        break;
    }
    
    return (time_v >= 0 ) && (time_v <= time_max);
    
}


void input_time(){

    char val1, val2;
    uint8_t rtc_reg;
    uint8_t bcdresult;
    
    while(true){
        uart_write_string("write time to save to rtc : \n");

        for (int i = 0; i < 3; i++) {

            if (i == 0) {
                uart_write_string("skriv timmar: ");
                rtc_reg = 0x02;
            }
            if (i == 1) {
                uart_write_string("skriv minuter: ");
                rtc_reg = 0x01;
            }
            if (i == 2) {
                uart_write_string("skriv sekunder: ");
                rtc_reg = 0x00;
            }
        
            while (!uart_read_char(&val1) || !isdigit(val1)){} 
            uart_write_char(val1);

            while (!uart_read_char(&val2) || !isdigit(val1)) {}
            uart_write_char(val2);
            uart_write_string("\n");
            
            int value = (val1 - '0') * 10 + (val2 - '0');
            
            if(validate_time(value, rtc_reg)){
                break;
            }
            else{
                uart_write_string("wrong time format"); // eller return ERROR
            }
        }
    }

    converttobcd(&bcdresult,
                 (val1 - '0'),
                 (val2 - '0'));

    twi_write_register(0x68, rtc_reg, bcdresult);
}
        

int converttobcd(uint8_t *bcd, int val1, int val2){

    if(val1 > 9 || val2 > 9){
        return -1;
    }
    *bcd = ( val1 << 4) | (val2);
    return 1;
}
uint8_t convvertbcdtoint(uint8_t bcd){ //0x24 -> 24
    return ((bcd>> 4) * 10) + (bcd & 0x0F);
}

//read RTC 
void read_time(){

        uint8_t time_hrs;
        uint8_t time_min;
        uint8_t time_s; 
        char hrs [3];
        char min [3];
        char s [3];
        char bufferout[4] = {0};
        
        uart_write_string("rtc time:");
        
        twi_read_register(0x68, 0x02, &time_hrs);
        twi_read_register(0x68, 0x01, &time_min);
        twi_read_register(0x68, 0x00, &time_s);
        
        itoa(convvertbcdtoint(time_hrs),hrs, 10);
        itoa(convvertbcdtoint(time_min),min, 10);  
        itoa(convvertbcdtoint(time_s),s, 10);

        if(time_hrs < 10 ) uart_write_char('0');
        uart_write_string(hrs);
        uart_write_char(':');
        if(time_min < 10 ) uart_write_char('0');
        uart_write_string(min);
        uart_write_char(':');
        if(time_s < 10 ) uart_write_char('0');
        uart_write_string(s);
        uart_write_string("\n");
        

        //uint8_t timedataresult = ((timedata >> 4) * 10) + (timedata & 0x0F);
        //uint8_t timeresult = convvertbcdtoint(timedata);
    }    

