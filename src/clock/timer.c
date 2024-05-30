#include "timer.h"
#include "../taskbar/taskbar.h"
#include "../basic/time.h"
#include "../uart/uart.h"


void initTimer(){
    TaskBarItem timer = {"00:00"};
    int index = addTaskBarItem(timer, LEFT);
    taskBarDraw();
    int prevTotalSeconds = 0;
    while(1){
        wait_msec(200);
        char text[6];
        int uptime = uptime_msec();
        int totalSeconds = uptime / 1000;
        if(totalSeconds == prevTotalSeconds){
            continue;
        }
        prevTotalSeconds = totalSeconds;
        int minutes = totalSeconds / 60;
        int seconds = totalSeconds % 60;
        text[0] = '0' + (minutes / 10); // Tens place
        text[1] = '0' + (minutes % 10); // Units place
        text[2] = ':';
        text[3] = '0' + (seconds / 10); // Tens place
        text[4] = '0' + (seconds % 10); // Units place
        text[5] = '\0';
        TaskBarItem timer1 = {text};
        redrawTaskBarItem(timer1,LEFT,index);
    }
}