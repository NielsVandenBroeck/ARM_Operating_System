#include "array.h"
#include "malloc.h"
#include "../basic/error.h"
#include "../graphicInterface/framebuffer.h"
#include "../Command-Line-Interface/console.h"
#include <stddef.h>

Array* newArray(int length, int elmSize){
    void* firstItem = (char *)malloc(length * elmSize);
    Array createdArray = {length, elmSize, length-1, firstItem, NULL};
    Array* arrayItem = (Array *)malloc(sizeof(Array));
    arrayItem[0] = createdArray;
    return arrayItem;
}

//TODO delete array item
void arrayDelete(Array* array){
    if(array == NULL){
        return;
    }
    free(array->firstItem);
    arrayDelete(array->nextArray);
}

Array* ArrayConcat(Array* array1, Array* array2){
    if(array1->lastIndex <= array1->elmCount - 1){
        array1->elmCount = array1->lastIndex;
    }
    if(array1->nextArray == NULL){
        array1->nextArray = array2;
        return array1;
    }
    ArrayConcat(array1->nextArray, array2);
    return array1;
}

void* arrayGetItem(Array* array, int i){
    if(array == NULL){
        throw("Index out of range");
        return 0;
    }
    if(i > array->lastIndex){
        return arrayGetItem(array->nextArray, i - array->lastIndex - 1);
    }
    return array->firstItem + array->elmSize * i;
}

void* arrayRemoveItem(Array* array, int i){
    if(array == NULL || i < 0){
        throw("Index out of range");
        return 0;
    }
    if(i > array->lastIndex){
        return arrayRemoveItem(array->nextArray, i - array->lastIndex - 1);
    }

    // Remove the item at index i
    char* base = (char*)array->firstItem;
    void* src = base + (i + 1) * array->elmSize;
    void* dest = base + i * array->elmSize;
    unsigned int bytes_to_move = (array->lastIndex - i) * array->elmSize;
    memMove(dest, src, bytes_to_move);
    array->lastIndex--;
}

void* arrayInsertItem(Array* array, int i, Character item){
    if(array == NULL || i < 0){
        throw("Index out of range");
        return 0;
    }
    else if(i > array->lastIndex){
        return arrayInsertItem(array->nextArray, i - array->lastIndex - 1, item);
    }
    //room to just move all other elements by 1 and insert item at index
    else if(array->lastIndex < array->elmCount-1){
        char* base = (Character*)array->firstItem;
        void* src = base + i * array->elmSize;
        void* dest = base + i+1 * array->elmSize;
        unsigned int bytes_to_move = (array->lastIndex - i+1) * array->elmSize;
        memMove(dest, src, bytes_to_move);
        *(Character*)arrayGetItem(array, i) = item;
        array->lastIndex++;
    }
    //no room so make new array
    else{
        Array* newItemArray = newArray(1, array->elmSize);
        *(Character*)arrayGetItem(newItemArray, 0) = item;
        //split original array in 2 so newItemArray get get in between them.
        Array* secondArray = newArray(array->lastIndex - i+1, array->elmSize);
        void* src = (Character*)array->firstItem + (i * array->elmSize);
        void* dest =  (Character*)secondArray->firstItem;
        unsigned int bytes_to_move = (array->lastIndex - i+1) * array->elmSize;
        //todo original array should be shortened? idk how that works with the free()
        array->lastIndex = i-1;
        //link all arrays together so they become one array
        newItemArray->nextArray = secondArray;
        secondArray->nextArray = array->nextArray;
        array->nextArray = newItemArray;
    }


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
    else if(array->lastIndex >= array->elmCount - 1){
        array->nextArray = newArray(10, array->elmSize);
        array->nextArray->lastIndex = 0;
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
        return array->lastIndex+1;
    }
    return array->lastIndex+1 + arrayGetLength(array->nextArray);
}