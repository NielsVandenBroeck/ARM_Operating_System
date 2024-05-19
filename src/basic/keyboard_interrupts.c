#include "../graphicInterface/framebuffer.h"
#include "../Command-Line-Interface/console.h"

void onKeyPress(char key){
    processChar(key);
}


void KeyboardInterruptionHandler(){
    while (1){
        wait_msec(1855);
        onKeyPress('z');
        onKeyPress('\n');
    }

}