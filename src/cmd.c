#include "uart.h"

static unsigned char cmd_index = 0;
#define CMD_BUFFER_SIZE 32

int get_input(char *cmd_buffer){
    
    char c; 
   
    while (!uart_read_char(&c)) //uart_read_char(&c)
    {
        uart_write_char(c);

        if (c == '\r' || c == '\n')
        {
            uart_write_string("\n");

            if (cmd_index > 0)
            {
                cmd_buffer[cmd_index] = '\0';
                //process_command(cmd_buffer);
                
                cmd_index = 0;
                return 1;
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
                return 0;
            }
        }
    }
    return 0;
}