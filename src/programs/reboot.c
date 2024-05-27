#include "reboot.h"
#include "../command_line_interface/console.h"

void reset_system(void);

void reboot(char* params){
    printText("Rebooting..", CURRENT_COLOR);
    reset_system();
}