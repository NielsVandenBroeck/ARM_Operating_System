#include "kernel.h"
#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
//#include "sd_card/sd_card.h"

#include "basic/multicore.h"


void core1_main(void)
{
    // user input
    wait_msec(3000);
    colorTest();
}

void core2_main(void)
{

}

void core3_main(void)
{

}

void testfunction(){
    unsigned int core0_val = 0;
    while (1) {
        printText("core0:", green);
        printInt(core0_val, green);
        printChar('\n', green);
        core0_val++;
        wait_msec(10);
//        if(core0_val == 20){
//            setScaleSize(1);
//        }
    }
}

void core0_main(void)
{
    /*
    initConsole();
    //start_core3(core3_main); // Kick it off on core 3
    colorTest2();
    //core0_main();                 // Loop endlessly
    runConsole();*/
    initConsole();
    printText("waiting...\n", green);
    wait_msec(10000);
    printText("starting uart init\n", green);
    uart_init();
    printText("uart init done\n", green);
    wait_msec(2000);
    int i = 0;
    while(1){
        printText("hello world: ", green);
        printInt(i, green);
        printText("\n", green);
        uart_writeText("Test \n");
        wait_msec(2000);
        i++;
    }
}

void main()
{
    core0_main();
    //irq_init_vectors();
    //enable_interrupt_controller();
    //irq_barrier();
    //irq_enable();
}