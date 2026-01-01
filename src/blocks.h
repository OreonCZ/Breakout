#ifndef BLOCKS_H
#define BLOCKS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct 
{
    SDL_Rect blockRect;
    SDL_Color blockColor;

} Block;

Block Block_Create(int x, int y, int w, int h, SDL_Color blockColor);
void Block_Draw(SDL_Renderer* renderer, Block* block);
void Block_Load(Block* blocksArray, int* blockCount, char* path, int startX, int startY);


#endif