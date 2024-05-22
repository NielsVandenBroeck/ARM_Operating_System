enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};


//Communication over serial port, used for testing.
void initConsole();

void runConsole();

void processChar(char c);

void handleCommand();

char* readLine();

void runCursor();

void printText(char *outputString, int color);

void printInt(unsigned int number, int color);

void printChar(char c, int color);

void drawGlyph(char c, int x, int y, int color);

void scrollUp();

void drawCursor();

void clearCursor();

void clearTextBuffer();

void clearConsole();

void drawFromBuffer();

//functionality that can be called by user
void rotateScreen(int orientation);
void scaleScreen(int multiplier);
void changeTextColor(int color);