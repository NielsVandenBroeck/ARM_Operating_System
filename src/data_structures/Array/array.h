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
void arrayRemoveItem(Array* array, int i);
void arrayAppend(Array* array);
int arrayGetLength(Array* array);
void Simplify();