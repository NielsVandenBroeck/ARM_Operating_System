#ifndef __FRAMEBUFFER_H_
#define __FRAMEBUFFER_H_

#include "../data_structures/enums.h"

typedef enum { TOP, BOTTOM } yOffsetLocation;

void fb_init();

void setYWindowOffsets(int yOffset, yOffsetLocation offsetLocation);

void setXWindowOffsets(int xOffset, leftRight offsetLocation);

unsigned int getWindowHeight();

unsigned int getWindowWidth();

unsigned int getScreenHeight();

unsigned int getScreenWidth();

void setScaleSize(int size);

void setRotation(int rotation);

void drawScreen(int color);

int getPixelColor(int x, int y);

void drawScaledPixelsWindow(int x, int y, int color);

void drawScaledPixelsScreen(int x, int y, int color);

void setInterfaceScaling(unsigned int scalingValue);

void drawGlyph(char c, int x, int y, int color);

void drawGlyphScreen(char c, int x, int y, int color);

void colorTest();

void colorTest2();

//default colors
enum {
    black = 0x000000,
    white = 0xFFFFFF,
    red = 0xFF0000,
    green = 0x00FF00,
    blue = 0x0000FF
};
#endif