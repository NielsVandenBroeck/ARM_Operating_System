#include "kernel_components/uart.c"

void main(uint32_t r0, uint32_t r1, uint32_t atags)
{
    (void) r0;
    (void) r1;
    (void) atags;

    initializeUart()

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}