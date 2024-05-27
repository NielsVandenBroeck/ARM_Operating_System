#include "../graphicInterface/framebuffer.h"
#include "../basic/error.h"
#include "../basic/mem.h"
#include "console.h"
#include "../command_line_interface/Font.h"
#include "../basic/multicore.h"
#include "../basic/keyboard_interrupts.h"
#include "../data_structures/array.h"
#include "../uart/uart.h"
#include "../programs/commands.h"
#include "../data_structures/string.h"
#include <stddef.h>

//variables related to texbuffer
Array* textBuffer; //Array of arrays of Characters for buffering the text shown on screen
Array* currentLine; //latest line (stored for easier access)
static int currentWindowIndex = 0; //The line (index in textBuffer) that is shown first (on top) of the screen
static int cursorIndex = 0; //index in the currentLine where the cursor is positioned
static int inputStartIndex = 0; //index in the current line that indicates the start of the user input

int currentConsolePosition[2]={XOFFSET,YOFFSET};
int CURRENT_COLOR = white;

void initConsole(){
    CURRENT_COLOR = white;
    setScaleSize(1);
    setRotation(0);
    //Create the array buffer for the displayed text
    textBuffer = newArray(1, sizeof(Array*));
    currentLine = newArray(0, sizeof(Character));
    arraySetItem(textBuffer,0,&currentLine);
}

void runConsole(){
    printText("Welcome to:\n",CURRENT_COLOR);
    printText(" _____                                      _____ \n",CURRENT_COLOR);
    printText("( ___ )                                    ( ___ )\n",CURRENT_COLOR);
    printText(" |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | \n",CURRENT_COLOR);
    printText(" |   |  _    _ _           _                |   | \n",CURRENT_COLOR);
    printText(" |   | | |  | | |         | |               |   | \n",CURRENT_COLOR);
    printText(" |   | | |  | | |__  _   _| |_ _ __  _   _  |   | \n",CURRENT_COLOR);
    printText(" |   | | |  | | '_ \\| | | | __| '_ \\| | | | |   | \n",CURRENT_COLOR);
    printText(" |   | | |__| | |_) | |_| | |_| | | | |_| | |   | \n",CURRENT_COLOR);
    printText(" |   |  \\____/|_.__/ \\__,_|\\__|_| |_|\\__,_| |   | \n",CURRENT_COLOR);
    printText(" |   |                                      |   | \n",CURRENT_COLOR);
    printText(" |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___| \n",CURRENT_COLOR);
    printText("(_____)                                    (_____)\n",CURRENT_COLOR);
    initiatePreText();

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
        handleCommand();
        printChar('\n',CURRENT_COLOR);
        initiatePreText();
    }
    else if(c == '\b'){
        //remove previous character
        if(cursorIndex != inputStartIndex){
            clearLastLine();
            arrayRemoveItem(currentLine, cursorIndex-1);
            currentConsolePosition[0] -= FONT_WIDTH;
            cursorIndex--;
            drawLastLine();
        }
    }
    else if(c == ':'){
        //go left (cursor)
        if(cursorIndex > inputStartIndex){
            currentConsolePosition[0] -= FONT_WIDTH;
            cursorIndex--;
        }
    }
    else if(c == '='){
        if(cursorIndex < arrayGetLength(currentLine)){
            currentConsolePosition[0] += FONT_WIDTH;
            cursorIndex++;
        }
    }
    else{
        //check if cursor is not at the end
        if(cursorIndex < arrayGetLength(currentLine)){
            clearLastLine();
            printChar(c,CURRENT_COLOR);
            drawLastLine();
        }
        else{
            printChar(c,CURRENT_COLOR);
        }
    }
}

void initiatePreText(){
    printText("user@Ubutnu: ", green);
    inputStartIndex = cursorIndex;
}

