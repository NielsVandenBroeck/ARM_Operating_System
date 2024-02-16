#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../Command-Line-Interface/Font.h"

static int currentConsolePosition[]={10,10};//x,y
static int CURRENT_COLOR = green;


void initConsole(){
    //Make the frame buffer ready to use
    fb_init();
}

void runConsole(){
    printText("Ubutnu@user");
    nextLine();
    //todo wait for input
    int x = 1;
    while(1){
        wait_msec(100);
        printText("test line: ");
        printInt(x);
        nextLine();
        x++;
    }
}

void nextLine(){
    currentConsolePosition[0] = XOFFSET; //On \r, go back to begin of screen
    currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
}

char* readLine(){
    //TODO
    throw("console_readline function not implemented yet");
    return "";
}

void clearConsole(){
    drawScreen(black);
    currentConsolePosition[0] = 10;
    currentConsolePosition[1] = 10;
}

void setColor(int newColor){
    CURRENT_COLOR = newColor;
}

void printText(char *s){
    while (*s) {
        printChar(*s);
        s++; //read next character in string
    }
}

void printChar(char c){
    if (c == '\r') {
        currentConsolePosition[0] = XOFFSET; //On \r, go back to begin of screen
    } else if(c == '\n') {
        currentConsolePosition[0] = XOFFSET;
        currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
    } else {
        unsigned char *glyph = (unsigned char *)&font + (c < FONT_NUMGLYPHS ? c : 0) * FONT_BPG;

        for (int i=0;i<FONT_HEIGHT;i++) {
            for (int j=0;j<FONT_WIDTH;j++){
                if(*glyph & 1 << j){
                    drawPixel(currentConsolePosition[0]+j, currentConsolePosition[1]+i, CURRENT_COLOR); //1 value in bitmap, has to be colored
                }
                else {
                    //todo background/highlights?
                    drawPixel(currentConsolePosition[0]+j, currentConsolePosition[1]+i, 0x000000); //0 value in bitmap, pixel set to background color
                }
            }
            glyph += FONT_BPL; //position for next row
        }
        currentConsolePosition[0] += FONT_WIDTH; //position for next character
    }
}

void printInt(unsigned int number){
    unsigned int tempNumber = number;
    int devider = 1;
    while(tempNumber > 9){
        tempNumber = (int)(tempNumber / 10);
        devider *= 10;
    }

    while(devider >= 1){
        unsigned int digitNumber = (unsigned int)(number / devider);
        char asChar = digitNumber + '0';
        printChar(asChar);
        number -= digitNumber * devider;
        devider = (int)(devider / 10);
    }
}

void drawCursor(){
    for (int i=0;i<LINEHEIGHT;i++) {
        for (int j=0;j<1;j++){
            drawPixel(currentConsolePosition[0]+j, currentConsolePosition[1]+i, CURRENT_COLOR);
        }
    }
    currentConsolePosition[0] += FONT_WIDTH;
}