#include "app.h"
#include "config.h"
#include "gpio.h"
#include "pins.h"
#include "uart.h"
#include "millis.h"
#include "spi.h"
#include <string.h>
#include "74hc595.h"
#include "led.h"
#include <util/delay.h>
#include "keypad.h"
#include "buzzer.h"
#include "pwm.h"
#include "servo.h"
#include "twi.h"
#include <stdlib.h>
#include "cmd.h"
#include "rtc.h"
#include "mfrc522.h"
#include "keyscan.h"

#define CMD_BUFFER_SIZE 32

static char cmd_buffer[CMD_BUFFER_SIZE];
static unsigned char cmd_index = 0;
int inputResult = 0;

typedef enum{
    IDLE,
    INPUT_AWAIT,
    ACCESS_GRANTED,
    ERROR
    
    
}State;

State test_code_input();

State state = IDLE;
millis_t start;

static void process_command(const char *cmd)
{
    if (strcmp(cmd, "led on") == 0)
    {
        green_led_on();
                
    }
    else if (strcmp(cmd, "led off") == 0)
    {
        green_led_off();
        
    }
    else if(strcmp(cmd, "time set") == 0){
        
        input_time();

    }
     else if(strcmp(cmd, "time get") == 0){
        
        read_time();

    }
    else if(strcmp(cmd, "test code") == 0){
        
        state = test_code_input();
        
    }
    else if(strcmp(cmd, "buzzer on") == 0){
        
        buzzer_on();
        
    }
    else if(strcmp(cmd, "buzzer off") == 0){
        
        buzzer_off();
        
    }
    else if(strcmp(cmd, "servo open") == 0){
        
        servo_open();
        
    }
    else if(strcmp(cmd, "servo close") == 0){
        
        servo_close();
        
    }

    else if (strcmp(cmd, "help") == 0)
    {
        uart_write_string("Commands: help,test code,servo open,servo close, buzzer on ,buzzer off,time get, time set,led on, led off\n");
    }
    else
    {
        uart_write_string("Unknown command\n");
    }
}

void app_init(void)
{
    // gpio_pin_output(&LED_DDR, LED_PIN);
    // gpio_pin_low(&LED_PORT, LED_PIN);

    millis_init();
    shift_init();
    led_init();
    keypad_init();
    buzzer_init();
    //timer1_init();
    servo_init();
    twi_init(100000);
    uart_init(UART_BAUDRATE);
    spi_init();

    mfrc522_init();

    uart_write_string("System ready. system will do a test run\n");
   

    //testa system vid start
    red_led_on();
    _delay_ms(500);
    
    red_led_off();
    _delay_ms(500);

    blue_led_on();
    _delay_ms(500);
     
    blue_led_off();
    _delay_ms(500);

    green_led_on();
    _delay_ms(500);

    green_led_off();
    _delay_ms(500);
                  
    buzzer_on();
    _delay_ms(500);
    buzzer_off();
     
    servo_open();
    _delay_ms(1000);
    servo_close();
    _delay_ms(1000);
     
    //  test_code_input();

    uart_write_string("Type: help\n");
}

void app_run(void)
{
    
    switch(state){

        case IDLE:
            green_led_off();
            blue_led_off();
            red_led_on();
            break;
        case ACCESS_GRANTED:
            red_led_off();
            blue_led_off();
            green_led_on();
            break;
        default:
            break;
    }
    
        uint8_t atqa[2];
        uint8_t atqa_len;

        if (mfrc522_request_a(atqa, &atqa_len) == MFRC522_OK)
        {
            mfrc522_uid_t uid; 
            uart_write_string("CARD FOUND\n");
            _delay_ms(5);

            
            if (mfrc522_anticoll_select(&uid) == MFRC522_OK)
            {
                uart_write_string("UID OK\n");
                uart_write_hex_buffer(uid.uid, 4);
                blue_led_off();
            }

            mfrc522_halt();
        }

    
     
    char c;
    
     while(uart_read_char(&c)) {
        uart_write_char(c);

        if (c == '\r' || c == '\n')
        {
            uart_write_string("\n");

            if (cmd_index > 0)
            {
                cmd_buffer[cmd_index] = '\0';
                process_command(cmd_buffer);
                cmd_index = 0;
            }
        }
        else
        {
            if (cmd_index < (CMD_BUFFER_SIZE - 1))
            {
                cmd_buffer[cmd_index++] = c;
            }
            else
            {
                cmd_index = 0;
                uart_write_string("\nCommand too long\n");
            }
        }
    
    }
}

    State test_code_input(){
    
    
    int code_check = 0;
    millis_t  time_start = millis_get();
    millis_t start_blink = millis_get();    
    
    while(true){
    
        
        int code_check = screen();
        // if(((millis_t)(millis_get() - start_blink) == 250)){
        //     red_led_on();
        // }
        // if(((millis_t)(millis_get() - start_blink) == 500)){
        //     red_led_off();
        //     start_blink = millis_get();
        // }
        
        if(((millis_t)(millis_get() - time_start) >= 5000) || code_check == -1){
            screen_init();
            return IDLE;
            
        }
        if(code_check == 1){
            return ACCESS_GRANTED;
        }
        
    }
    return ERROR;

}

