enum{
    XOFFSET = 5,
    YOFFSET = 5,
    LINEHEIGHT = 10
};

//Communication over serial port, used for testing.
void console_init();

void console_run();

void console_print(char *outputString);

void console_println(char *outputString);

void console_printc(char c);

char* console_readline();

void console_color(int newColor);