#include "color.h"
#include "../command_line_interface/console.h"
#include "../data_structures/string.h"
#include "../graphicInterface/framebuffer.h"

void color(char* params){
    if (strcmp(params, "black") == 0) {
        changeTextColor(black);
    } else if (strcmp(params, "blue") == 0) {
        changeTextColor(blue);
    } else if (strcmp(params, "red") == 0) {
        changeTextColor(red);
    } else if (strcmp(params, "green") == 0) {
        changeTextColor(green);
    } else if (strcmp(params, "white") == 0) {
        changeTextColor(white);
    } else {
        printText("Unknown color: ", CURRENT_COLOR);
        printText(params, CURRENT_COLOR);
        return;
    }
    printText("Color of text is set to ", CURRENT_COLOR);
    printText(params, CURRENT_COLOR);
}