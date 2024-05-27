#include "scale.h"
#include "../command_line_interface/console.h"

void scale(char* params){
    //todo check valid params
    int multiplication = params[0] - '0';
    if(multiplication < 1 || multiplication > 5){
        printText("Invalid parameter, Multiplication must be an integer between 1 and 5\n", CURRENT_COLOR);
        return;
    }
    scaleScreen(multiplication);
    printText("Screen scaled with multiplication: ", CURRENT_COLOR);
    printInt(multiplication,CURRENT_COLOR);
}