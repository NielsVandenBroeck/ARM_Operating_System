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

//compiled program
unsigned long testProg[] = {0xD2800640,0xD2800600,0x91002800,0xD65F03C0,};

unsigned int read_from_x0() {
    unsigned int value;
    __asm volatile ("mov %0, x0" : "=r" (value));
    return value;
}

void write_to_x0(unsigned int value) {
    __asm volatile ("mov x0, %0" : : "r" (value));
}

void core2_main(void)
{
    unsigned long* firstItem = malloc(4 * sizeof(unsigned long));
    for(int i = 0; i < 4; i++){
        firstItem[i] = testProg[i];
    }
    void (*func_ptr)() = (void*)firstItem;
    asm volatile (
            "mov x0, %0\n"   // Move the address to x0
            "blr x0\n"        // Branch to the address in x0
            :
            : "r"(func_ptr)  // Input operand
            : "x0"           // Clobbered register
            );
    printText("\n Core 2 done\n", green);
    int i = read_from_x0();
    printInt(i, blue);
    printText("\n", green);
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