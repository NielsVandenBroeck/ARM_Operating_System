struct array {   // Structure declaration
    int elmCount;           // Member (int variable)
    int elmSize;
    int lastIndex;
    void* firstItem;       // Member (char variable)
    struct array* nextArray;
};

typedef struct array Array;

Array* newArray(int length, int elmSize);
void arrayDelete(Array* array);
Array* ArrayConcat(Array* array1, Array* array2);
void* arrayGetItem(Array* array, int i);
void arraySetItem(Array* array, int i, void* item);
void arrayRemoveItem(Array* array, int i);
void arrayInsertItem(Array* array, int i, void* item);
void arrayAppendItem(Array* array, void* item);
int arrayGetLength(Array* array);
void arraySimplify();
void arrayShift(Array* array);