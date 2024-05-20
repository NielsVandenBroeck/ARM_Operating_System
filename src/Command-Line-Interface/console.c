#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../Command-Line-Interface/Font.h"
#include "../basic/multicore.h"
#include "../basic/keyboard_interrupts.h"
#include "../data_structures/array.h"

static int currentConsolePosition[]={XOFFSET,LINEHEIGHT+2};//x,y
static int currentCursorPosition[]={XOFFSET,LINEHEIGHT+2};
static int CURRENT_COLOR = green;

Array* textBuffer;


void initConsole(){
    //Make the frame buffer ready to use
    fb_init(1);
    setScaleSize(2);
    setRotation(3);
    //Create the array buffer for the displayed text

//    textBuffer = newArray(1,sizeof (Array));
//    Array* consoleLine = newArray(1,sizeof (char));
//    *(Array*)arrayGetItem(textBuffer,0) = *consoleLine;

    textBuffer = newArray(1, sizeof(Array*));
    Array* consoleLine = newArray(1, sizeof(char));
    *(Array**)arrayGetItem(textBuffer, 0) = consoleLine;

    Attach(processChar);
}

void runConsole(){
    int val = 0;
    while(1){
        printText("core0: ",CURRENT_COLOR);
        printInt(val,CURRENT_COLOR);
        printChar('\n',CURRENT_COLOR);
        wait_msec(500);
//        clearConsole();
//        if(val > 80){
//            drawFromBuffer(val-80);
//        }
//        else{
//            drawFromBuffer(0);
//        }
        val++;
    }
    runCursor();
}

void processChar(char c){
    printChar(c,CURRENT_COLOR);
}

void nextLine(){
    currentConsolePosition[0] = XOFFSET;
    if(currentConsolePosition[1] >= getHeight()-LINEHEIGHT*5){
//        drawFromBuffer(0); //todo
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
    drawScreen(red);
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

void printChar(char c, int color){
    //todo \r, \t, ...
    if(c == '\n') {
        nextLine();

        // end this console line and add a new one to textBuffer
        arrayAppend(textBuffer);
        Array* consoleLine = newArray(0,sizeof (char));
        *(Array**)arrayGetItem(textBuffer,arrayGetLength(textBuffer)-1) = consoleLine;
    }
    else{
        // draw character on screen
        drawGlyph(c,color);

        // add character to the textBuffer
        Array* currentLine = *(Array**)arrayGetItem(textBuffer, arrayGetLength(textBuffer)-1);
        arrayAppend(currentLine);
        *(char *)arrayGetItem(currentLine,arrayGetLength(currentLine)-1) = c;
    }
    updateCursorPosition();
}

void drawGlyph(char c, int color){
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

void drawFromBuffer(int startLine){
    unsigned int length = arrayGetLength(textBuffer);
    if(arrayGetLength(textBuffer) > (getHeight()-1)/LINEHEIGHT){
        length = (getHeight()-1)/LINEHEIGHT;
    }
    for(int i = startLine; i < length;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            drawGlyph(*(char *)(arrayGetItem(consoleLine, j)), CURRENT_COLOR);
        }
        //do not print nextline because current console line is still active.
        if(i == length-1) break;
        nextLine();
    }
}