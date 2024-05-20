#include "mem.h"
#include <stddef.h>
#include <stdint.h>

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

//todo fix value colors
void* memset(void* dest, int value, unsigned long count) {
    unsigned char* ptr = dest;

    // Set initial bytes until ptr is aligned to a larger boundary
    while (count && ((uintptr_t)ptr % sizeof(uint64_t)) != 0) {
        *ptr++ = (unsigned char)value;
        count--;
    }

    // Set large chunks at once
    uint64_t large_value = (unsigned char)value;
    large_value |= large_value << 8;
    large_value |= large_value << 16;
    large_value |= large_value << 32;

    uint64_t* large_ptr = (uint64_t*)ptr;
    while (count >= sizeof(uint64_t)) {
        *large_ptr++ = large_value;
        count -= sizeof(uint64_t);
    }

    // Set any remaining bytes
    ptr = (unsigned char*)large_ptr;
    while (count-- > 0) {
        *ptr++ = (unsigned char)value;
    }

    return dest;
}