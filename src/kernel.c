#include "uart.h"

void main()
{
    uart_init(115200);
    while(1){
        char test = uart_readline();
        uart_print(test);
    }
    while(1);
}