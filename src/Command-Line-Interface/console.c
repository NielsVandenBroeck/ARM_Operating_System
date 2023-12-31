#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"

static int currentConsolePosition[]={5,5};//x,y

/**
 * Initializes UART by setting all the memory addresses to the correct values
 * @param baud: The baud rate of serial communication
 */
void console_init(){
    //Make the frame buffer ready to use
    fb_init();
}

void console_run(){
    console_print("Ubutnu@RaspberryPi");
    drawCursor(&currentConsolePosition[0], &currentConsolePosition[1], green);
    console_print("\nUbutnu@RaspberryPi");
}

/**
 * Prints a string to the UART
 * @param outputString: String to be printed
 */
void console_print(char *outputString){
    drawString(&currentConsolePosition[0], &currentConsolePosition[1], outputString, green);
}

void console_println(char *outputString){
    console_print(outputString);
    console_print("\r\n");
}

void console_printc(char c){
    /*
    if(c == '\n'){
        uart_writeByteBlocking('\r');
    }
    uart_writeByteBlocking(c);*/
}

char* console_readline(){
    //TODO
    throw("console_readline function not implemented yet");
    return "";
}