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
