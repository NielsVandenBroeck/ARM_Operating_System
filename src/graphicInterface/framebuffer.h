void fb_init();

unsigned int getHeight();

unsigned int getWidth();

void drawScreen(int color);

void drawPixel(int x, int y, int color);

void setInterfaceScaling(unsigned int scalingValue);

void wait_msec(unsigned int n);

void colorTest();

//default colors
enum {
    black = 0x000000,
    white = 0xFFFFFF,
    red = 0x0000FF,
    green = 0x00FF00,
    blue = 0xFF0000
};