#include "../basic/mem.h"
#include "../basic/mb.h"
#include "../basic/error.h"
#include "../Command-Line-Interface/console.h"

unsigned int width, height, pitch, isrgb;
unsigned int sizeScale = 2;
unsigned char *fb;

void fb_init()
{
    mbox[0] = 35*4; // Length of message in bytes
    mbox[1] = MBOX_REQUEST;

    mbox[2] = MBOX_TAG_SETPHYWH; // Tag identifier
    mbox[3] = 8; // Value size in bytes
    mbox[4] = 0;
    mbox[5] = 1920; // Value(width)
    mbox[6] = 1080; // Value(height)

    mbox[7] = MBOX_TAG_SETVIRTWH;
    mbox[8] = 8;
    mbox[9] = 8;
    mbox[10] = 1920;
    mbox[11] = 1080;

    mbox[12] = MBOX_TAG_SETVIRTOFF;
    mbox[13] = 8;
    mbox[14] = 8;
    mbox[15] = 0; // Value(x)
    mbox[16] = 0; // Value(y)

    mbox[17] = MBOX_TAG_SETDEPTH;
    mbox[18] = 4;
    mbox[19] = 4;
    mbox[20] = 32; // Bits per pixel

    mbox[21] = MBOX_TAG_SETPXLORDR;
    mbox[22] = 4;
    mbox[23] = 4;
    mbox[24] = 1; // RGB

    mbox[25] = MBOX_TAG_GETFB;
    mbox[26] = 8;
    mbox[27] = 8;
    mbox[28] = 4096; // FrameBufferInfo.pointer
    mbox[29] = 0;    // FrameBufferInfo.size

    mbox[30] = MBOX_TAG_GETPITCH;
    mbox[31] = 4;
    mbox[32] = 4;
    mbox[33] = 0; // Bytes per line

    mbox[34] = MBOX_TAG_LAST;

    // Check call is successful and we have a pointer with depth 32
    if (mbox_call(MBOX_CH_PROP) && mbox[20] == 32 && mbox[28] != 0) {
        mbox[28] &= 0x3FFFFFFF; // Convert GPU address to ARM address
        width = mbox[10];       // Actual physical width
        height = mbox[11];      // Actual physical height
        pitch = mbox[33];       // Number of bytes per line
        isrgb = mbox[24];       // Pixel order
        fb = (unsigned char *)((long)mbox[28]);
    }
}

unsigned int getHeight(){
    return height;
}

unsigned int getWidth(){
    return width;
}

void drawPixel(int x, int y, int color)
{
    for(int scalex = 0; scalex < sizeScale; scalex++){
        for(int scaley = 0; scaley < sizeScale; scaley++){
            int newY = (y * sizeScale) + scaley;
            int newX = (x * sizeScale) + scalex;
            int offs = (newY * pitch) + (newX * 4);
            *((unsigned int*)(fb + offs)) = color;
        }
    }
}

void drawScreen(int color){
    for(int x = 0; x < getHeight(); x++){
        for(int y = 0; y < getWidth(); y++){
            drawPixel(x,y,color);
        }
    }
}

void wait_msec(unsigned int n)
{
    register unsigned long f, t, r;

    // Get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    t+=((f/1000)*n);
    do{asm volatile ("mrs %0, cntpct_el0" : "=r"(r));}while(r<t);
}

void colorTest(){
    int x = 0;
    for(int r = 0; r < 256; r += 4){
        for(int g = 0; g < 256; g += 4){
            for(int b = 0; b < 256; b += 4){
                float newColor = ((b & 0xff) << 16) + ((g & 0xff) << 8) + ((r & 0xff));
                for(int i = 0; i < 5; i++){
                    drawPixel(x, i, newColor);
                }
                x++;
            }
        }
    }
}

//upscaling does not work only down scaling because we take te previus pixel
//try working from R bottom when upscaling!
void setInterfaceScaling(unsigned int scalingValue){

    if(scalingValue <= 0){
        throw("Cannot define a scaling value smaller of the same as 0");
    }

    int scaleChange = sizeScale - scalingValue;

    if(scaleChange < 0){
        scaleChange = -scaleChange;
    }

    int changeSizeX =  width/sizeScale;
    int changeSizeY =  height/sizeScale;

    int oldScaleSize = sizeScale;
    sizeScale = scalingValue;

    for(int y = 0; y <= changeSizeY; y++){
        for(int x = 0; x <= changeSizeX; x++){
            int oldY = (y * oldScaleSize);
            int oldX = (x * oldScaleSize);
            int oldOffs = (oldY * pitch) + (oldX * 4);
            //int newoffs = (y * pitch) + (x * 4);
            drawPixel(x, y, *((unsigned int*)(fb + oldOffs)));
            //*((unsigned int*)(fb + newoffs)) = *((unsigned int*)(fb + oldOffs));
        }
    }

    for(int y = 0; y < height; y++){
        for(int x = changeSizeX; x < width; x++){
            int newoffs = (y * pitch) + (x * 4);
            *((unsigned int*)(fb + newoffs)) = 0x000000;
        }
    }
    for(int y = changeSizeY; y < height; y++){
        for(int x = 0; x < width; x++){
            int newoffs = (y * pitch) + (x * 4);
            *((unsigned int*)(fb + newoffs)) = 0x000000;
        }
    }

}