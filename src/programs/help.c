#include "help.h"
#include "../command_line_interface/console.h"
#include "commands.h"

void help(char* params){
    printText("All possible commands:", CURRENT_COLOR);
    for(int i = 0; i < programsLength; i++){
        printChar('\n', CURRENT_COLOR);
        printText(programs[i].name, CURRENT_COLOR);
    }
}