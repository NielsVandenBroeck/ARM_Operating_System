#include "taskbar.h"
#include "../graphicInterface/framebuffer.h"
#include "../data_structures/array.h"
#include "../uart/uart.h"
#include "../command_line_interface/Font.h"
#include "../basic/time.h"
#include <stddef.h>

Array* leftTaskBarItems = NULL;
Array* rightTaskBarItems = NULL;

struct taskBarItemLocator{
    int leftLocation;
    int length;
};

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

    int oldStartPossition = (*(TaskBarItem*)arrayGetItem(leftTaskBarItems, i)).startPossition;
    char* oldText = (*(TaskBarItem*)arrayGetItem(leftTaskBarItems, i)).text;
    int oldStartPossitionCopy = oldStartPossition;
    drawText(oldText, &oldStartPossitionCopy, BACKGROUNDCOLOR);
    uart_print("old text:");
    uart_print(oldText);
    uart_printc('\n');
    uart_print("new text:");
    uart_print(item.text);
    uart_printc('\n');
    item.startPossition = oldStartPossition;
    if(position == LEFT){
        arraySetItem(leftTaskBarItems, i, &item);
    }
    else{
        arraySetItem(rightTaskBarItems, i, &item);
    }

    int newoldpos = oldStartPossition;
    int* newoldStartPossitionCopy = &newoldpos;
    ((TaskBarItem*)arrayGetItem(leftTaskBarItems,i))->startPossition = oldStartPossition;
    oldStartPossition = oldStartPossitionCopy;
    drawText(item.text, newoldStartPossitionCopy, black);
    //drawIcon(item.icon, drawPosition);
    (*newoldStartPossitionCopy) += 5;
    drawVerticalLine(newoldStartPossitionCopy);

}

void drawBackground(){
    for(int x = 0; x < getScreenWidth(); x++){
        for(int y = 0; y < TASKBARHEIGHT; y++){
            drawScaledPixelsScreen(x, y, BACKGROUNDCOLOR);
        }
    }
}

void drawText(char* text, int* xPos, int color){
    char *p = text;
    int i = 0;
    for (char c = *p; c != '\0'; c = *++p)
    {
        drawGlyphScreen(c, *xPos, OFFSET + FONT_HEIGHT/2, color);
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
        ((TaskBarItem*)arrayGetItem(items,i))->startPossition = *drawPosition;
        drawText(item.text, drawPosition, black);
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

void taskBarClear(){
    for(int x = 0; x < getScreenWidth(); x++){
        for(int y = 0; y < TASKBARHEIGHT; y++){
            drawScaledPixelsScreen(x, y, black);
        }
    }
}

