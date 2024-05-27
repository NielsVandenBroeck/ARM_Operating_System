#include "echo.h"
#include "../command_line_interface/console.h"

void echo(char* params){
    printText(params, CURRENT_COLOR);
}