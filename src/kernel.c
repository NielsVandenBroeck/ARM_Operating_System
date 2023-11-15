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
    console_run();
}