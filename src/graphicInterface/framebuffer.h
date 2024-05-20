void fb_init();

unsigned int getHeight();

unsigned int getWidth();

void setScaleSize(int size);

int getScaleSize();

void setRotation(int rotation);

void drawScreen(int color);

void drawPixel(int x, int y, int color);

int getPixelColor(int x, int y);

void drawScaledPixels(int x, int y, int color);

void setInterfaceScaling(unsigned int scalingValue);

void printtest();

void wait_msec(unsigned int n);

void colorTest();

void colorTest2();

void uartDraw(char c);

//default colors
enum {
    black = 0x000000,
    white = 0xFFFFFF,
    red = 0xFF0000,
    green = 0x00FF00,
    blue = 0x0000FF
};