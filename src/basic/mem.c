#include "mem.h"

/**
 * Memory-Mapped I/O write, sets memory address to a value
 * @param reg: memory address
 * @param val: value
 */
void mmio_write(long reg, unsigned int val){
    *(volatile unsigned int *)reg = val;
}

/**
 * Memory-Mapped I/O read, reads the value of a memory address
 * @param reg: memory address
 */
unsigned int mmio_read(long reg){
    return *(volatile unsigned int *)reg;
}
