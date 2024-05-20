#include "array.h"
struct queue {   // Structure declaration
    int firstItemIndex;
    Array * array;
};

typedef struct queue Queue;

Queue* newQueue(int elmSize);
void queueDelete(Queue* queue);
void* queuePop(Queue* queue);
void queuePush(Queue* queue, void* item);
int queueGetLenght(Queue* queue);