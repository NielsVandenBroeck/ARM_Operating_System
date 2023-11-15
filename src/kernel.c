#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"
#include "console/console.h"
#include "basic/error.h"


void main()
{
    char* temp;
    uart_init(115200);
    asm ("");

    uart_print("Hello world\r\n");
    console_init();
    console_println("Hello world");
    console_println("Hello world");
    console_println("Hello world");
    console_println("Hello world");
}