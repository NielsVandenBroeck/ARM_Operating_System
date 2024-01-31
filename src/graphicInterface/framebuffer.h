void fb_init();

unsigned int getHeight();

unsigned int getWidth();

void drawPixel(int x, int y, int color);

//x and y will be set to the new current position
void drawString(int* x, int* y, char *s, int color);

void drawCursor(int*x, int* y, int color);

void setInterfaceScaling(unsigned int scalingValue);

//default colors
enum {
    black = 0x000000,
    white = 0x000000,
    red = 0x0000FF,
    green = 0x00FF00,
    blue = 0xFF0000
};