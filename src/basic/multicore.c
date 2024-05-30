#include "multicore.h"
#include "../icons/icon.h"
#include "../taskbar/taskbar.h"
#include "../uart/uart.h"
#include "time.h"
#include <stddef.h>

void store32(unsigned long address, unsigned long value)
{
    *(unsigned long *) address = value;
}

unsigned long load32(unsigned long address)
{
    return *(unsigned long *) address;
}

//int icon_core0[ICONSIZE][ICONSIZE] = LOADING_ICON;
//int icon_core1[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//int icon_core2[ICONSIZE][ICONSIZE] = PAUSE_ICON;
//int icon_core3[ICONSIZE][ICONSIZE] = PAUSE_ICON;

int taskBarIndex[4] = {NULL, NULL, NULL, NULL};

void (*core1Function)(void) = NULL;
void (*core2Function)(void) = NULL;
void (*core3Function)(void) = NULL;

void startCore(int coreNumber, char* coreName){
    TaskBarItem item = {coreName};
    redrawTaskBarItem(item, LEFT, coreNumber);
    taskBarDraw();
}

void endCore(int coreNumber, char* coreName){
    TaskBarItem item1 = {coreName};
    redrawTaskBarItem(item1, LEFT, coreNumber);
    while(1){
        wait_msec(5000);
    }
}

void startCore1Function(){
    startCore(1, "C1 (running):");
    core1Function();
    for(int i = 0; i < 5; i++){
        uart_print("hello test\n");
        wait_msec(1000);
    }
    endCore(1, "C1 (stopped):");
}

void startCore2Function(){
    startCore(2, "C2 (running):");
    core2Function();
    for(int i = 0; i < 5; i++){
        uart_print("hello test\n");
        wait_msec(1000);
    }
    endCore(2, "C2 (stopped):");
}

void startCore3Function(){
    //startCore(3, "C3 (running):");
    uart_print("C3 starting\n");
    core3Function();
    uart_print("C3 done\n");
    for(int i = 0; i < 5; i++){
        uart_print("hello test\n");
        wait_msec(1000);
    }
    while (1){
        uart_print("stopped c3\n");
        wait_msec(1000);
    }
    //endCore(3, "C3 (stopped):");
}

void cores_init(){

    TaskBarItem item0 = {"C0 (running):"};
    taskBarIndex[0] = addTaskBarItem(item0, LEFT);

    TaskBarItem item1 = {"C1 (stopped):"};
    taskBarIndex[1] = addTaskBarItem(item1, LEFT);

    TaskBarItem item2 = {"C2 (stopped):"};
    taskBarIndex[2] = addTaskBarItem(item2, LEFT);

    TaskBarItem item3 = {"C3 (stopped):"};
    taskBarIndex[3] = addTaskBarItem(item3, LEFT);
    taskBarDraw();
}

void start_core1(void (*func)(void))
{
    uart_print("start_core1\n");
    core1Function = func;
    void (*newFunction)(void) = startCore1Function;
    store32((unsigned long)&spin_cpu1, (unsigned long)newFunction);
    asm volatile ("sev");
}

void start_core2(void (*func)(void))
{
    uart_print("start_core2\n");
    core2Function = func;
    void (*newFunction)(void) = startCore2Function;
    store32((unsigned long)&spin_cpu2, (unsigned long)newFunction);
    asm volatile ("sev");
}

void start_core3(void (*func)(void))
{
    uart_print("start_core3\n");
    core3Function = func;
    void (*newFunction)(void) = startCore3Function;
    store32((unsigned long)&spin_cpu3, (unsigned long)newFunction);
    asm volatile ("sev");
}

void clear_core1(void)
{
    store32((unsigned long)&spin_cpu1, 0);
}

void clear_core2(void)
{
    store32((unsigned long)&spin_cpu2, 0);
}

void clear_core3(void)
{
    store32((unsigned long)&spin_cpu3, 0);
}
