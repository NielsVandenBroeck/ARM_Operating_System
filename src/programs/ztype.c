#include "ztype.h"
#include "../command_line_interface/console.h"
#include "../basic/time.h"
#include "../data_structures/array.h"
#include "../graphicInterface/framebuffer.h"
#include <stddef.h>

struct letter{
    char c;
    int x, y;
    int speed;
    //int scale;
};

typedef struct letter Letter;

void removeLetters(Letter* letters){
    for(int i = 0; i < arrayGetLength(letters); i++){
        Letter* letter = (Letter *)arrayGetItem(letters, i);
        drawGlyph(letter->c, letter->x, letter->y, black);
    }
}

void zTypeGameLoop(){
    int score = 0;
    Array* letters = newArray(0, sizeof(Letter));
    int likeliness = 50;
    int counter = 1;
    while(1){
        if(counter%400 == 0){
            if(likeliness >= 4){
                likeliness-=2;
            }
        }
        if(random(likeliness) == 0){
            Letter l = {
                    'a' + random(26),
                    random(getWindowWidth()-20)+10,
                    10,
                    random(51-likeliness)+1
            };
            drawGlyph(l.c,l.x,l.y,CURRENT_COLOR);
            arrayAppendItem(letters,&l);
        }
        char* inputChar = keyboardInterruptionGetChar();
        while (inputChar != NULL){
            if(*inputChar == '&'){
                removeLetters(letters);
                printText("Game exited Final score: ",CURRENT_COLOR);
                printInt(score, CURRENT_COLOR);
                return;
            }
            //check if valid letter, add points or else remove points
            for(int i = 0; i < arrayGetLength(letters); i++){
                Letter* letter = (Letter *)arrayGetItem(letters, i);
                if(*inputChar == letter->c){
                    score += 50;
                    drawGlyph(letter->c, letter->x, letter->y, black);
                    arrayRemoveItem(letters, i);
                    break;
                }
            }
            if(score >= 25){
                score-=25;
            }
            //todo display score
            inputChar = keyboardInterruptionGetChar();
        }

        //update letter positions
        for(int i = 0; i < arrayGetLength(letters); i++){
            Letter* letter = (Letter *)arrayGetItem(letters, i);
            drawGlyph(letter->c, letter->x, letter->y, black);
            letter->y += letter->speed;
            if(letter->y > getWindowHeight()-10){
                removeLetters(letters);
                printText("Game over. Final score: ",CURRENT_COLOR);
                printInt(score, CURRENT_COLOR);
                return;
            }
            drawGlyph(letter->c, letter->x, letter->y, CURRENT_COLOR);
        }
        wait_msec(20);
        counter++;
    }

}


void ztype(char* params){
    printText("Starting ztype.\n", CURRENT_COLOR);
    clearConsole();
    zTypeGameLoop();
    drawFromBuffer();
}

