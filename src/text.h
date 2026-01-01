#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Texture* texture;
    int w;
    int h;
} TextObject;

TextObject Text_Create(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color color);
void Text_Draw(SDL_Renderer* renderer, TextObject textObj, int x, int y);
void Text_Destroy(TextObject* textObj);
int text_Center(int window_WH, int text_WH);

#endif