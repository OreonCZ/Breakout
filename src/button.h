#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "player.h"
#include "text.h"
#include "scene.h"

typedef struct {
    SDL_Rect buttonRect;
    TextObject buttonText;
    SDL_Color buttonColor;
} Button;

Button Button_Create(SDL_Renderer* renderer, TTF_Font* font, SDL_Color buttonColor, char* buttonText, int x, int y, int width, int height);
int Button_Clicked(Button* button, SDL_Event* event, int isBarEmpty);

void Button_Draw(SDL_Renderer* renderer, Button* button, int isBarEmpty);
void Button_Destroy(Button* button);

#endif