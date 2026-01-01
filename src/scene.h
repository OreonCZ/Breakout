#ifndef SCENE_H
#define SCENE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "player.h"
#include "text.h"
#include "button.h"
#include "blocks.h"
#include "ball.h"
#include "leaderboard.h"

#define BLOCKS_Buffer 400
#define PLAYERNAME_Buffer 17

typedef struct {
    int paddingX;
    int paddingY;
    SDL_Rect gameBoardRect;
} GameBoard;

typedef struct GameInfo {
    SDL_Renderer* renderer;
    SDL_Window* window;
    int windowWidth;
    int windowHeight;
    TTF_Font* font;
    TTF_Font* fontBigger;
    int sceneIndex;
    
    Player player;
    GameBoard gameBoard;

    Block blocks[BLOCKS_Buffer];
    int blockCount;
    char playerName[PLAYERNAME_Buffer];
    int isPlayerWriting;
    int isBarEmpty;

    int playerScore;
    int playerLifes;

    SDL_Rect inputBar;
    Ball ball;
    HighScore highScore[LEADERBOARD_Size];

    Mix_Chunk* soundPlayer;
    Mix_Chunk* soundBlock;
    Mix_Chunk* soundWall;
    Mix_Chunk* soundType;
    Mix_Chunk* soundOuch;
} GameInfo;

void MainMenu(GameInfo* gameInfo);
void MainMenu_Input(GameInfo* gameInfo, SDL_Event* event);

void InGame(GameInfo* gameInfo);
void InGame_Input(GameInfo* gameInfo, SDL_Event* event);

void Text_Input(GameInfo* gameInfo, SDL_Event* event);
void TextBar_Draw(GameInfo* gameInfo, int x, int y, int w, int h);

#endif