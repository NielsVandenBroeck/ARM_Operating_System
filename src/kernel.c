#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"


void main()
{
    char* temp;
    uart_init(115200);
    asm ("");

    uart_print("Hello world\r\n");
    drawPixel(0,0,'a');
    while(1){
        temp = uart_readline();
        uart_print(temp);
        uart_print("\r\n");
        free(temp);
    }
}