#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
#include "sd_card/sd_card.h"

#include "basic/multicore.h"

void core3_main(void)
{
    unsigned int core3_val = 0;

    clear_core3();                // Only run once

    while (1) {
        wait_msec(3000);
        printText("core3:", green);
        printInt(core3_val, green);
        if (core3_val < 100) core3_val++;
    }
}

void core2_main(void)
{
    unsigned int core2_val = 0;

    clear_core2();                // Only run once

    while (1) {
        wait_msec(2000);
        printText("core2:", green);
        printInt(core2_val, green);
        if (core2_val < 100) core2_val++;
    }
}

void core1_main(void)
{
    unsigned int core1_val = 0;

    clear_core1();                // Only run once

    while (1) {
        wait_msec(1000);
        printText("core1:", green);
        printInt(core1_val, green);
        if (core1_val < 100) core1_val++;
    }
}

void core0_main(void)
{
    unsigned int core0_val = 0;

    while (1) {
        wait_msec(1000);
        printText("core0:", green);
        printInt(core0_val, green);
        if (core0_val < 100) core0_val++;
    }
}

void main()
{
    initConsole();
    start_core3(core3_main);      // Kick it off on core 3
    core0_main();                 // Loop endlessly


    runConsole();
}