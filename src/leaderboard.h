#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdio.h>
#include <string.h>

// only top 8 players
#define LEADERBOARD_Size 8
#define NAME_Buffer 17

typedef struct {
    char name[NAME_Buffer];
    int score;
} HighScore;

void Leaderboard_Load(HighScore scores[LEADERBOARD_Size], const char* fileName);
void Leaderboard_Save(HighScore scores[LEADERBOARD_Size], const char* fileName);
int Leaderboard_Update(HighScore scores[LEADERBOARD_Size], char* name, int score);

#endif