#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
#include "sd_card/sd_card.h"

void main()
{
   // uart_init(115200);
   asm ("");
   // uart_print("Hello world\r\n");

    console_init();
    console_printline("Starting Ubutnu...");
    //todo loading OS
    console_clear();
    console_run();
    console_printline("Starting Ubutnu...");


    while(1){
        wait_msec(1000);
        console_printline("1 sec passed");
    }



    setInterfaceScaling(2);
    //console_printline("Hello world yeet");
    while(1){
        char* temp = uart_readline();
        console_printline(temp);

        //setInterfaceScaling(1);
        //uart_print("\r\n");
        free(temp);
    }

    int g = 0;
    while(1){
        for(int i = 0; i <= getWidth(); i++){
            for(int a = 0; a <= getHeight(); a++){
                if(g % 3 == 0){
                    drawPixel(a,i,green);
                }
                else if(g % 3 == 1){
                    drawPixel(a,i,red);
                }
                else if(g % 3 == 2){
                    drawPixel(a,i,blue);
                }
            }
        }
        g++;
    }
}