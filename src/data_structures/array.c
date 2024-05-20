#include "array.h"
#include "malloc.h"
#include "../basic/error.h"
#include "../graphicInterface/framebuffer.h"
#include "../Command-Line-Interface/console.h"
#include <stddef.h>

Array* newArray(int length, int elmSize){
    printInt(length * elmSize,red);
    printChar(' ',green);
    void* firstItem = (char *)malloc(length * elmSize);
    if(firstItem == NULL){
        drawGlyph('z',red);
    }
    Array createdArray = {length, elmSize, length-1, firstItem, NULL};
    printInt(sizeof(Array),red);
    Array* arrayItem = (Array *)malloc(sizeof(Array));
    if(arrayItem == NULL){
        drawGlyph('z',red);
    }
    arrayItem[0] = createdArray;
    drawGlyph('a',blue);
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
        return 0;
    }
    if(i > array->lastIndex){
        return arrayGetItem(array->nextArray, i - array->lastIndex - 1);
    }
    return array->firstItem + array->elmSize * i;
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
        array->lastIndex+=1;
    }
    drawGlyph('b',blue);
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























