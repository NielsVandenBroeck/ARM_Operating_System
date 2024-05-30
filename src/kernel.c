#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "command_line_interface/console.h"
#include "basic/multicore.h"
#include "basic/keyboard_interrupts.h"
#include "basic/mb.h"
#include "taskbar/taskbar.h"
#include "basic/time.h"
#include "clock/timer.h"

void core1_main(void)
{
    uart_init();
    uart_print("core1 started");
    KeyboardInterruptionInit();
    KeyboardInterruptionHandler(uart_readchar);
}

//compiled program
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

void externalRunner(){
    uart_print("\nmemory copy\n");

//    asm volatile (
//            "mov x6, %0\n"   // Move the address to x0
//            "blr x6\n"        // Branch to the address in x0
//            :
//            : "r"(func_ptr)  // Input operand
//            : "x6"           // Clobbered register
//            );


}
/*
void start_external_code(){
    unsigned long* firstItem = malloc(2 * sizeof(unsigned long));
    for(int i = 0; i < 2; i++){
        firstItem[i] = testProg[i];
    }
    void (*func_ptr)() = (void*)firstItem;
    unsigned long lrRegister = read_from_lr();
    uart_print("\nJumping to mem adress\n");
        asm volatile (
            "mov x6, %0\n"   // Move the address to x0
            "blr x6\n"        // Branch to the address in x0
            :
            : "r"(func_ptr)  // Input operand
            : "x6"           // Clobbered register
            );
    uart_print("\nWe are back\n");
    write_to_lr(lrRegister);

    wait_msec(1000);
    uart_print("\nWe are back\n");

    int address = 93856432;
    int *ptr = (int *)address;
    // For demonstration purposes, print the value stored at the address
    while(1){
        uart_print("\n\nValue at address ");
        uart_printInt((void *)ptr);
        uart_print(" is  ");
        uart_printInt(*ptr);
        wait_msec(1000);
    }

}
*/

void core0_main(void)
{
    runConsole();
}
void core2_main(void)
{
    initTimer();
}

//compiled program
unsigned long testProg[] = {0xD2800547,0xD65F03C0,};

unsigned int read_from_x0() {
    unsigned int value;
    __asm volatile ("mov %0, x0" : "=r" (value));
    return value;
}

void write_to_x0(unsigned int value) {
    __asm volatile ("mov x0, %0" : : "r" (value));
}

void extrnTest()
{
    unsigned long* firstItem = malloc(2 * sizeof(unsigned long));
    for(int i = 0; i < 2; i++){
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
    uart_print("\n Core 2 done: ");
    int i = read_from_x0();
    uart_printInt(i);
    uart_print("\n");
}

void main()
{
    //Make the frame buffer ready to use
    fb_init();
    uart_init();
    taskBarInit();
    cores_init();

    initConsole();
    //printText("hello world\n", green);
    start_core1(core1_main);
    wait_msec(500);
    start_core2(core2_main);
    wait_msec(500);
    extrnTest();
    core0_main();
}