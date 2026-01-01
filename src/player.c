#include "player.h"

Player Player_Create(int x_cor, int y_cor, int width, int height, float speed, SDL_Color playerColor){
    Player player = {
        .playerRect.x = x_cor,
        .playerRect.y = y_cor,
        .playerRect.w = width,
        .playerRect.h = height,
        .speed = speed,
        .playerColor = playerColor
    };

    return player;
}

void Player_Draw(SDL_Renderer *renderer, Player *player){

    SDL_SetRenderDrawColor(renderer, player->playerColor.r, player->playerColor.g, player->playerColor.b, player->playerColor.a);
    SDL_RenderFillRect(renderer, &player->playerRect);
}

void Player_Move(Player *player, int minX, int maxX){
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    if ((state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) && (player->playerRect.x > minX)){
        player->playerRect.x -= (int)player->speed;
    }

    if ((state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) && (player->playerRect.x < (maxX - player->playerRect.w))){
        player->playerRect.x += (int)player->speed;
    }
}

