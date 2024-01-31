#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"

static int currentConsolePosition[]={5,5};//x,y
static int CURRENT_COLOR = green;

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
    drawCursor(&currentConsolePosition[0], &currentConsolePosition[1], CURRENT_COLOR);
    console_print("\nUbutnu@RaspberryPi");
}

/**
 * Prints a string to the UART
 * @param outputString: String to be printed
 */
void console_print(char *outputString){
    drawString(&currentConsolePosition[0], &currentConsolePosition[1], outputString, CURRENT_COLOR);
}

void console_println(char *outputString){
    console_print(outputString);
    console_print("\r\n");
}

void console_print_int(int number){
    int tempNumber = number;
    int devider = 1;
    while(tempNumber > 9){
        tempNumber = (int)(tempNumber / 10);
        devider *= 10;
    }

    while(devider >= 1){
        int digitNumber = (int)(number / devider);
        char asChar = digitNumber + '0';
        console_printc(asChar);
        number -= digitNumber * devider;
        devider = (int)(devider / 10);
    }
    console_print("\r\n");
}

void console_printc(char c){
    if(c == '\n'){
        uart_writeByteBlocking('\r');
    }
    char* charString = (char*)malloc(1 * sizeof(char));
    charString[0] = c;
    console_print(charString);
}

char* console_readline(){
    //TODO
    throw("console_readline function not implemented yet");
    return "";
}

void console_color(int newColor){
    CURRENT_COLOR = newColor;
}