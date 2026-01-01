#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>

#include "player.h"
#include "text.h"
#include "scene.h"
#include "button.h"
#include "colors.h"
#include "ball.h"
#include "leaderboard.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

// fps cap
#define FPS 60
#define FPS_SBuffer 20
#define WINDOW_W 800
#define WINDOW_H 680

int main(int argc, char *argv[]){
    srand(time(NULL));

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        printf("SDL_mixer Error: %s\n", Mix_GetError());
    }

    // create window
    SDL_Window* window = SDL_CreateWindow(
        "Beardout",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, // imagine using static cordinates
        WINDOW_W, // width
        WINDOW_H, // height
        SDL_WINDOW_SHOWN
        //| SDL_WINDOW_RESIZABLE
    );

    // create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (TTF_Init() == -1){
        printf("TTF_Init error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Event event;
    int paddingX = 50;
    //int paddingY = 0;

    //Ball ball = Ball_Create(400, 500, 4);

    GameInfo gameInfo = {
        .renderer = renderer,
        .window = window,
        .windowWidth = WINDOW_W,
        .windowHeight = WINDOW_H,
        .font = TTF_OpenFont("./font/vcr.ttf", 24),
        .fontBigger = TTF_OpenFont("./font/vcr.ttf", 48),
        .sceneIndex = 0,
        .gameBoard = {
            .paddingX = paddingX,
            .paddingY = 0,
            .gameBoardRect = {
                .x = paddingX,
                .y = 50,
                .w = WINDOW_W - (paddingX * 2),
                //.h = WINDOW_H - (paddingY * 2)
                .h = WINDOW_H
            }
        },
        .inputBar = {
            .x = text_Center(gameInfo.windowWidth, 240),
            .y = 120,
            .w = 240,
            .h = 50
        },
        .playerLifes = 2,
        .playerScore = 0,
    };

    Leaderboard_Load(gameInfo.highScore, "save.bin");

    gameInfo.soundPlayer = Mix_LoadWAV("./sound/player.wav");
    gameInfo.soundBlock = Mix_LoadWAV("./sound/block.wav");
    gameInfo.soundWall = Mix_LoadWAV("./sound/wall.wav");
    gameInfo.soundType = Mix_LoadWAV("./sound/type.wav");
    gameInfo.soundOuch = Mix_LoadWAV("./sound/ouch.wav");

    /*
    --------------------------------------------------------------------------------------------
        IMPORTANT VARIABLES
    */
    int running = 1;

    // fps limit
    const int ms = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    //char fpstr[FPS_SBuffer];
    //sprintf(fpstr, "%d FPS", FPS);

    /*
    --------------------------------------------------------------------------------------------
        Text / button Area
    */
    
    TextObject myText = Text_Create(gameInfo.renderer, gameInfo.font, "Score: 0" + gameInfo.playerScore, white);
    TextObject lifes = Text_Create(gameInfo.renderer, gameInfo.font, "Lifes: 0" + gameInfo.playerLifes, white);
    TextObject name = Text_Create(gameInfo.renderer, gameInfo.font, gameInfo.playerName, white);
    TextObject beardout = Text_Create(gameInfo.renderer, gameInfo.fontBigger, "BEARDOUT", white);

    Button playButton = Button_Create(gameInfo.renderer, gameInfo.font, white, "PLAY", text_Center(gameInfo.windowWidth, 240), 
        gameInfo.inputBar.y + 60, 240, 50);


    Button quitButton = Button_Create(gameInfo.renderer, gameInfo.font, white, "QUIT", text_Center(gameInfo.windowWidth, 240),
        gameInfo.inputBar.y + 120, 240, 50);

    /*
    --------------------------------------------------------------------------------------------
        behold! my game loop
        arch linux gaming
    */

    while (running == 1){
        frameStart = SDL_GetTicks();

        // events
        while (SDL_PollEvent(&event)){
            // if window closed, running 0
            if (event.type == SDL_QUIT) running = 0;

            if (gameInfo.sceneIndex == 0) MainMenu_Input(&gameInfo, &event);
            else if (gameInfo.sceneIndex == 1) InGame_Input(&gameInfo, &event);
        }

        /*
        --------------------------------------------------------------------------------------------
            SCENES Render
        */
        switch (gameInfo.sceneIndex){
            case 0:
                MainMenu(&gameInfo);
                Text_Draw(gameInfo.renderer, beardout, text_Center(gameInfo.windowWidth, 220), 60);
                TextBar_Draw(&gameInfo, gameInfo.inputBar.x, gameInfo.inputBar.y, gameInfo.inputBar.w, gameInfo.inputBar.h);

                Button_Draw(gameInfo.renderer, &playButton, gameInfo.isBarEmpty);
                if(Button_Clicked(&playButton, &event, gameInfo.isBarEmpty) == 1 && !gameInfo.isBarEmpty){
                    Text_Destroy(&name); 
                    Block_Load(gameInfo.blocks, &gameInfo.blockCount, "blocks.txt", gameInfo.gameBoard.gameBoardRect.x, gameInfo.gameBoard.gameBoardRect.y);
                    name = Text_Create(gameInfo.renderer, gameInfo.font, gameInfo.playerName, white);
                    Ball ball = Ball_Create(390, 500, 4);
                    gameInfo.ball = ball;
                    gameInfo.playerScore = 0;
                    gameInfo.playerLifes = 2;

                    Player player = Player_Create(text_Center(gameInfo.gameBoard.gameBoardRect.w, 30), 640, 135, 20, 5, playerColor);
                    gameInfo.player = player;

                    gameInfo.sceneIndex = 1;
                }
                
                Button_Draw(gameInfo.renderer, &quitButton, 0);
                if(Button_Clicked(&quitButton, &event, 0) == 1) running = 0;
                break;

            case 1:
                InGame(&gameInfo);
                char scoreBuffer[32]; 
                char lifesBuffer[32];
                sprintf(scoreBuffer, "Score:%d", gameInfo.playerScore);
                sprintf(lifesBuffer, "Lifes:%d", gameInfo.playerLifes);
                Text_Destroy(&myText);
                Text_Destroy(&lifes);
                myText = Text_Create(gameInfo.renderer, gameInfo.font, scoreBuffer, white);
                lifes = Text_Create(gameInfo.renderer, gameInfo.font, lifesBuffer, white);

                Text_Draw(gameInfo.renderer, myText, text_Center(gameInfo.windowWidth, myText.w - 150), text_Center(40, myText.h));
                Text_Draw(gameInfo.renderer, lifes, text_Center(gameInfo.windowWidth, myText.w + 150), text_Center(40, myText.h));
                Text_Draw(gameInfo.renderer, name, 10, 10);
                break;
        
            default:
                break;
            }

        SDL_RenderPresent(gameInfo.renderer);
        // fps
        frameTime = SDL_GetTicks() - frameStart;
        if (ms > frameTime) SDL_Delay(ms - frameTime);
    }

    // destroy all hehe
    SDL_DestroyRenderer(gameInfo.renderer);
    Mix_FreeChunk(gameInfo.soundPlayer);
    Mix_FreeChunk(gameInfo.soundBlock);
    Mix_FreeChunk(gameInfo.soundWall);
    Mix_Quit();
    Text_Destroy(&myText);
    Text_Destroy(&name);
    Text_Destroy(&beardout);
    Button_Destroy(&playButton);
    Button_Destroy(&quitButton);
    TTF_CloseFont(gameInfo.font);
    SDL_DestroyWindow(gameInfo.window);
    SDL_Quit();

    return 0;
}
