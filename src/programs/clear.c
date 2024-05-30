#include "clear.h"
#include "../command_line_interface/console.h"
#include "../graphicInterface/framebuffer.h"
#include "../taskbar/taskbar.h"

void clear(char* params){
    drawScreen(black);
    taskBarDraw();
    drawFromBuffer();
    printText("Cleared screen.", CURRENT_COLOR);
}