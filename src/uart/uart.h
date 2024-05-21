//Communication over serial port, used for testing.
void uart_init();

void uart_print(char *outputString);

void uart_printc(char c);

void uart_printInt(unsigned int number);

char* uart_readline();

char* uart_readchar();