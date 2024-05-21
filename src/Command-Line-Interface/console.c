#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../Command-Line-Interface/Font.h"
#include "../basic/multicore.h"
#include "../basic/keyboard_interrupts.h"
#include "../data_structures/array.h"
#include "../uart/uart.h"
#include <stddef.h>

//variables related to pixels and positions
static int currentConsolePosition[]={XOFFSET,YOFFSET}; //The position to draw the next character
static int CURRENT_COLOR = red; //The color of the text to be drawn

//variables related to texbuffer
Array* textBuffer; //Array of arrays of Characters for buffering the text shown on screen
Array* currentLine; //latest line (stored for easier access)
static int currentWindowIndex = 0; //The line (index in textBuffer) that is shown first (on top) of the screen
static int cursorIndex = 0; //index in the currentLine where the cursor is positioned
static int inputStartIndex = 0; //index in the current line that indicates the start of the user input

void initConsole(){
    setScaleSize(2);
    setRotation(0);
    //Create the array buffer for the displayed text
    textBuffer = newArray(1, sizeof(Array*));
    currentLine = newArray(0, sizeof(Character));
    *(Array**)arrayGetItem(textBuffer, 0) = currentLine;

    //keyboardInterruptionAttach(processChar); //not needed will self check in runConsole, to limit load on input core
}

void runConsole(){
    printText("user@Ubutnu:/home$", green);
    inputStartIndex = 18;

    int cursorCounter = 0;
    uart_print("runConsole\n");
    while(1){
        char* inputChar = keyboardInterruptionGetChar();
        while (inputChar != NULL){
            clearCursor();
            processChar(*inputChar);
            drawCursor();
            inputChar = keyboardInterruptionGetChar();
        }
        cursorCounter++;
        if(cursorCounter == 10){
            drawCursor();
        }
        else if(cursorCounter == 20){
            clearCursor();
            cursorCounter = 0;
        }
        wait_msec(50);
    }
}

void processChar(char c){
    //special cases
    if(c == '\n'){
        if(currentConsolePosition[1] >= getHeight()-LINEHEIGHT*2){
            clearConsole();
            currentWindowIndex += 1;
            currentConsolePosition[0] = XOFFSET;
            drawFromBuffer();
        }
        else{
            currentConsolePosition[0] = XOFFSET;
            currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
        }

        // end this console line and add a new one to textBuffer
        arrayAppend(textBuffer);
        currentLine = newArray(0,sizeof (Character));
        *(Array**)arrayGetItem(textBuffer,arrayGetLength(textBuffer)-1) = currentLine;
    }
    else if(c == '\b'){
        //remove previous character
        if(cursorIndex != inputStartIndex){
            clearConsole();
            arrayRemoveItem(currentLine, cursorIndex-1);
            currentConsolePosition[0] -= FONT_WIDTH;
            cursorIndex--;
            drawFromBuffer();
        }
    }
    else if(c == 'a'){
        //go left (cursor)
        if(cursorIndex > inputStartIndex){
            currentConsolePosition[0] -= FONT_WIDTH;
            cursorIndex--;
        }
    }
    else if(c == 'z'){
        if(cursorIndex < arrayGetLength(currentLine)){
            currentConsolePosition[0] += FONT_WIDTH;
            cursorIndex++;
        }
    }
    else if(c == 'e'){
        rotateScreen(3);
    }
    else if(c == 'r'){
        rotateScreen(0);
    }
    else{
        printChar(c,CURRENT_COLOR);
    }
}

void handleCommand(){

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
    //todo \n should still work here if done in printText()
    //Check if user exceeds screenwidth when does go to next line
    if(currentConsolePosition[0] > getWidth() - XOFFSET){
        currentConsolePosition[1] += LINEHEIGHT;
        currentConsolePosition[0] = XOFFSET;
    }
    drawGlyph(c,currentConsolePosition[0],currentConsolePosition[1],color);
    currentConsolePosition[0] += FONT_WIDTH; //position for next character

    // add character to the textBuffer
    arrayAppend(currentLine);
    Character newCharacter = {c, color};
    *(Character *)arrayGetItem(currentLine,arrayGetLength(currentLine)-1) = newCharacter;

    if(currentConsolePosition[1] - LINEHEIGHT >= getHeight()-LINEHEIGHT*2){
        currentConsolePosition[1] -= LINEHEIGHT;
        clearConsole();
        currentWindowIndex += 1;
        drawFromBuffer();
    }
    cursorIndex++;
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

void drawCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixels(currentConsolePosition[0]-1, currentConsolePosition[1]+i, CURRENT_COLOR);
    }
}

void clearCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixels(currentConsolePosition[0]-1,  currentConsolePosition[1]+i, black);
    }
}

void clearConsole(){
    int xOffset = XOFFSET;
    int yOffset = YOFFSET;

    unsigned int length = arrayGetLength(textBuffer);
    if(length > (getHeight()/LINEHEIGHT)-2+currentWindowIndex){
        length = (getHeight()/LINEHEIGHT)-2+currentWindowIndex;
    }
    for(int i = currentWindowIndex; i < length;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            //Check if user exceeds screenwidth when does go to next line
            if(xOffset > getWidth() - XOFFSET){
                yOffset += LINEHEIGHT;
                xOffset = XOFFSET;
            }

            Character c = *(Character *)(arrayGetItem(consoleLine, j));
            drawGlyph(c.value, xOffset, yOffset, black);
            xOffset += FONT_WIDTH; //position for next character
        }
        xOffset = XOFFSET;
        yOffset += LINEHEIGHT; //on \n, start on new line below
    }
}

void drawFromBuffer(){
    unsigned int length = arrayGetLength(textBuffer);
    int xOffset = XOFFSET;
    int yOffset = YOFFSET;
    if(length > (getHeight()/LINEHEIGHT)-2+currentWindowIndex){
        length = (getHeight()/LINEHEIGHT)-2+currentWindowIndex;
    }
    //draw all lines except for the last one
    for(int i = currentWindowIndex; i < length-1;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            if(xOffset > getWidth() - XOFFSET){
                yOffset += LINEHEIGHT;
                xOffset = XOFFSET;
            }
            Character c = *(Character *)(arrayGetItem(consoleLine, j));
            drawGlyph(c.value, xOffset, yOffset, c.color);
            xOffset += FONT_WIDTH; //position for next character
        }
        xOffset = XOFFSET;
        yOffset += LINEHEIGHT; //on \n, start on new line below
    }
    //last line is kinda special, so do this appart
    for(int j = 0; j < arrayGetLength(currentLine);  j++){
        if(j == cursorIndex){
            currentConsolePosition[0] = xOffset;
            currentConsolePosition[1] = yOffset;
        }
        if(xOffset > getWidth() - XOFFSET){
            yOffset += LINEHEIGHT;
            xOffset = XOFFSET;
        }
        Character c = *(Character *)(arrayGetItem(currentLine, j));
        drawGlyph(c.value, xOffset, yOffset, c.color);
        xOffset += FONT_WIDTH; //position for next character
    }
}

void rotateScreen(int rotation){
    clearConsole();
    setRotation(rotation);
    //edit current window for resized height
    int newWindow = arrayGetLength(textBuffer) - ((getHeight()/LINEHEIGHT)-2);
    if(newWindow > 0){
        currentWindowIndex = newWindow;
    }
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