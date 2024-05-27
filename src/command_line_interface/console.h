#ifndef __CONSOLE_H_
#define __CONSOLE_H_

enum{
    XOFFSET = 10,
    YOFFSET = 10,
    LINEHEIGHT = 10
};

//variables related to pixels and positions
extern int currentConsolePosition[2]; //The position to draw the next character
extern int CURRENT_COLOR; //The color of the text to be drawn

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

void initiatePreText();

void handleCommand();

void runCursor();

void printText(char *outputString, int color);

void printInt(unsigned int number, int color);

void printChar(char c, int color);

void scrollUp();

void drawCursor();

void clearLastLine();

void drawLastLine();

void clearCursor();

void clearTextBuffer();

void clearConsole();

void drawFromBuffer();

//functionality that can be called by user
void rotateScreen(int orientation);
void scaleScreen(int multiplier);
void changeTextColor(int color);
#endif