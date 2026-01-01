#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_keyboard.h>

typedef struct{
    float speed;
    SDL_Rect playerRect;
    SDL_Color playerColor;
} Player;

Player Player_Create(int x_cor, int y_cor, int width, int height, float speed, SDL_Color playerColor);
void Player_Draw(SDL_Renderer *renderer, Player *player);
void Player_Move(Player *player, int minX, int maxX);

#endif