#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
#include "sd_card/sd_card.h"
#include "basic/multicore.h"
#include "basic/keyboard_interrupts.h"
#include "data_structures/array.h"

void core1_main(void)
{
    uart_init();
    clear_core1();
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
    initConsole();
    //start_core1(core1_main);

//    drawGlyph('h', green);
//    Array* myUpperArray = newArray(1,sizeof (Array*));
//    for(int k = 0; k<50;k++){
//        Array* consoleLine = newArray(1, sizeof(int));
//        arrayAppend(myUpperArray);
//        *(Array**)arrayGetItem(myUpperArray, k) = consoleLine;
//        for(int i = 0; i < 12; i++){
//            arrayAppend(consoleLine);
//            *(int *)arrayGetItem(consoleLine,i) = i;
//        }
//        drawGlyph('\n',green);
//        for(int i = 0; i < arrayGetLength(myUpperArray);  i++){
//            Array* childArray = (Array *)arrayGetItem(myUpperArray,i);
//            for(int j = 0; j < arrayGetLength(childArray);  j++){
//                drawGlyph(*(int *)(arrayGetItem(childArray, j)), green);
//            }
//        }
//    }
    core0_main();
}