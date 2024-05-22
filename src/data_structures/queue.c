#include "queue.h"
#include "malloc.h"

/**
 * Create a new Queue
 * @param elmSize
 * @return
 */
Queue* newQueue(int elmSize){
    Array* queueArray = newArray(0, elmSize);
    Queue createdArray = {0, queueArray};
    Queue* queueItem = (Queue *)malloc(sizeof(Queue));
    queueItem[0] = createdArray;
    return queueItem;
}

/**
 * Delete the queue
 * @param queue
 */
void queueDelete(Queue* queue){
    arrayDelete(queue->array);
    free(queue);
}

/**
 * Returns item on index 0 and moves removes removes first item
 * @param array
 * @return item on index 0
 */
void* queuePop(Queue* queue){
    if(queue == NULL){
        throw("empty queue");
        return NULL;
    }
    if(queue->firstItemIndex > queue->array->lastIndex && queue->array->nextArray != NULL){
        free(queue->array->firstItem);//When nothing stored in array remove space
        queue->array = queue->array->nextArray;
        queue->firstItemIndex = 0;
        return queuePop(queue);
    }
    queue->firstItemIndex++;
    return arrayGetItem(queue->array, queue->firstItemIndex - 1);
}

/**
 * Returns item on index 0 and moves removes removes first item (only works on basic type)
 * @param array
 * @return item on index 0
 */
void queuePush(Queue* queue, void* item){
    if(queue->array == NULL){
        throw("Queue does not contain an array");
        return NULL;
    }
    arrayAppendItem(queue->array, &item);
}

int queueGetLenght(Queue* queue){
    return (arrayGetLength(queue->array) - queue->firstItemIndex);
}