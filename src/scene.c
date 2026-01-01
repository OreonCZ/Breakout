#include <string.h>

#include "scene.h"
#include "blocks.h"
#include "colors.h"
#include "text.h"
#include "leaderboard.h"

// menu ////////////////////////////////////////////////////////////
void MainMenu(GameInfo* gameInfo){
    SDL_SetRenderDrawColor(gameInfo->renderer, ubuntuOG.r, ubuntuOG.g, ubuntuOG.b, 255);
    SDL_GetWindowSize(gameInfo->window, &gameInfo->windowWidth, &gameInfo->windowHeight);
    // render background
    SDL_RenderClear(gameInfo->renderer);

    int startY = 310;
    for (int i = 0; i < LEADERBOARD_Size; i++){

        if (gameInfo->highScore[i].score == 0) continue; 
        
        char buffer[64];
        sprintf(buffer, "%d. %s  %d", i + 1, gameInfo->highScore[i].name, gameInfo->highScore[i].score);

        TextObject leaderboardText = Text_Create(gameInfo->renderer, gameInfo->font, buffer, white);
        Text_Draw(gameInfo->renderer, leaderboardText, text_Center(gameInfo->windowWidth, leaderboardText.w), startY + (i * 30));
        Text_Destroy(&leaderboardText);
    }
}

void MainMenu_Input(GameInfo* gameInfo, SDL_Event* event){
    SDL_Cursor* cursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    SDL_Cursor* cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    SDL_Cursor* cursorText = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);

    int mx, my;
    SDL_GetMouseState(&mx, &my);

    if (mx > gameInfo->inputBar.x && mx < gameInfo->inputBar.x + gameInfo->inputBar.w 
        && my > gameInfo->inputBar.y && my < gameInfo->inputBar.y + gameInfo->inputBar.h){
            SDL_SetCursor(cursorText);
    }
    else SDL_SetCursor(cursorArrow);

    if (event->type == SDL_MOUSEBUTTONDOWN){
        SDL_Rect fieldRect = {gameInfo->inputBar.x, gameInfo->inputBar.y, gameInfo->inputBar.w, gameInfo->inputBar.h}; 

        if (mx > fieldRect.x && mx < fieldRect.x + fieldRect.w &&
            my > fieldRect.y && my < fieldRect.y + fieldRect.h){
            gameInfo->isPlayerWriting = 1;
            SDL_StartTextInput(); 
        } 
        else {
            gameInfo->isPlayerWriting = 0;
            SDL_StopTextInput();
        }
    }

    Text_Input(gameInfo, event);

    if (event->key.keysym.sym == SDLK_r) remove("save.bin");
}

// game ////////////////////////////////////////////////////////////
void InGame(GameInfo* gameInfo){

    SDL_SetRenderDrawColor(gameInfo->renderer, 120, 0, 60, 255);
    SDL_GetWindowSize(gameInfo->window, &gameInfo->windowWidth, &gameInfo->windowHeight);
    // render background
    SDL_RenderClear(gameInfo->renderer);

    SDL_SetRenderDrawColor(gameInfo->renderer, gameBoardColor.r, gameBoardColor.g, gameBoardColor.b, gameBoardColor.a);
    SDL_RenderFillRect(gameInfo->renderer, &gameInfo->gameBoard.gameBoardRect);

    for (int i = 0; i < gameInfo->blockCount; i++) Block_Draw(gameInfo->renderer, &gameInfo->blocks[i]);

    Ball_Draw(gameInfo->renderer, &gameInfo->ball);
    Ball_Update(&gameInfo->ball, gameInfo);
    Player_Draw(gameInfo->renderer, &gameInfo->player);
    // player movement
    Player_Move(&gameInfo->player, gameInfo->gameBoard.gameBoardRect.x, gameInfo->gameBoard.gameBoardRect.x + gameInfo->gameBoard.gameBoardRect.w);
}

void InGame_Input(GameInfo* gameInfo, SDL_Event* event){
    if (event->type == SDL_KEYDOWN){
        switch (event->key.keysym.sym){
            case SDLK_ESCAPE:
                gameInfo->sceneIndex = 0;
                break;
        }
    }
}

void Text_Input(GameInfo* gameInfo, SDL_Event* event){
    if (gameInfo->isPlayerWriting == 0) return;

    if (event->type == SDL_TEXTINPUT){
        size_t len = strlen(gameInfo->playerName);

        if (len < PLAYERNAME_Buffer - 1){
            strcat(gameInfo->playerName, event->text.text);
            if (gameInfo->isPlayerWriting) Mix_PlayChannel(-1, gameInfo->soundType, 0);
        };
    }
    else if (event->type == SDL_KEYDOWN){
        if (event->key.keysym.sym == SDLK_BACKSPACE){
            size_t len = strlen(gameInfo->playerName);
            if (len > 0){
                gameInfo->playerName[len - 1] = '\0';
                if (gameInfo->isPlayerWriting) Mix_PlayChannel(-1, gameInfo->soundType, 0);
            }
        }
        else if (event->key.keysym.sym == SDLK_RETURN){
            gameInfo->isPlayerWriting = 0;
            SDL_StopTextInput();
        }
    }
}

void TextBar_Draw(GameInfo* gameInfo, int x, int y, int w, int h){
    SDL_Rect inputRect = {x, y, w, h};

    if (gameInfo->isBarEmpty == 0) SDL_SetRenderDrawColor(gameInfo->renderer, white.r, white.g, white.b, white.a);
    else if (gameInfo->isPlayerWriting) SDL_SetRenderDrawColor(gameInfo->renderer, white.r - 55, white.g - 15, white.b - 55, white.a);
    else SDL_SetRenderDrawColor(gameInfo->renderer, white.r - 55, white.b - 55, white.b - 55, white.a);
    
    SDL_RenderFillRect(gameInfo->renderer, &inputRect);

    // outline
    SDL_SetRenderDrawColor(gameInfo->renderer, black.r, black.g, black.b, black.a);
    SDL_RenderDrawRect(gameInfo->renderer, &inputRect);

    char displayString[PLAYERNAME_Buffer];
    strcpy(displayString, gameInfo->playerName);


    if (strlen(displayString) > 0){

        TextObject nameObj = Text_Create(gameInfo->renderer, gameInfo->font, displayString, black);
        Text_Draw(gameInfo->renderer, nameObj, x + 10, y + (h - nameObj.h) / 2); 
        gameInfo->isBarEmpty = 0;

        Text_Destroy(&nameObj); 
    }
    else gameInfo->isBarEmpty = 1;
    
}
