//Communication over serial port, used for testing.
void uart_init(int baud);

void uart_print(char *outputString);

void uart_printc(char c);

char* uart_readline();
