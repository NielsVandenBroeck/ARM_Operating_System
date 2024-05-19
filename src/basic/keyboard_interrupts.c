#include "keyboard_interrupts.h"

observerFunct currentObserverFunction;

void onKeyPress(char key){
    processChar(key);
}


void KeyboardInterruptionHandler(char (*inputFunction)()){
    while (1){
        char test = inputFunction();
        Notify(test);
    }
}

void Attach(observerFunct observer){
    currentObserverFunction = observer;
}

void Notify(char interruptChart){
    currentObserverFunction(interruptChart);
}

