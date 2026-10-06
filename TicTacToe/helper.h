#ifndef TICTACTOE_HELPER_H
#define TICTACTOE_HELPER_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"

typedef struct {
    int wlt;
    int r;
    int c;
} move;

typedef struct {
    int a1, a2;
    int b1, b2;
    int c1, c2;
} pattern;

bool checkIfEnd(int board[3][3], bool *winner);
bool boardIsFull(int board[3][3]);
void makeInGameRender(int (*board)[3][3], int player, int turn, int *sr, int *sc, char (*lines)[20][200]);
void makeDialogueRender(char **prompt, char *(*options)[5], int *r, int *c, char (*lines)[20][200]);
void render(char (*lines)[20][200]);
void initBord(int arr[3][3]);
int readInput(void);
void handleMovement(int *r, int *c, int *rmax, int *cmax, bool (*canMove)(void), int ch);
void handleSelectionDialogue(int *r, int *c, int *player, int ch);
void handleSelectionIngame(int *r, int *c, int *currTurn, int board[3][3], void (*select)(int [3][3], int, int), int ch);
void handleAIMove(int (*board)[3][3], int *ar, int *ac, int (*apr)[6], int (*apc)[6], int *userPlayer, double *startTime, double *currTime, int *turn);
move findOptimalMove(int board[3][3], int currMover, int *userPlayer, move last);

#endif
