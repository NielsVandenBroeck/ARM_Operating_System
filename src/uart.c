enum
{
    GPIO_BASE = 0x3F200000, //The GPIO registers base address. for raspi2 & 3, 0x20200000 for raspi1
    GPPUD = (GPIO_BASE + 0x94),
    GPPUDCLK0 = (GPIO_BASE + 0x98),

    UART0_BASE = 0x3F201000, //The base address for UART. for raspi2 & 3, 0x20201000 for raspi1
    UART0_DR     = (UART0_BASE + 0x00),
    UART0_RSRECR = (UART0_BASE + 0x04),
    UART0_FR     = (UART0_BASE + 0x18),
    UART0_ILPR   = (UART0_BASE + 0x20),
    UART0_IBRD   = (UART0_BASE + 0x24),
    UART0_FBRD   = (UART0_BASE + 0x28),
    UART0_LCRH   = (UART0_BASE + 0x2C),
    UART0_CR     = (UART0_BASE + 0x30),
    UART0_IFLS   = (UART0_BASE + 0x34),
    UART0_IMSC   = (UART0_BASE + 0x38),
    UART0_RIS    = (UART0_BASE + 0x3C),
    UART0_MIS    = (UART0_BASE + 0x40),
    UART0_ICR    = (UART0_BASE + 0x44),
    UART0_DMACR  = (UART0_BASE + 0x48),
    UART0_ITCR   = (UART0_BASE + 0x80),
    UART0_ITIP   = (UART0_BASE + 0x84),
    UART0_ITOP   = (UART0_BASE + 0x88),
    UART0_TDR    = (UART0_BASE + 0x8C),
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
    mmio_write(UART0_CR, 0x00000000);
    mmio_write(GPPUD, 0x00000000);
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    mmio_write(GPPUDCLK0, 0x00000000);
    mmio_write(UART0_ICR, 0x7FF);
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |(1 << 7) | (1 << 8) | (1 << 9) | (1 << 10));
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
}

/**
 * Writes a char to the memory address of the UART
 * @param ch: char to be written
 */
void uart_writeByteBlocking(unsigned char ch) {
    while (!(mmio_read(UART0_FR) & (1 << 4))); //Waits until ready to send to UART
    mmio_write(UART0_DR, (unsigned int)ch); //Writes
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