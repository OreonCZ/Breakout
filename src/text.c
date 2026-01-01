#include "text.h"
#include "scene.h"

// text create
TextObject Text_Create(SDL_Renderer* renderer, TTF_Font* font, char* text, SDL_Color color){

    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return (TextObject){
        .texture = NULL, 
        .w = 0, 
        .h = 0
    }; 

    TextObject textObj = {
        .texture = SDL_CreateTextureFromSurface(renderer, surface),
        .w = surface->w,
        .h = surface->h
    };

    SDL_FreeSurface(surface);
    return textObj;
}

void Text_Draw(SDL_Renderer* renderer, TextObject textObj, int x, int y){
    if (textObj.texture){
        SDL_Rect destRect = { 
            x,
            y,
            textObj.w,
            textObj.h
        };
        
        SDL_RenderCopy(renderer, textObj.texture, NULL, &destRect);
    }
}

void Text_Destroy(TextObject* textObj){
    if (textObj && textObj->texture){
        SDL_DestroyTexture(textObj->texture);
        textObj->texture = NULL;
        textObj->w = 0;
        textObj->h = 0;
    }
}

int text_Center(int window_WH, int text_WH){
    return (window_WH - text_WH) / 2;
}

