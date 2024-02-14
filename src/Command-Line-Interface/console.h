enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};

//Communication over serial port, used for testing.
void console_init();

void console_run();

void console_print(char *outputString);

void console_printline(char *outputString);

void console_print_int(unsigned int number);

void console_printc(char c);

void console_nextline();

char* console_readline();

void console_clear();

void console_color(int newColor);

//x and y will be set to the new current position
void drawString(int* x, int* y, char *s, int color);

void drawCursor(int*x, int* y, int color);