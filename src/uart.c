enum {
    PERIPHERAL_BASE = 0xFE000000,
    GPFSEL0         = PERIPHERAL_BASE + 0x200000,
    GPSET0          = PERIPHERAL_BASE + 0x20001C,
    GPCLR0          = PERIPHERAL_BASE + 0x200028,
    GPPUPPDN0       = PERIPHERAL_BASE + 0x2000E4
};

enum {
    AUX_BASE        = PERIPHERAL_BASE + 0x215000,
    AUX_IRQ         = AUX_BASE,
    AUX_ENABLES     = AUX_BASE + 4,
    AUX_MU_IO_REG   = AUX_BASE + 64,
    AUX_MU_IER_REG  = AUX_BASE + 68,
    AUX_MU_IIR_REG  = AUX_BASE + 72,
    AUX_MU_LCR_REG  = AUX_BASE + 76,
    AUX_MU_MCR_REG  = AUX_BASE + 80,
    AUX_MU_LSR_REG  = AUX_BASE + 84,
    AUX_MU_MSR_REG  = AUX_BASE + 88,
    AUX_MU_SCRATCH  = AUX_BASE + 92,
    AUX_MU_CNTL_REG = AUX_BASE + 96,
    AUX_MU_STAT_REG = AUX_BASE + 100,
    AUX_MU_BAUD_REG = AUX_BASE + 104,
    AUX_UART_CLOCK  = 500000000,
    UART_MAX_QUEUE  = 16 * 1024
};

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

/**
 * Initializes UART by setting all the memory addresses to the correct values
 * @param baud: The baud rate of serial communication
 */
void uart_init(int baud){
    mmio_write(AUX_ENABLES, 1); //Activates UART1
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_CNTL_REG, 0);
    mmio_write(AUX_MU_LCR_REG, 3); //8 bits
    mmio_write(AUX_MU_MCR_REG, 0);
    mmio_write(AUX_MU_IER_REG, 0);
    mmio_write(AUX_MU_IIR_REG, 0xC6); //Disables interups
    mmio_write(AUX_MU_BAUD_REG, (AUX_UART_CLOCK/(baud*8))-1);
    mmio_write(AUX_MU_CNTL_REG, 3); //Activates RX/TX
}

/**
 * Writes a char to the memory address of the UART
 * @param ch: char to be written
 */
void uart_writeByteBlocking(unsigned char ch) {
    while (!(mmio_read(AUX_MU_LSR_REG) & 0x20)); //Waits until ready to send to UART
    mmio_write(AUX_MU_IO_REG, (unsigned int)ch); //Writes
}

/**
 * Prints a string to the UART
 * @param outputString: String to be printed
 */
void uart_print(char *outputString){
    while (*outputString) {
        if (*outputString == '\n') uart_writeByteBlocking('\r');
        uart_writeByteBlocking(*outputString++);
    }
}