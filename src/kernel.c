#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface//console.h"
#include "basic/error.h"


void main()
{
    uart_init(115200);
    asm ("");

    uart_print("Hello world\r\n");
    console_init();


    int x = 0;
    for(int r = 0; r < 256; r += 4){
        for(int g = 0; g < 256; g += 4){
            for(int b = 0; b < 256; b += 4){
                float newColor = ((b & 0xff) << 16) + ((g & 0xff) << 8) + ((r & 0xff));
                for(int i = 0; i < 5; i++){
                    drawPixel(x, i, newColor);
                }
                x++;
            }
        }
    }

    console_println("done");
    while(1){
        char* input = uart_readline();
        console_println(input);
        //setInterfaceScaling(5);
        free(input);
    }

    console_run();
}