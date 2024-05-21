#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/multicore.h"
#include "basic/keyboard_interrupts.h"

void core1_main(void)
{
    uart_init();
    clear_core1();
    KeyboardInterruptionInit();
    KeyboardInterruptionHandler(uart_readchar);
}

void core0_main(void)
{
    runConsole();
    //    uart_init();
    //    while(1){
    //        char text_back = uart_readchar();
    //        printChar(text_back, green);
    //    }
}

void main()
{
    //Make the frame buffer ready to use
    fb_init();
    initConsole();
    uart_init();

    printText("hello world\n", green);

    start_core1(core1_main);
    core0_main();
}