#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define RED
#define GREEN
#define YELLOW
#define BLUE
#define RESET

bool checkIfEnd(int *board[], bool *winner);
void makeInGameRender(int (*board)[][], bool player, int *sr, int *sc, char (*lines)[][]);
void makeDialogueRender(char *prompt, char *options[5], int *r, int *c, char (*lines)[][]);
void render(char (*lines)[][]);
void initBord(int arr[][]);
void handleMovement(int *r, int *c, int *rmax, int *cmax, bool (*canMove)(void));
void handleSelectionDialogue(int *r, int *c, int *player);
void handleSelectionIngame(int *r, int *c, void (*select)(int, int));