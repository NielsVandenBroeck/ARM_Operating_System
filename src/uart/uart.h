//Communication over serial port, used for testing.
void uart_init();

void uart_print(char *outputString);

void uart_printc(char c);

char* uart_readline();

char* uart_readchar();