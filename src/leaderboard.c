#include "leaderboard.h"

void Leaderboard_Load(HighScore scores[LEADERBOARD_Size], const char* fileName){
    FILE* file = fopen(fileName, "rb");
    if (file){
        fread(scores, sizeof(HighScore), LEADERBOARD_Size, file);
        fclose(file);
    }
    else {
        for (int i = 0; i < LEADERBOARD_Size; i++){
            strcpy(scores[i].name, "");
            scores[i].score = 0;
        }
    }
}

void Leaderboard_Save(HighScore scores[LEADERBOARD_Size], const char* fileName){
    FILE* file = fopen(fileName, "wb");
    if (file){
        fwrite(scores, sizeof(HighScore), LEADERBOARD_Size, file);
        fclose(file);
    }
}

int Leaderboard_Update(HighScore scores[LEADERBOARD_Size], char* name, int score){
    if (score <= scores[LEADERBOARD_Size - 1].score) return 0;

    else {
        for (int i = 0; i < LEADERBOARD_Size; i++){
        
            if (score > scores[i].score){
                for (int j = LEADERBOARD_Size - 1; j > i; j--) scores[j] = scores[j - 1];
            
                strcpy(scores[i].name, name);
                scores[i].score = score;

                return 1;
            }
        }
    }
    
    return 0;
}
