#include "ball.h"
#include "scene.h"

Ball Ball_Create(int x, int y, float speed){
    Ball ball;
    ball.rect.x = x;
    ball.rect.y = y;
    ball.rect.w = 15;
    ball.rect.h = 15;
    ball.speed = speed;

    if (rand() % 2 == 0) ball.dirX = speed;
    else ball.dirX = -speed;

    ball.dirY = -speed; 
    return ball;
}

void Ball_Draw(SDL_Renderer* renderer, Ball* ball){
    SDL_SetRenderDrawColor(renderer, yellow.r, yellow.g, yellow.b, yellow.a);
    SDL_RenderFillRect(renderer, &ball->rect);
}

void Ball_Update(Ball* ball, GameInfo* gameInfo){
    
    ball->rect.x += (int)ball->dirX;
    ball->rect.y += (int)ball->dirY;
    float oldSpeed = ball->speed;

    // walls
    if (ball->rect.x <= gameInfo->gameBoard.gameBoardRect.x){
        ball->rect.x = gameInfo->gameBoard.gameBoardRect.x;
        ball->dirX = -ball->dirX;
        Mix_PlayChannel(-1, gameInfo->soundWall, 0);
    }
    if (ball->rect.x + ball->rect.w >= gameInfo->gameBoard.gameBoardRect.x + gameInfo->gameBoard.gameBoardRect.w){
        ball->rect.x = (gameInfo->gameBoard.gameBoardRect.x + gameInfo->gameBoard.gameBoardRect.w) - ball->rect.w;
        ball->dirX = -ball->dirX;
        Mix_PlayChannel(-1, gameInfo->soundWall, 0);
    }

    // top
    if (ball->rect.y <= gameInfo->gameBoard.gameBoardRect.y){
        ball->rect.y = gameInfo->gameBoard.gameBoardRect.y;
        ball->dirY = -ball->dirY;
        Mix_PlayChannel(-1, gameInfo->soundWall, 0);
    }
    
    // bottom - gg
    if (ball->rect.y > gameInfo->windowHeight) {
        gameInfo->playerLifes -= 1;
        Mix_PlayChannel(-1, gameInfo->soundOuch, 0);
        if (gameInfo->playerLifes > 0){
            Ball ballNew = Ball_Create(400, 500, 4);
            gameInfo->ball = ballNew;
        }
        else {
            if (Leaderboard_Update(gameInfo->highScore, gameInfo->playerName, gameInfo->playerScore)){
                Leaderboard_Save(gameInfo->highScore, "save.bin");
            }
            gameInfo->sceneIndex = 0;
        }
    }

if (SDL_HasIntersection(&ball->rect, &gameInfo->player.playerRect)){
        float playerCenter = gameInfo->player.playerRect.x + (gameInfo->player.playerRect.w / 2);
        float ballCenter = ball->rect.x + (ball->rect.w / 2);
        float diff = ballCenter - playerCenter;
        float perc = diff / (gameInfo->player.playerRect.w / 2);
        Mix_PlayChannel(-1, gameInfo->soundPlayer, 0);

        // speed
        ball->dirX = perc * ball->speed; 
        ball->dirY = -ball->speed; 
        ball->rect.y = gameInfo->player.playerRect.y - ball->rect.h;
    }

    for (int i = 0; i < gameInfo->blockCount; i++){
        Block* block = &gameInfo->blocks[i];
        
        if (SDL_HasIntersection(&ball->rect, &block->blockRect)){
            ball->dirY = -ball->dirY;
            
            gameInfo->blocks[i] = gameInfo->blocks[gameInfo->blockCount - 1];
            gameInfo->blockCount--;
            
            gameInfo->playerScore += 10;
            i--;
            Mix_PlayChannel(-1, gameInfo->soundBlock, 0);

            if (gameInfo->blockCount <= 0){
                TextObject winText = Text_Create(gameInfo->renderer, gameInfo->fontBigger, "VICTORY", green);
                TextObject readyText = Text_Create(gameInfo->renderer, gameInfo->font, "Get ready for another round!", white);

                Text_Draw(gameInfo->renderer, winText, text_Center(gameInfo->windowWidth, winText.w), 
                    text_Center(gameInfo->windowHeight, winText.h) - 30);
                          
                Text_Draw(gameInfo->renderer, readyText, text_Center(gameInfo->windowWidth, readyText.w), 
                    text_Center(gameInfo->windowHeight, readyText.h) + 30);

                Block_Load(gameInfo->blocks, &gameInfo->blockCount, "blocks.txt", gameInfo->gameBoard.gameBoardRect.x, 
                    gameInfo->gameBoard.gameBoardRect.y);

                SDL_RenderPresent(gameInfo->renderer);

                SDL_Delay(5000);

                Text_Destroy(&winText);
                Text_Destroy(&readyText);

                // next level harder
                Ball ballNew = Ball_Create(390, 500, ball->speed + 1.5);
                *ball = ballNew;
            }
            break; 
        }
    }
}