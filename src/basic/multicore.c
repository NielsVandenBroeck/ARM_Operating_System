#include "multicore.h"
#include "../icons/icon.h"
#include "../taskbar/taskbar.h"
#include "../uart/uart.h"
#include "wait.h"
#include <stddef.h>

void store32(unsigned long address, unsigned long value)
{
    *(unsigned long *) address = value;
}

unsigned long load32(unsigned long address)
{
    return *(unsigned long *) address;
}

int taskBarIndex[4] = {NULL, NULL, NULL, NULL};
//int icon_core0[ICONSIZE][ICONSIZE] = LOADING_ICON;
//int icon_core1[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//int icon_core2[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//int icon_core3[ICONSIZE][ICONSIZE] = PAUSE_ICON;

void cores_init(){
    TaskBarItem item0 = {"C0 (running):", LOADING_ICON};
    taskBarIndex[0] = addTaskBarItem(item0, LEFT);

    TaskBarItem item1 = {"C1:", PAUSE_ICON};
    taskBarIndex[1] = addTaskBarItem(item1, LEFT);

    TaskBarItem item2 = {"C2:", PAUSE_ICON};
    taskBarIndex[2] = addTaskBarItem(item2, LEFT);

    TaskBarItem item3 = {"C3:", PAUSE_ICON};
    taskBarIndex[3] = addTaskBarItem(item3, LEFT);
    taskBarDraw();
}

void start_core1(void (*func)(void))
{
    TaskBarItem item = {"C1:", LOADING_ICON};
    redrawTaskBarItem(item, LEFT, taskBarIndex[1]);
    taskBarDraw();
    store32((unsigned long)&spin_cpu1, (unsigned long)func);
    asm volatile ("sev");

//    int icon_core1[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//    TaskBarItem item1 = {"C1 (stopped):", icon_core1};
//    redrawTaskBarItem(item1, LEFT, taskBarIndex[1]);
//    taskBarDraw();
}

void (*funcToStart)(void) = NULL;

void startCoreFunction(){
    /*
    TaskBarItem item = {"C2 (running):", LOADING_ICON};
    redrawTaskBarItem(item, LEFT, 2);
    taskBarDraw();*/
    funcToStart();
    for(int i = 0; i < 5; i++){
        uart_print("hello test\n");
        wait_msec(1000);
    }

    /*
    uart_print("\ncore 2 stopped\n");
    TaskBarItem item1 = {"C2 (stopped):", PAUSE_ICON};
    redrawTaskBarItem(item1, LEFT, 2);
    taskBarDraw();*/
    while(1){
        uart_print("core dead test\n");
        wait_msec(1000);
    }
}

void start_core2(void (*func)(void))
{
    funcToStart = func;
    void (*newFunction)(void) = startCoreFunction;
    store32((unsigned long)&spin_cpu2, (unsigned long)newFunction);
    asm volatile ("sev");
}

void start_core3(void (*func)(void))
{
    uart_print("core 3 started");
    TaskBarItem item = {"C3:", LOADING_ICON};
    redrawTaskBarItem(item, LEFT, taskBarIndex[3]);
    taskBarDraw();
    store32((unsigned long)&spin_cpu3, (unsigned long)func);
    asm volatile ("sev");

    int icon_core1[ICONSIZE][ICONSIZE] = PAUSE_ICON;
    TaskBarItem item1 = {"C3:", icon_core1};
    redrawTaskBarItem(item1, LEFT, taskBarIndex[3]);
}

void clear_core1(void)
{
//    int icon_core[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//    TaskBarItem item = {"C3:", icon_core};
//    redrawTaskBarItem(item, LEFT, taskBarIndex[1]);

    store32((unsigned long)&spin_cpu1, 0);
}

void clear_core2(void)
{
//    int icon_core[ICONSIZE][ICONSIZE] = LOADING_ICON;
//    TaskBarItem item = {"C2:", icon_core};
//    redrawTaskBarItem(item, LEFT, taskBarIndex[2]);

    store32((unsigned long)&spin_cpu2, 0);
}

void clear_core3(void)
{
//    int icon_core[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//    TaskBarItem item = {"C3:", icon_core};
//    redrawTaskBarItem(item, LEFT, taskBarIndex[3]);

    store32((unsigned long)&spin_cpu3, 0);
}
