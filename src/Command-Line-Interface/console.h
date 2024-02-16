enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};

//Communication over serial port, used for testing.
void initConsole();

void runConsole();

void nextLine();

char* readLine();

void clearConsole();

void setColor(int newColor);

void printText(char *outputString, int color);

void printChar(char c, int color);

void printInt(unsigned int number, int color);

void runCursor();

void drawCursor();

void clearCursor();