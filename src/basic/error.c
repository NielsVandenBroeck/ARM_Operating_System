#include "../graphicInterface/framebuffer.h"
#include "../uart/uart.h"

void throw(char *s){
    uart_init();
    uart_print("\nERROR: ");
    uart_print(s);
    uart_print("\n");

    for (int i=0;i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            drawScaledPixels(getWidth() - j -5 , 5 + i,red); //1 value in bitmap, has to be colored
        }
    }
}