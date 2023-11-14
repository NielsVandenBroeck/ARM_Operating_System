void fb_init();

unsigned int getHeight();

unsigned int getWidth();

void drawPixel(int x, int y, int color);

void drawString(int x, int y, char *s, int color);

//possible colors
enum {
    red = 0xAA0000,
    green = 0x00AA00,
    blue = 0x0000AA
};