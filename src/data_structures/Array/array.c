#include "array.h"
#include "malloc.h"
#include <stddef.h>

Array* newArray(int length, int elmSize){
    void* firstItem = (char *)malloc(length * elmSize);
    Array createdArray = {length, elmSize, length, firstItem, NULL};

    Array* arrayItem = (Array *)malloc(sizeof(Array));
    arrayItem[0] = createdArray;
    return arrayItem;
}

void arrayDelete(Array* array){
    if(array == NULL){
        return;
    }
    free(array->firstItem);
    arrayDelete(array->nextArray);
}

Array* ArrayConcat(Array* array1, Array* array2){
    if(array1->nextArray == NULL){
        array1->nextArray = array2;
        return array1;
    }
    ArrayConcat(array1->nextArray, array2);
    return array1;
}

void* arrayGetItem(Array* array, int i){
    if(i > array->elmCount){
        return arrayGetItem(array->nextArray, i - array->elmCount);
    }
    return array->firstItem + array->elmSize * i;
}

void arrayRemoveItem(Array* array, int i){

}

/**
 * Add one memory space to the array
 * @param array
 */
void arrayAppend(Array* array){
    /**
     * Implemented by allocating 10 items on stack, or when there is space available moving the lastindex
     */
    if(array->nextArray != NULL){
        arrayAppend(array->nextArray);
    }
    if(array->lastIndex >= array->elmCount){
        array->nextArray = newArray(10, array->elmSize);
    }
    else{
        array->lastIndex++;
    }
};

/**
 * Returns reserved size of array
 * @param array
 * @return size of array
 */
int arrayGetLength(Array* array){
    if(array->nextArray == NULL){
        return array->lastIndex;
    }
    return array->elmCount + arrayGetLength(array->nextArray);
}























