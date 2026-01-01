#include "blocks.h"
#include "colors.h"

Block Block_Create(int x, int y, int w, int h, SDL_Color blockColor){
    Block block = {
        .blockRect.x = x,
        .blockRect.y = y,
        .blockRect.w = w,
        .blockRect.h = h,
        .blockColor = blockColor
    };

    return block;
};
void Block_Draw(SDL_Renderer* renderer, Block* block){
    SDL_SetRenderDrawColor(renderer, block->blockColor.r, block->blockColor.g, block->blockColor.b, block->blockColor.a);
    SDL_RenderFillRect(renderer, &block->blockRect);

    SDL_SetRenderDrawColor(renderer, black.r, black.g, black.b, black.a);
    SDL_RenderDrawRect(renderer, &block->blockRect);
};


void Block_Load(Block* blocksArray, int* blockCount, char* path, int startX, int startY){
    FILE* file = fopen(path, "r");
    if (file == NULL) return;

    int blockYCor = 0;
    int blockXCor = 0;
    int blockY = 0;
    int blockX = 0;
    
    int blockW = 50;
    int blockH = 25;

    int count = 0;
    char temp[16];

    while (fgets(temp, 16, file)){
        for (int i = 0; temp[i] != '\0'; i++){
            if(temp[i] == 'O'){
                blockX = startX + (blockXCor * blockW);
                blockY = startY + (blockYCor * blockH);
                blocksArray[count] = Block_Create(blockX, blockY, blockW, blockH, ubuntu);
                count++;
                blockXCor++;
            }
            else if (temp[i] == '\n'){
                blockYCor++;
                blockXCor = 0;
            }
            else if (temp[i] == ' ') blockXCor++;    
        }

        *blockCount = count;
    }

    fclose(file);
}
