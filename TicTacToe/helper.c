#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"


// output: 0 = no, 1 = yes
// player: 1 = false, 2 = true
// grid: 0 = nothing, 1 = player 1, 2 = player 2
bool checkIfEnd(int *board[], bool *winner) {
    pattern pts[8] = {
        {0,0, 1,0, 2,0},
        {0,1, 1,1, 2,1},
        {0,2, 1,2, 2,2},

        {0,0, 0,1, 0,2},
        {1,0, 1,1, 1,2},
        {2,0, 2,1, 2,2},

        {0,0, 1,1, 2,2},
        {0,2, 1,1, 2,0}
    };

    for (int i = 0; i < 8; i ++) {
        pattern p = pts[i];
        int val = board[p.a1][p.a2];
        if (val == 0) continue;
        if (val == board[p.b1][p.b2] && val == board[p.c1][p.c2]) {
            *winner = val - 1;
            return true;
        }
    }

    return false;
}

void makeInGameRender(int *(*board)[], bool player, int *sr, int *sc, char *(*lines)[]) {
    int currLine = 0;
    char header[4][20] = {
        "|=================|\n",
        "|  YOUR TURN  (X) |\n",
        "|  MY TURN    (O) |\n",
        "|=================|\n\n"
    };
    strcpy(header[2-player], "");

    for (int i = 0; i < 3; i ++){
        strcpy((*lines)[currLine++], header[i]);
    }

        // Template

     // "   [ ]  [ ]  [ ]   \n\n"

     // "   [ ]  [ ]  [ ]   \n\n"

     // "   [ ]  [ ]  [ ]   \n\n"
    

    char *bottom[] = {
        "|=================|"
    };

    for (int r = 0; r < 3; r ++){
        char s[200] = "";
        strcat(s, "|   ");
        for (int c = 0; c < 3; c ++){
            bool selected = r == sr && c == sc;
            strcat(s, (selected ? YELLOW "[" RESET: RESET "["));

            strcat(s, (*board)[r][c] == 1 ? BLUE "X" RESET : RED "O" RESET);
            
            strcat(s, (selected ? YELLOW "]" RESET: RESET "]"));

            strcat(s, "  ");
        }
        strcat(s, r < 2 ? " |\n\n" : " |\n");
        strcpy((*lines)[currLine++], s);
    }

    strcpy((*lines)[currLine++], bottom[0]);

    strcpy((*lines)[currLine], "");
}

void makeDialogueRender(char **prompt, char *(*options)[], int *r, int *c, char *(*lines)[]) {
    int currLine = 0;
    snprintf((*lines)[currLine++], 200, "%s\n", prompt);
    for (int i = 0; i < 5; i++) {
        if ((*options)[i] == NULL || (*options)[i][0] == '\0') break;
        bool selected = (*r == i);
        snprintf((*lines)[currLine++], 200, "%s%s\n", selected ? RESET " [" YELLOW ">" RESET "] " : " [ ] ", (*options)[i]);
    }
    strcpy((*lines)[currLine], "");
}

void render(char *(*lines)[]) {
    printf("\n\n\n");
    int currLine = 0;
    while ((*lines)[currLine][0] != "\0" && currLine < 20){
        printf("%s", (*lines)[currLine++]);
    }
}

void initBord(int *arr[]) {
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j ++){
            arr[i][j] = 0;
        }
    }
}

void handleMovement(int *r, int *c, int *rmax, int *cmax, int) {
    if (!canMove()) return;
    int c = getchar();
        if (c == '\033') {
            getchar(); // skip '['
            int direction = getchar();

            if (direction == 'A') { // up
                *r --;
                *r %= *rmax;
                if (*r < 0)
                    *r += *rmax;
            }

            if (direction == 'B') { // down
                *r ++;
                *r %= *rmax;
                if (*r < 0)
                    *r += *rmax;
            }

            if (direction == 'D') { // left
                *c --;
                *c %= *cmax;
                if (*c < 0)
                    *c += *cmax;
            }

            if (direction == 'C') { // right
                *c ++;
                *c %= *cmax;
                if (*c < 0)
                    *c += *cmax;
            }
        }
}

void handleSelectionDialogue(int *r, int *c, int *player) {
    int ch = getchar();
    if (ch == '\n') {
        *player = *r + 1;
    }
}

void handleSelectionIngame(int *r, int *c, int *currTurn) {
    int ch = getchar(); 
    if (ch == '\n'){
        *currTurn = 3 - *currTurn;
    }
}

// -1 = not decided, -2 = end
void handleAIMove(int *(*board)[], int *ar, int *ac, int (*apr)[], int (*apc)[], int *userPlayer, float *startTime, float *currTime){
    int dTime = *currTime - *startTime;
    if ((*apr)[0] == -1) {
        int tr, tc;

    }
    if (dTime > 4) return;
    if ((*apr)[dTime] == -1){

    }
}

move findOptimalMove(int *board[], int currMover, int *userPlayer, move last) {
    bool win;
    if (checkIfEnd(board, &win)){
        if (win + 1 == currMover) {
            last.wlt = 1;
            return last;
        } else {
            last.wlt = -1;
            return last;
        }
    }

    move movest[9] = {};
    move movesw[9] = {};
    move movesl[9] = {};
    int cw = 0;
    int ct = 0;
    int cl = 0;

    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j ++){
            if (board[i][j] != 1 && board[i][j] != 2) {
                // new board
                int newBoard[3][3] = {};

                for (int r = 0; r < 3; r ++){
                    for (int c = 0; c < 3; c ++)
                        newBoard[r][c] = board[r][c];
                }
                newBoard[i][j] = (currMover);

                move l = {-2, i, j};
                move m = findOptimalMove(newBoard, 3 - currMover, userPlayer, l);

                if (m.wlt < 0) movesl[cl++] = m;
                else if (m.wlt == 0) movest[ct++] = m;
                else movesw[cw++] = m;
                
            }
        }
    }

    // return condition
    if (cw + ct + cl == 0) {
        last.wlt = 0;
        return last;
    }

    if (cw != 0) return movesw[0];
    else if (ct != 0) return movest[0];
    else if (cl != 0) return movesl[0];

}

typedef struct
{
    int wlt;
    int r;
    int c;
} move;


typedef struct {
    int a1, a2;
    int b1, b2;
    int c1, c2;
} pattern;
