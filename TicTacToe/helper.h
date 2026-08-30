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
void makeInGameRender(int (*board)[3][3], bool player, int *sr, int *sc, char (*lines)[20][200]);
void render(char (*lines)[20][200]);
void initBord(int arr[3][3]);
void handleMovement(int *r, int *c, int *rmax, int *cmax, bool (*canMove)(void));
void handleSelectionDialogue(int *r, int *c, void (*nextFunction[])(void));
int handleSelectionIngame(int *r, int *c, void (*select)(int, int));
typedef struct {
    int (*runThese[5])(int, int);
} gameStep;
