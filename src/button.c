#include "button.h"
#include "colors.h"

Button Button_Create(SDL_Renderer* renderer, TTF_Font* font, SDL_Color buttonColor, char* buttonText, int x, int y, int width, int height){
    
    Button button = {
        .buttonRect = (SDL_Rect){x, y, width, height},
        .buttonColor = {buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a},
        .buttonText = Text_Create(renderer, font, buttonText, black)
    };
    return button;
}


int Button_Clicked(Button* button, SDL_Event* event, int isBarEmpty){
    SDL_Cursor* cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor* cursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_Cursor* cursorBadNoNo = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

    int mouseX, mouseY;
    // mouse position
    SDL_GetMouseState(&mouseX, &mouseY);

    if ((mouseX > button->buttonRect.x && mouseX < button->buttonRect.x + button->buttonRect.w) 
        && (mouseY > button->buttonRect.y && mouseY < button->buttonRect.y + button->buttonRect.h)){
            if (isBarEmpty) SDL_SetCursor(cursorBadNoNo);
            else SDL_SetCursor(cursorHand);
            
            if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT){
                SDL_SetCursor(cursorArrow);
                return 1;
            }
            else return 0;
        }
    else {
        return 0;
    }
};

void Button_Draw(SDL_Renderer* renderer, Button* button, int isBarEmpty){
    if (isBarEmpty == 1) SDL_SetRenderDrawColor(renderer, button->buttonColor.r - 50, button->buttonColor.g - 50,
        button->buttonColor.b - 50, button->buttonColor.a);

    else  SDL_SetRenderDrawColor(renderer, button->buttonColor.r, button->buttonColor.g, button->buttonColor.b, button->buttonColor.a);
    
    SDL_RenderFillRect(renderer, &button->buttonRect);

    int textX = button->buttonRect.x + text_Center(button->buttonRect.w, button->buttonText.w);
    int textY = button->buttonRect.y + text_Center(button->buttonRect.h, button->buttonText.h);
    Text_Draw(renderer, button->buttonText, textX, textY);

    // outline
    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderDrawRect(renderer, &button->buttonRect);
    
}

void Button_Destroy(Button* button){
    Text_Destroy(&button->buttonText);
}





