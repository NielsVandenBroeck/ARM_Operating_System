#include "pong.h"
#include "../command_line_interface/console.h"
#include "../graphicInterface/framebuffer.h"
#include "../basic/keyboard_interrupts.h"
#include "../basic/time.h"
#include "../taskbar/taskbar.h"
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

    int values[] = {-2, -1, 1, 2};
    ball->dx = values[random(4)];
    ball->dy = values[random(4)];
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

void clearScore(Score* score){
    drawGlyph(score->player1 + '0',100,100,black);
    drawGlyph('-',110,100,black);
    drawGlyph(score->player2 + '0',120,100,black);
}


void goLeft(Paddle* player){
    if(player->x > 80){
        player->x -= 20;
        for(int x = player->x-70; x < player->x-50; x++){
            for(int y = player->y-15; y < player->y+15; y++){
                drawScaledPixelsWindow(x,y,white);
            }
        }
        for(int x = player->x+70; x < player->x+90; x++){
            for(int y = player->y-15; y < player->y+15; y++){
                drawScaledPixelsWindow(x,y,black);
            }
        }
    }
}

void goRight(Paddle* player){
    if(player->x < getWindowWidth()-80){
        player->x += 20;
        for(int x = player->x+50; x < player->x+70; x++){
            for(int y = player->y-15; y < player->y+15; y++){
                drawScaledPixelsWindow(x,y,white);
            }
        }
        for(int x = player->x-90; x < player->x-70; x++){
            for(int y = player->y-15; y < player->y+15; y++){
                drawScaledPixelsWindow(x,y,black);
            }
        }
    }
}

void updateBall(Ball* ball, int multiplier){
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,black);
        }
    }
    ball->x += ball->dx*multiplier;
    ball->y += ball->dy*multiplier;
    for(int y = ball->y-3; y < ball->y+3; y++){
        for(int x = ball->x-3; x < ball->x+3; x++){
            drawScaledPixelsWindow(x,y,white);
        }
    }
}

void checkCollision(Paddle* player1, Paddle* player2, Ball* ball){
    //check if ball is about to touch a wall
    if(ball->x <= 5 || ball->x >= getWindowWidth()-5){
        ball->dx = -ball->dx;
    }
    // Collision with paddles
    if(ball->y >= player1->y-18 && ball->x > player1->x-70 && ball->x < player1->x+70){
        if(ball->dy < 0){
            ball->dy = -ball->dy+1;
        }
        else{
            ball->dy = -ball->dy-1;
        }
        if(ball->dx < 0){
            ball->dx = ball->dx-1;
        }
        else{
            ball->dx = ball->dx+1;
        }
    }
    if(ball->y <= player2->y+18 && ball->x > player2->x-70 && ball->x < player2->x+70){
        if(ball->dy < 0){
            ball->dy = -ball->dy+1;
        }
        else{
            ball->dy = -ball->dy-1;
        }
        if(ball->dx < 0){
            ball->dx = ball->dx-1;
        }
        else{
            ball->dx = ball->dx+1;
        }
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

void pongGameLoop(){
    Score score;
    initScore(&score);
    while(1){
        Paddle player1, player2;
        Ball ball;
        initPaddle(&player1, getWindowWidth()/2, getWindowHeight()-35);
        initPaddle(&player2, getWindowWidth()/2, 35);
        initBall(&ball,getWindowWidth()/2, getWindowHeight()/2);
        while(1){
            unsigned int startTime = 0;
            start_timer(&startTime);
            char* inputChar = keyboardInterruptionGetChar();
            while (inputChar != NULL){
                if(*inputChar == '&'){
                    clearBall(&ball);
                    clearPaddle(&player1);
                    clearPaddle(&player2);
                    clearScore(&score);
                    printText("Final score: ",CURRENT_COLOR);
                    printInt(score.player1, CURRENT_COLOR);
                    printChar('-', CURRENT_COLOR);
                    printInt(score.player2, CURRENT_COLOR);
                    return;
                }
                if(*inputChar == 'a'){
                    goLeft(&player1);
                }
                if(*inputChar == 'd'){
                    goRight(&player1);
                }
                if(*inputChar == '.'){
                    goLeft(&player2);
                }
                if(*inputChar == '/'){
                    goRight(&player2);
                }
                inputChar = keyboardInterruptionGetChar();
            }
            wait_msec(10);
            int elapsedTime = elapsed_time(startTime);
            updateBall(&ball, elapsedTime/10);
            checkCollision(&player1, &player2, &ball);
            if(ball.y < 5){
                updateScore(&score, 1);
                if(score.player1 == 5){
                    clearBall(&ball);
                    clearPaddle(&player1);
                    clearPaddle(&player2);
                    clearScore(&score);
                    printText("Player 1 Won! Final score: ",CURRENT_COLOR);
                    printInt(score.player1, CURRENT_COLOR);
                    printChar('-', CURRENT_COLOR);
                    printInt(score.player2, CURRENT_COLOR);
                    return;
                }
                clearPaddle(&player1);
                clearPaddle(&player2);
                clearBall(&ball);
                break;
            }
            if(ball.y > getWindowHeight()-5){
                updateScore(&score, 2);
                if(score.player1 == 5){
                    clearBall(&ball);
                    clearPaddle(&player1);
                    clearPaddle(&player2);
                    clearScore(&score);
                    printText("Player 2 Won! Final score: ",CURRENT_COLOR);
                    printInt(score.player1, CURRENT_COLOR);
                    printChar('-', CURRENT_COLOR);
                    printInt(score.player2, CURRENT_COLOR);
                    return;
                }
                clearPaddle(&player1);
                clearPaddle(&player2);
                clearBall(&ball);
                break;
            }
        }
    }
}


void pong(char* params){
    printText("Starting pong.\n", CURRENT_COLOR);
    clearConsole();
    setRotation(0);
    taskBarClear();
    pongGameLoop();
    taskBarDraw();
    drawFromBuffer();
}