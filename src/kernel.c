#include "uart/uart.h"
#include "basic/malloc.h"
#include "graphicInterface/framebuffer.h"
#include "console/console.h"
#include "basic/error.h"


void main()
{
    char* temp;
    uart_init(115200);
    asm ("");

    uart_print("Hello world\r\n");
/*
    fb_init();

    int g = 0;

    while(1){
        for(int i = 0; i <= getWidth(); i++){
            for(int a = 0; a <= getHeight(); a++){
                if(g % 3 == 0){
                    drawPixel(i,a,green);
                }
                else if(g % 3 == 1){
                    drawPixel(i,a,red);
                }
                else if(g % 3 == 2){
                    drawPixel(i,a,blue);
                }
            }
        }
        g++;
    }*/
/*
    drawString(10,10,"nielsvandenbroeck@MSI:",red);
    drawString(180,10,"~",green);
    drawString(188,10,"$ ls",blue);
    drawString(10,25,"PPDB-Template-App  gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf  key  node_modules  test-website",1);
    throw("Cannot do that!");
*/
    console_init();
    console_println("Hello world");

    setInterfaceScaling(1);
    console_println("Hello world yeet");




    while(1){
        temp = uart_readline();
        console_println(temp);

        setInterfaceScaling(2);
        //uart_print("\r\n");
        free(temp);
    }
}