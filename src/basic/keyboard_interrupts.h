typedef void (*observerFunct)(char);

void KeyboardInterruptionHandler(char (*inputFunction)());

void Attach(observerFunct observer);

void Notify(char interruptChart);