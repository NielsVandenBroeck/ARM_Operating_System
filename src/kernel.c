#include "kernel.h"
#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
#include "sd_card/sd_card.h"

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
    initConsole();
    setScaling(2);
    setRotation(3);
    //start_core1(core1_main);
    testfunction();

}

void main()
{
    core0_main();
    //irq_init_vectors();
    //enable_interrupt_controller();
    //irq_barrier();
    //irq_enable();
}