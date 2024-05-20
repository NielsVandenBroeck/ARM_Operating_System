typedef void (*observerFunct)(char);

/**
 * By default is the observer set to place text into buffer this can be read using the getChar function
 */
void keyboardInterruptionInit();

/**
 * set the input function, this can be set to uart, textinput, netwerk input etc
 * @param inputFunction
 */
void keyboardInterruptionHandler(char (*inputFunction)());

/**
 * Change the observer (only one observer can be set) when changed the text will not be placed in the text buffer!
 * @param observer
 */
void keyboardInterruptionAttach(observerFunct observer);

/**
 * Create a virtual char input
 * @param interruptChart
 */
void keyboardInterruptionNotify(char interruptChart);

/**
 * Get the oldest added char from the buffer (only works when observer is set) when buffer is empty returns 0
 * @return
 */
char* keyboardInterruptionGetChar();