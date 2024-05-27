#include "rotate.h"
#include "../command_line_interface/console.h"

void rotate(char* params){
    int rotation = params[0] - '0';
    if(rotation < 0 || rotation > 4){
        printText("Invalid parameter, orientation must be an integer between 0 and 4\n", CURRENT_COLOR);
        return;
    }
    rotateScreen(rotation);
    printText("Screen rotated to orientation: ", CURRENT_COLOR);
    printInt(rotation,CURRENT_COLOR);
}