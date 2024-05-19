enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};

//Communication over serial port, used for testing.
void initConsole();

void runConsole();

void processChar(char c);

void nextLine();

char* readLine();

void clearConsole();

void updateCursorPosition();

void runCursor();

void setScaling(int size);

//temp public for testing
void printText(char *outputString, int color);
void printChar(char c, int color);
void printInt(unsigned int number, int color);
void scrollUp();
void drawCursor();
void clearCursor();
void setColor(int newColor);