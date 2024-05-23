#include "mem.h"

/**
 * Memory-Mapped I/O write, sets memory address to a value
 * @param reg: memory address
 * @param val: value
 */
void mmio_write(unsigned int reg, unsigned int val){
    *(volatile unsigned int *)reg = val;
}

/**
 * Memory-Mapped I/O read, reads the value of a memory address
 * @param reg: memory address
 */
unsigned int mmio_read(unsigned int  reg){
    return *(volatile unsigned int *)reg;
}

void memMove(void *dest, void *src,  unsigned int  n)
{
    // Typecast src and dest addresses to (char *)
    char *csrc = (char *)src;
    char *cdest = (char *)dest;

    // Create a temporary array to hold data of src
    char *temp[n];

    // Copy data from csrc[] to temp[]
    for (int i=0; i<n; i++)
        temp[i] = csrc[i];

    // Copy data from temp[] to cdest[]
    for (int i=0; i<n; i++)
        cdest[i] = temp[i];
}

void *memset(void *s, int c,  unsigned long n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

// Custom implementation of memcpy
void *memcpy(void *dest, const void *src,  unsigned long n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}