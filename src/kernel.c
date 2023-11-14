#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"


void main()
{
    char* temp;
    uart_init(115200);
    asm ("");

    uart_print("Hello world\r\n");

    fb_init();
    int g = 0;
    while(1){
        for(int i = 0; i <= getWidth(); i++){
            for(int a = 0; a <= getHeight(); a++){
                drawPixel(i,a,i/20+g);
            }
        }
        g++;
    }


    while(1){
        temp = uart_readline();
        uart_print(temp);
        uart_print("\r\n");
        free(temp);
    }
}