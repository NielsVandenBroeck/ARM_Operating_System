#include "timer.h"
#include "../taskbar/taskbar.h"
#include "../basic/time.h"


void initTimer(){
    TaskBarItem timer = {"00:00"};
    int index = addTaskBarItem(timer, LEFT);
    taskBarDraw();
    while(1){
        wait_msec(00);

        char text[5];
        int uptime = uptime_msec();

        int total_seconds = uptime / 1000;
        int minutes = total_seconds / 60;
        int seconds = total_seconds % 60;
        text[0] = '0' + (minutes / 10); // Tens place
        text[1] = '0' + (minutes % 10); // Units place
        text[2] = ':';
        text[3] = '0' + (seconds / 10); // Tens place
        text[4] = '0' + (seconds % 10); // Units place
        TaskBarItem timer1 = {text};

        redrawTaskBarItem(timer1,LEFT,index);
    }
}