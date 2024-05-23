#ifndef __TASKBAR_H_
#define __TASKBAR_H_

#include "../data_structures/enums.h"

#define TASKBARHEIGHT 20
#define OFFSET 2
#define ICONSIZE TASKBARHEIGHT - 2 * OFFSET
#define BACKGROUNDCOLOR 0xa6a6a6


struct taskBarItem{
    char* text;
    int icon[16][16]
};

typedef struct taskBarItem TaskBarItem;

/**
 *
 * @param item
 * @param position
 * @return index of placed item
 */
int addTaskBarItem(TaskBarItem item, leftRight position);

/**
 * Pass index of iteù
 * @param item
 * @param position
 * @param i
 */
void redrawTaskBarItem(TaskBarItem item, leftRight position, int i);

void taskBarDraw();

void taskBarInit();
#endif