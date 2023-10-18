#include "uart.h"

void main()
{
    uart_init(115200);

    uart_print("Sup rakkers\r\n");
    while(1);
}