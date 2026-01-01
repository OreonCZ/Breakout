#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>
#include <time.h>
#include <stdlib.h>
#include "blocks.h"
#include "colors.h"

typedef struct GameInfo GameInfo;

typedef struct {
    SDL_Rect rect;
    float dirX;
    float dirY;
    float speed;
} Ball;

Ball Ball_Create(int x, int y, float speed);
void Ball_Draw(SDL_Renderer* renderer, Ball* ball);
void Ball_Update(Ball* ball, GameInfo* gameInfo);

#endif