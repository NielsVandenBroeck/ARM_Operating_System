enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};

struct character {
    char value;
    int color;
    //add other attributes later
};

typedef struct character Character;

//Communication over serial port, used for testing.
void initConsole();

void runConsole();

void processChar(char c);

char* readLine();

void updateCursorPosition();

void runCursor();

void printText(char *outputString, int color);

void printInt(unsigned int number, int color);

void printChar(char c, int color);

void drawGlyph(char c, int x, int y, int color);

void scrollUp();

void drawCursor();

void clearCursor();

void clearConsole();

void drawFromBuffer();

//functionality that can be called by user
void rotateScreen(int orientation);
void scaleScreen(int multiplier);
void changeTextColor(int color);