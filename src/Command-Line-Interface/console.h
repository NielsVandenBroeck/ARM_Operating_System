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

void printText(char *outputString);

void printChar(char c);

void printInt(unsigned int number);

void drawCursor();