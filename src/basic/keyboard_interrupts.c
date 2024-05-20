#include "keyboard_interrupts.h"
#include "../data_structures/queue.h"
#include "../uart/uart.h"
#include <stddef.h>

observerFunct currentObserverFunction = NULL;
static Queue* charQueue;

void KeyboardInterruptionInit(){
    charQueue = newQueue(sizeof(char));
}

char bufferChar(char interruptChart){
    queuePush(charQueue, interruptChart);
}

void KeyboardInterruptionHandler(char (*inputFunction)()){
    while (1){
        char newChar = inputFunction();
        keyboardInterruptionNotify(newChar);
    }
}

void keyboardInterruptionAttach(observerFunct observer){
    currentObserverFunction = observer;
}

void keyboardInterruptionNotify(char interruptChart){
    if(currentObserverFunction == NULL){
        bufferChar(interruptChart);
    } else{
        currentObserverFunction(interruptChart);
    }
}

char* keyboardInterruptionGetChar(){
    if(queueGetLenght(charQueue) > 0){
        return (char*)queuePop(charQueue);
    }
    return NULL;
}