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

    console_print_int(1);
    console_print_int(2);
    console_print_int(3);
    console_print_int(4);
    console_print_int(5);
    console_print_int(6);
    console_print_int(7);
    console_print_int(8);
    console_print_int(9);
    console_print_int(251468515);

    //sd_card_init();

    console_println("done 1");
    while(1){
        char* input = uart_readline();
        console_println(input);
        //setInterfaceScaling(5);
        free(input);
    }

    console_run();
}