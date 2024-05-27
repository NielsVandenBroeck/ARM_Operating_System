#include "pong.h"
#include "../command_line_interface/console.h"
#include "../graphicInterface/framebuffer.h"
#include "../basic/keyboard_interrupts.h"
#include <stddef.h>

struct paddle{
    int x, y;
};

struct ball{
    int x, y;
    int dx, dy;
};

struct score{
   int player1, player2;
};

typedef struct paddle Paddle;
typedef struct ball Ball;
typedef struct score Score;

void initPaddle(Paddle* player, int xPos, int yPos) {
    player->x = xPos;
    player->y = yPos;
    for(int y = player->y-15; y < player->y+15; y++){
        for(int x = player->x-70; x < player->x+70; x++){
            drawScaledPixelsWindow(x,y,white);
        }
    }
}

void initBall(Ball* ball, int xPos, int yPos) {
    ball->x = xPos;
    ball->y = yPos;
    ball->dx = 1;
    ball->dy = 2;
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,white);
        }
    }
}

void initScore(Score* score){
    score->player1 = 0;
    score->player2 = 0;
    drawGlyph('0',100,100,white);
    drawGlyph('-',110,100,white);
    drawGlyph('0',120,100,white);
}

void clearPaddle(Paddle* player){
    for(int y = player->y-15; y < player->y+15; y++){
        for(int x = player->x-70; x < player->x+70; x++){
            drawScaledPixelsWindow(x,y,black);
        }
    }
}

void clearBall(Ball* ball){
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,black);
        }
    }
}


void goLeft(Paddle* player){
    if(player->x > 80){
        player->x -= 20;
        for(int y = player->y-15; y < player->y+15; y++){
            for(int x = player->x-70; x < player->x-50; x++){
                drawScaledPixelsWindow(x,y,white);
            }
            for(int x = player->x+70; x < player->x+90; x++){
                drawScaledPixelsWindow(x,y,black);
            }
        }
    }
}

void goRight(Paddle* player){
    if(player->x < 1840){
        player->x += 20;
        for(int y = player->y-15; y < player->y+15; y++){
            for(int x = player->x+50; x < player->x+70; x++){
                drawScaledPixelsWindow(x,y,white);
            }
            for(int x = player->x-90; x < player->x-70; x++){
                drawScaledPixelsWindow(x,y,black);
            }
        }
    }
}

void updateBall(Ball* ball){
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,black);
        }
    }
    ball->x += ball->dx;
    ball->y += ball->dy;
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,white);
        }
    }
}

void checkCollision(Paddle* player1, Paddle* player2, Ball* ball){
    //check if ball is about to touch a wall
    if(ball->x <= 5 || ball->x >= 1915){
        ball->dx = -ball->dx;
    }
    // Collision with paddles
    if(ball->y >= player1->y-18 && ball->x > player1->x-70 && ball->x < player1->x+70){
        ball->dy = -ball->dy;
    }
    if(ball->y <= player2->y+18 && ball->x > player2->x-70 && ball->x < player2->x+70){
        ball->dy = -ball->dy;
    }
}

void updateScore(Score* score, int player){
    if(player == 1){
        drawGlyph(score->player1 + '0',100,100,black);
        score->player1++;
        drawGlyph(score->player1 + '0',100,100,white);
    }
    else{
        drawGlyph(score->player2 + '0',120,100,black);
        score->player2++;
        drawGlyph(score->player2 + '0',120,100,white);
    }
}

void gameLoop(){
    Score score;
    initScore(&score);
    while(1){
        Paddle player1, player2;
        Ball ball;
        initPaddle(&player1, 960, 1015);
        initPaddle(&player2, 960, 65);
        initBall(&ball,960, 540);
        while(1){
            char* inputChar = keyboardInterruptionGetChar();
            while (inputChar != NULL){
                if(*inputChar == '&'){
                    return;
                }
                if(*inputChar == 'q'){
                    goLeft(&player1);
                }
                if(*inputChar == 'd'){
                    goRight(&player1);
                }
                if(*inputChar == ':'){
                    goLeft(&player2);
                }
                if(*inputChar == '='){
                    goRight(&player2);
                }
                inputChar = keyboardInterruptionGetChar();
            }
            updateBall(&ball);
            checkCollision(&player1, &player2, &ball);
            if(ball.y < 5){
                updateScore(&score, 1);
                clearPaddle(&player1);
                clearPaddle(&player2);
                clearBall(&ball);
                break;
            }
            if(ball.y > 1075){
                updateScore(&score, 2);
                clearPaddle(&player1);
                clearPaddle(&player2);
                clearBall(&ball);
                break;
            }
            wait_msec(5);
        }
    }
}


void pong(char* params){
    printText("Starting pong.", CURRENT_COLOR);
    clearConsole();
    setRotation(0);
    setScaleSize(1);
    gameLoop();
    drawScreen(black);
    drawFromBuffer();
}