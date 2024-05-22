#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/multicore.h"
#include "basic/keyboard_interrupts.h"
#include "basic/mb.h"

void core1_main(void)
{
    uart_init();
    clear_core1();
    KeyboardInterruptionInit();
    KeyboardInterruptionHandler(uart_readchar);
}

//compiled program
unsigned long testProg[] ={0xD2809A41,0x52800142,0xB9000022,0xD65F03C0};


unsigned int read_from_x7() {
    unsigned int value;
    __asm volatile ("mov %0, x7" : "=r" (value));
    return value;
}

void write_to_x7(unsigned int value) {
    __asm volatile ("mov x7, %0" : : "r" (value));
}

unsigned int read_from_lr() {
    unsigned int value;
    __asm volatile ("mov %0, lr" : "=r" (value));
    return value;
}

void write_to_lr(unsigned int value) {
    __asm volatile ("mov lr, %0" : : "r" (value));
}
/*
void start_external_code(){
    unsigned long* firstItem = malloc(13 * sizeof(unsigned long));
    for(int i = 0; i < 13; i++){
        firstItem[i] = testProg[i];
    }
    void (*func_ptr)() = (void*)firstItem;
    unsigned long lrRegister = read_from_lr();
    asm volatile (
            "mov x6, %0\n"   // Move the address to x0
            "blr x6\n"        // Branch to the address in x0
            :
            : "r"(func_ptr)  // Input operand
            : "x6"           // Clobbered register
            );
    write_to_lr(lrRegister);
    int i = read_from_x7();
}

void core2_main(void)
{
    clear_core2();
    while(1){
        start_external_code();
        wait_msec(5000);
    }
}
*/

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

    //printText("hello world\n", green);
    start_core1(core1_main);

//    unsigned long* firstItem = malloc(4 * sizeof(unsigned long));
//    for(int i = 0; i < 4; i++){
//        firstItem[i] = testProg[i];
//    }
//    void (*func_ptr)() = (void*)firstItem;
//    unsigned long lrRegister = read_from_lr();
    /*
    asm volatile (
            "mov x6, %0\n"   // Move the address to x0
            "blr x6\n"        // Branch to the address in x0
            :
            : "r"(func_ptr)  // Input operand
            : "x6"           // Clobbered register
            );*/

//    clear_core2();
//    start_core2(func_ptr);
//    write_to_lr(lrRegister);

//    wait_msec(5000);
    //start_core2(core2_main);
//    unsigned int setNumber = mmio_read(1234);
//    printText("\nSet number: ",red);
//    printInt(setNumber, red);
//    printText("\n",red);
    core0_main();
}