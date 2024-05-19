#include "uart/uart.h"
#include "graphicInterface/framebuffer.h"
#include "Command-Line-Interface/console.h"
#include "basic/error.h"
#include "sd_card/sd_card.h"
#include "basic/multicore.h"
#include "basic/keyboard_interrupts.h"
#include "data_structures/Array/array.h"

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
    Array* myArray = newArray(10,sizeof (int));
    int* numbers = (int *)myArray->firstItem;

    for(int i = 0; i < 10; i++){
        numbers[i] = i;
    }

    arrayAppend(myArray);
    numbers[10] = 10;

    printInt(arrayGetLength(myArray),green);

//    Array* myArray2 = newArray(20,sizeof (int));
//    int* numbers2 = (int *)myArray2->firstItem;
//    for(int i = 0; i < 20; i++){
//        numbers2[i] = i+50;
//    }
//    ArrayConcat(myArray,myArray2);
//    printText("Length:",green);
//    printInt(arrayGetLength(myArray),green);
    printChar('\n', green);
    for(int i = 0; i < arrayGetLength(myArray);  i++){
        printText("Number: ", green);
        printInt(*(int *)(arrayGetItem(myArray, i)), green);
        printChar('\n', green);
    }
    start_core1(core1_main);
    core0_main();
}