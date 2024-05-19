#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../Command-Line-Interface/Font.h"
#include "../basic/multicore.h"

static int currentConsolePosition[]={XOFFSET,LINEHEIGHT+2};//x,y
static int currentCursorPosition[]={XOFFSET,LINEHEIGHT+2};
static int CURRENT_COLOR = green;

void initConsole(){
    //Make the frame buffer ready to use
    fb_init(1);
    setScaling(2);
    setRotation(0);
}

void runConsole(){
    printText("Ubutnu@user$ ", green);
    runCursor();
}

void processChar(char c){
    printChar(c,CURRENT_COLOR);
}

void nextLine(){
    currentConsolePosition[0] = XOFFSET;
    if(currentConsolePosition[1] >= getHeight()-LINEHEIGHT*5){
        scrollUp();
    } else {
        currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
    }
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
    currentCursorPosition[0] = 10;
    currentCursorPosition[1] = 10;
}

void setColor(int newColor){
    CURRENT_COLOR = newColor;
}

void printText(char *s, int color){
    while (*s) {
        printChar(*s, color);
        s++; //read next character in string
    }
}

void printChar(char c, int color){
    if (c == '\r') {
        currentConsolePosition[0] = XOFFSET; //On \r, go back to begin of screen
    } else if(c == '\n') {
        nextLine();
    } else {
        unsigned char *glyph = (unsigned char *)&font + (c < FONT_NUMGLYPHS ? c : 0) * FONT_BPG;

        for (int i=0;i<FONT_HEIGHT;i++) {
            for (int j=0;j<FONT_WIDTH;j++){
                if(*glyph & 1 << j){
                    drawScaledPixels(currentConsolePosition[0]+j, currentConsolePosition[1]+i, color); //1 value in bitmap, has to be colored
                }
                else {
                    //todo background/highlights?
                    //drawScaledPixels(currentConsolePosition[0]+j, currentConsolePosition[1]+i, 0x000000); //0 value in bitmap, pixel set to background color
                }
            }
            glyph += FONT_BPL; //position for next row
        }
        currentConsolePosition[0] += FONT_WIDTH; //position for next character
    }
    updateCursorPosition();
}

void printInt(unsigned int number, int color){
    unsigned int tempNumber = number;
    int devider = 1;
    while(tempNumber > 9){
        tempNumber = (int)(tempNumber / 10);
        devider *= 10;
    }

    while(devider >= 1){
        unsigned int digitNumber = (unsigned int)(number / devider);
        char asChar = digitNumber + '0';
        printChar(asChar, color);
        number -= digitNumber * devider;
        devider = (int)(devider / 10);
    }
}

void updateCursorPosition(){
    clearCursor();
    currentCursorPosition[0] = currentConsolePosition[0];
    currentCursorPosition[1] = currentConsolePosition[1];
    drawCursor();
}

void runCursor(){
    while(1){
        drawCursor();
        wait_msec(500);
        clearCursor();
        wait_msec(500);
    }
}

void drawCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixels(currentCursorPosition[0]-1, currentCursorPosition[1]+i, CURRENT_COLOR);
    }
}

void clearCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixels(currentCursorPosition[0]-1, currentCursorPosition[1]+i, black);
    }
}



void scrollUp(){
    //start at the top, loop over every line,
    // push it up by one line.
    for(int line = 1; line < getHeight()/10; line++){
        for(int y = line*LINEHEIGHT; y < line*LINEHEIGHT+LINEHEIGHT; y++){
            for(int x = 0; x < getWidth(); x++){
                int color = getPixelColor(x,y);
                drawScaledPixels(x,y-LINEHEIGHT,color);
            }
        }
    }
}

void setScaling(int size){
    setScaleSize(size);
}