void handleCommand(){
    //find out where the first space is located
    int spaceIndex = inputStartIndex;
    while(spaceIndex < arrayGetLength(currentLine)){
        Character c = *(Character *)arrayGetItem(currentLine,spaceIndex);
        if(c.value == ' '){
            break;
        }
        spaceIndex++;
    }
    if(spaceIndex-inputStartIndex == 0){
        return;
    }
    char programName[spaceIndex-inputStartIndex+1];
    for(int i = inputStartIndex; i < spaceIndex; i++){
        Character c = *(Character *)arrayGetItem(currentLine,i);
        programName[i-inputStartIndex] = c.value;
    }
    programName[spaceIndex-inputStartIndex] = '\0';
    int paramLength = 0;
    if(arrayGetLength(currentLine)-(spaceIndex+1) > 0){
        paramLength = arrayGetLength(currentLine)-(spaceIndex+1);
    }
    char params[paramLength];
    for(int i = spaceIndex+1; i < arrayGetLength(currentLine); i++){
        Character c = *(Character *)arrayGetItem(currentLine,i);
        params[i-(spaceIndex+1)] = c.value;
    }
    printChar('\n',CURRENT_COLOR);
    //todo change 7 with length of programs
    for(int i = 0; i < 10; i++){
        if(strcmp(programName,programs[i].name) == 0){
            programs[i].function(&params[0]);
            return;
        }
    }
    printText(programName, CURRENT_COLOR);
    printText(": Command not found.", CURRENT_COLOR);
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
    if(c == '\n'){
        if(currentConsolePosition[1] >= getWindowHeight()-LINEHEIGHT*2){
            clearConsole();
            currentWindowIndex += 1;
            currentConsolePosition[0] = XOFFSET;
            drawFromBuffer();
        }
        else{
            currentConsolePosition[0] = XOFFSET;
            currentConsolePosition[1] += LINEHEIGHT; //on \n, start on new line below
        }
        cursorIndex = 0;
        // end this console line and add a new one to textBuffer
        currentLine = newArray(0,sizeof (Character));
        arrayAppendItem(textBuffer,&currentLine);
    }
    else{
        //Check if user exceeds screenwidth when does go to next line
        if(currentConsolePosition[0] > getWindowWidth() - XOFFSET){
            currentConsolePosition[1] += LINEHEIGHT;
            currentConsolePosition[0] = XOFFSET;
        }
        drawGlyph(c,currentConsolePosition[0],currentConsolePosition[1],color);
        currentConsolePosition[0] += FONT_WIDTH; //position for next character

        // add character to the textBuffer
        Character newCharacter = {c, color};
        if(cursorIndex < arrayGetLength(currentLine)){
            arrayInsertItem(currentLine, cursorIndex, &newCharacter);
        }
        else{
            arrayAppendItem(currentLine, &newCharacter);
        }

        if(currentConsolePosition[1] - LINEHEIGHT >= getWindowHeight()-LINEHEIGHT*2){
            currentConsolePosition[1] -= LINEHEIGHT;
            clearConsole();
            currentWindowIndex += 1;
            drawFromBuffer();
        }
        cursorIndex++;
    }

}

void drawCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixelsWindow(currentConsolePosition[0]-1, currentConsolePosition[1]+i, CURRENT_COLOR);
    }
}

void clearCursor(){
    for (int i=-2;i<LINEHEIGHT;i++) {
        drawScaledPixelsWindow(currentConsolePosition[0]-1,  currentConsolePosition[1]+i, black);
    }
}

void clearTextBuffer(){
    for(int i = currentWindowIndex; i < arrayGetLength(textBuffer);  i++){
        arrayDelete(*(Array**)arrayGetItem(textBuffer,i));
    }
    arrayDelete(textBuffer);
    textBuffer = newArray(1, sizeof(Array*));
    currentLine = newArray(0, sizeof(Character));
    arraySetItem(textBuffer,0,&currentLine);
    currentConsolePosition[0] = XOFFSET;
    currentConsolePosition[1] = YOFFSET;
}

void clearLastLine(){
    int xOffset = XOFFSET;
    // -Yoffset if exceeds line (todo check)
    int yOffset = currentConsolePosition[1];
    for(int j = 0; j < arrayGetLength(currentLine);  j++){
        //Check if user exceeds screenwidth when does go to next line
        if(xOffset > getWindowWidth() - XOFFSET){
            yOffset += LINEHEIGHT;
            xOffset = XOFFSET;
        }

        Character c = *(Character *)(arrayGetItem(currentLine, j));
        drawGlyph(c.value, xOffset, yOffset, black);
        xOffset += FONT_WIDTH; //position for next character
    }
}

void drawLastLine(){
    int xOffset = XOFFSET;
    int yOffset = currentConsolePosition[1];
    //last line is kinda special, so do this appart
    for(int j = 0; j < arrayGetLength(currentLine);  j++){
        if(xOffset > getWindowWidth() - XOFFSET){
            yOffset += LINEHEIGHT;
            xOffset = XOFFSET;
        }
        Character c = *(Character *)(arrayGetItem(currentLine, j));
        drawGlyph(c.value, xOffset, yOffset, c.color);
        xOffset += FONT_WIDTH; //position for next character
    }
}

void clearConsole(){
    int xOffset = XOFFSET;
    int yOffset = YOFFSET;

    unsigned int length = arrayGetLength(textBuffer);
    if(length > (getWindowHeight()/LINEHEIGHT)-2+currentWindowIndex){
        length = (getWindowHeight()/LINEHEIGHT)-2+currentWindowIndex;
    }
    for(int i = currentWindowIndex; i < length;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            //Check if user exceeds screenwidth when does go to next line
            if(xOffset > getWindowWidth() - XOFFSET){
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
    if(length > (getWindowHeight()/LINEHEIGHT)-2+currentWindowIndex){
        length = (getWindowHeight()/LINEHEIGHT)-2+currentWindowIndex;
    }
    //draw all lines except for the last one
    for(int i = currentWindowIndex; i < length-1;  i++){
        Array* consoleLine = *(Array**)arrayGetItem(textBuffer,i);
        for(int j = 0; j < arrayGetLength(consoleLine);  j++){
            if(xOffset > getWindowWidth() - XOFFSET){
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
        if(xOffset > getWindowWidth() - XOFFSET){
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
    int newWindow = arrayGetLength(textBuffer) - ((getWindowHeight()/LINEHEIGHT)-2);
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
    CURRENT_COLOR = color;
}