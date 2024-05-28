#include "taskbar.h"
#include "../graphicInterface/framebuffer.h"
#include "../data_structures/array.h"
#include "../uart/uart.h"
#include "../command_line_interface/Font.h"
#include "../basic/time.h"
#include <stddef.h>

Array* leftTaskBarItems = NULL;
Array* rightTaskBarItems = NULL;

int xDrawPosLeft = 5;
int xDrawPosRight = 0;

void taskBarInit(){
    leftTaskBarItems = newArray(0,sizeof (TaskBarItem));
    rightTaskBarItems = newArray(0,sizeof (TaskBarItem));
    setYWindowOffsets(TASKBARHEIGHT, TOP);
    taskBarDraw();
}

int addTaskBarItem(TaskBarItem item, leftRight position) {
    if(position == LEFT){
        arrayAppendItem(leftTaskBarItems, &item);
        return arrayGetLength(leftTaskBarItems) - 1;
    }
    else{
        arrayAppendItem(rightTaskBarItems, &item);
        return arrayGetLength(rightTaskBarItems) - 1;
    }
}

void redrawTaskBarItem(TaskBarItem item, leftRight position, int i){
    if(i == NULL){
        return;
    }
    if(position == LEFT){
        arraySetItem(leftTaskBarItems, i, &item);
    }
    else{
        arraySetItem(rightTaskBarItems, i, &item);
    }
}

void drawBackground(){
    for(int x = 0; x < getScreenWidth(); x++){
        for(int y = 0; y < TASKBARHEIGHT; y++){
            drawScaledPixelsScreen(x, y, BACKGROUNDCOLOR);
        }
    }
}

void drawText(char* text, int* xPos){
    char *p = text;
    int i = 0;
    for (char c = *p; c != '\0'; c = *++p)
    {
        drawGlyphScreen(c, *xPos, OFFSET + FONT_HEIGHT/2, black);
        (*xPos) += FONT_WIDTH;
    }
}

void drawIcon(int (*icon)[16], int* xPos){
    for(int x = 0; x < ICONSIZE; x++){
        for(int y = 0; y < ICONSIZE; y++){
            if(icon[x][y] != white){
                drawScaledPixelsScreen((*xPos) + x, y + OFFSET, icon[x][y]);
            }
        }
    }
    (*xPos) += ICONSIZE + OFFSET;
}

void drawVerticalLine(int* xPos){
    for(int x = 0; x < 1; x++){
        for(int y = OFFSET; y < ICONSIZE + OFFSET; y++){
            drawScaledPixelsScreen((*xPos) + x, y, black);
        }
    }
    (*xPos) += ICONSIZE + OFFSET;
}

void drawTaskBarItems(Array* items){
    int size = arrayGetLength(items);
    uart_print("array lenght:");
    uart_printInt(size);

    for(int i = 0; i < size; i++){
        TaskBarItem item = (*(TaskBarItem*)arrayGetItem(items,i));
        int* drawPosition = &xDrawPosLeft;
        drawText(item.text, drawPosition);
        //drawIcon(item.icon, drawPosition);
        (*drawPosition) += 5;
        drawVerticalLine(drawPosition);
    }
}

int taskBarLockLock = 0;
void taskBarDraw(){
    while(taskBarLockLock){
        wait_msec(500);
    }
    taskBarLockLock = 1;
    xDrawPosLeft = 5;
    xDrawPosRight = 0;
    drawBackground();
    drawTaskBarItems(leftTaskBarItems);
    taskBarLockLock = 0;
}

