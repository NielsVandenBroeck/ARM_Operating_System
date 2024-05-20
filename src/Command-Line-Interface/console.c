#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../Command-Line-Interface/Font.h"
#include "../basic/multicore.h"
#include "../basic/keyboard_interrupts.h"
#include "../data_structures/array.h"

static int currentConsolePosition[]={XOFFSET,YOFFSET};//x,y
static int currentCursorPosition[]={XOFFSET,YOFFSET};
static int CURRENT_COLOR = red;
int currentWindow = 0; //The line (index in textBuffer) that is shown first (on top) of the screen

Array* textBuffer;

void initConsole(){
    //Make the frame buffer ready to use
    fb_init(1);
    setScaleSize(2);
    setRotation(0);
    //Create the array buffer for the displayed text
    textBuffer = newArray(1, sizeof(Array*));
    Array* consoleLine = newArray(0, sizeof(Character));
    *(Array**)arrayGetItem(textBuffer, 0) = consoleLine;

    Attach(processChar);
}

void runConsole(){
    int val = 0;
    while(1){
        printText("core0: \n",green);
        printInt(val,CURRENT_COLOR);
        printChar('\n',CURRENT_COLOR);
        wait_msec(500);
        val++;
        if(val == 10){
            rotateScreen(3);
        }
        if(val == 15){
            changeTextColor(blue);
        }
        if(val == 20){
            scaleScreen(1);
        }
        if(val == 30){
            rotateScreen(0);
            scaleScreen(2);
        }
    }
    runCursor();
}

void processChar(char c){
    //printChar(c,CURRENT_COLOR);
}

char* readLine(){
    //TODO
    throw("console_readline function not implemented yet");
    return "";
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
        // end this console line and add a new one to textBuffer
        arrayAppend(textBuffer);
        Array* consoleLine = newArray(0,sizeof (Character));
        *(Array**)arrayGetItem(textBuffer,arrayGetLength(textBuffer)-1) = consoleLine;
        if(currentConsolePosition[1] >= getHeight()-LINEHEIGHT*2){
            clearConsole();
            currentWindow += 1;
            drawFromBuffer();
        }
        else{
            currentConsolePosition[0] = XOFFSET;
            currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
        }
    }
    else{
        // draw character on screen
        drawGlyph(c,currentConsolePosition[0],currentConsolePosition[1],color);
        currentConsolePosition[0] += FONT_WIDTH; //position for next character

        // add character to the textBuffer
        Array* currentLine = *(Array**)arrayGetItem(textBuffer, arrayGetLength(textBuffer)-1);
        arrayAppend(currentLine);
        Character newCharacter = {c, color};
        *(Character *)arrayGetItem(currentLine,arrayGetLength(currentLine)-1) = newCharacter;
    }
    //updateCursorPosition();
}

void drawGlyph(char c, int x, int y, int color){
    unsigned char *glyph = (unsigned char *)&font + (c < FONT_NUMGLYPHS ? c : 0) * FONT_BPG;
    for (int i=0;i<FONT_HEIGHT;i++) {
        for (int j=0;j<FONT_WIDTH;j++){
            if(*glyph & 1 << j){
                drawScaledPixels(x+j, y+i, color); //1 value in bitmap, has to be colored
            }
            else {
                //todo background/highlights?
                //drawScaledPixels(currentConsolePosition[0]+j, currentConsolePosition[1]+i, 0x000000); //0 value in bitmap, pixel set to background color
            }
        }
        glyph += FONT_BPL; //position for next row
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

void clearConsole(){
    int xOffset = XOFFSET;
    int yOffset = YOFFSET;

    unsigned int length = arrayGetLength(textBuffer);
    if(length > (getHeight()/LINEHEIGHT)-2){
        length = (getHeight()/LINEHEIGHT)-2+currentWindow;
    }
    for(int i = currentWindow; i < length;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            Character c = *(Character *)(arrayGetItem(consoleLine, j));
            drawGlyph(c.value, xOffset, yOffset, black);
            xOffset += FONT_WIDTH; //position for next character
        }
        //do not print nextline because current console line is still active.
        if(i == length-1) break;
        xOffset = XOFFSET;
        yOffset += LINEHEIGHT; //on \n, start on new line below
    }

    currentConsolePosition[0] = XOFFSET;
    currentConsolePosition[1] = YOFFSET;
}

void drawFromBuffer(){
    unsigned int length = arrayGetLength(textBuffer);
    if(length > (getHeight()/LINEHEIGHT)-2){
        length = (getHeight()/LINEHEIGHT)-2+currentWindow;
    }
    for(int i = currentWindow; i < length;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            Character c = *(Character *)(arrayGetItem(consoleLine, j));
            drawGlyph(c.value, currentConsolePosition[0], currentConsolePosition[1], c.color);
            currentConsolePosition[0] += FONT_WIDTH; //position for next character
        }
        //do not print nextline because current console line is still active.
        if(i == length-1) break;
        currentConsolePosition[0] = XOFFSET;
        currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
    }
}

void rotateScreen(int rotation){
    clearConsole();
    setRotation(rotation);
    drawFromBuffer();
}

void scaleScreen(int multiplier){
    clearConsole();
    setScaleSize(multiplier);
    drawFromBuffer();
}

void changeTextColor(int color){
    clearConsole();
    CURRENT_COLOR = color;
    drawFromBuffer();
}