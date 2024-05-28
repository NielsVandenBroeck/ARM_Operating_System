#include "array.h"
#include "malloc.h"
#include "../basic/error.h"
#include "../uart/uart.h"
#include <stddef.h>

Array* newArray(int length, int elmSize){
    void* firstItem = (void*)malloc(length * elmSize);
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

void arraySetItem(Array* array, int i, void* item){
    uart_print("array set\n");
    if(array == NULL || i < 0){
        throw("Index out of range");
        return 0;
    }
    else if(i > array->lastIndex){
        return arraySetItem(array->nextArray, i - array->lastIndex - 1, item);
    }
    void* dest = array->firstItem + (i * array->elmSize);
    memMove(dest, item, array->elmSize);
}

//todo remove array if 1 item
void arrayRemoveItem(Array* array, int i){
    if(array == NULL || i < 0){
        throw("Index out of range");
        return 0;
    }
    if(i > array->lastIndex){
        return arrayRemoveItem(array->nextArray, i - array->lastIndex - 1);
    }

    // Remove the item at index i
    void* base = array->firstItem;
    void* src = base + (i + 1) * array->elmSize;
    void* dest = base + i * array->elmSize;
    unsigned int bytes_to_move = (array->lastIndex - i) * array->elmSize;
    memMove(dest, src, bytes_to_move);
    array->lastIndex--;
}

void arrayInsertItem(Array* array, int i, void* item){
    if(array == NULL || i < 0){
        throw("Index out of range");
        return 0;
    }
    else if(i > array->lastIndex){
        return arrayInsertItem(array->nextArray, i - array->lastIndex - 1, item);
    }
    //room to just move all other elements by 1 and insert item at index
    else if(array->lastIndex < array->elmCount-1){
        void* base = array->firstItem;
        void* src = base + i * array->elmSize;
        void* dest = base + (i+1) * array->elmSize;
        unsigned int bytes_to_move = (array->lastIndex - i+1) * array->elmSize;
        memMove(dest, src, bytes_to_move);
        //put item at right location (src)
        memMove(src,item, array->elmSize);
        array->lastIndex++;
    }
    //no room so make new array
    else{
        Array* newItemArray = newArray(1, array->elmSize);
        //put item in address of firstitem of the array
        memMove(newItemArray->firstItem,item, array->elmSize);
        //split original array in 2 so newItemArray get get in between them.
        Array* secondArray = newArray(array->lastIndex - i+1, array->elmSize);
        void* src = array->firstItem + (i * array->elmSize);
        void* dest =  secondArray->firstItem;
        unsigned int bytes_to_move = (array->lastIndex - i+1) * array->elmSize;
        memMove(dest, src, bytes_to_move);

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
void arrayAppendItem(Array* array, void* item){
    /**
     * Implemented by allocating 10 items on stack, or when there is space available moving the lastindex
     */
    if(array->nextArray != NULL){
        return arrayAppendItem(array->nextArray, item);
    }
    else if(array->lastIndex >= array->elmCount - 1){
        array->nextArray = newArray(10, array->elmSize);
        array->nextArray->lastIndex = -1;
        arrayAppendItem(array->nextArray, item);
        return;
    }
    void* dest = array->firstItem + ((array->lastIndex+1) * array->elmSize);
    memMove(dest, item, array->elmSize);
    array->lastIndex++;
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