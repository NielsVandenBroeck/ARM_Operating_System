#include "uart.h"

void main()
{
    uart_init(115200);
    uart_print("Hello world\r\n");
    while(1){
        char input = uart_readline();
        char *pChar = &input;
        uart_print(pChar);
    }
    while(1);
}