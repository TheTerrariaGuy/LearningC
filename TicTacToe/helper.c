#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define RESET   "\033[0m"


// output: 0 = no, 1 = yes
// player: 1 = false, 2 = true
// grid: 0 = nothing, 1 = player 1, 2 = player 2
bool checkIfEnd(int *board[], bool *winner) {
    struct pattern pts[8] = {
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
        struct pattern p = pts[i];
        int val = board[p.a1][p.a2];
        if (val == 0) continue;
        if (val == board[p.b1][p.b2] && val == board[p.c1][p.c2]) {
            *winner = val - 1;
            return true;
        }
    }

    return false;
}

void makeInGameRender(int (*board)[3][3], bool player, int *sr, int *sc, char (*lines)[20][200]) {
    int currLine = 0;
    char header[4][20] = {
        "\n\n|=================|\n",
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

void makeDialogueRender(char *prompt, char *responses[], int *r, int *c) {
    
}

void render(char (*lines)[20][200]) {
    int currLine = 0;
    while ((*lines)[currLine][0] != "\0" && currLine < 20){
        printf("%s", (*lines)[currLine++]);
    }
}

void initBord(int arr[3][3]) {
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j ++){
            arr[i][j] = 0;
        }
    }
}

void handleMovement(int *r, int *c, int *rmax, int *cmax, bool (*canMove)(void)) {
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

void handleSelectionDialogue(int *r, int *c, void (*nextFunction[])(void)) {
    int ch = getchar();
    if (ch == '\n') {
        nextFunction[*r]();
    }
}

int handleSelectionIngame(int *r, int *c, void (*select)(int, int)) {
    int ch = getchar(); 
    if (ch == '\n'){
        select(*r, *c);
    }
}

struct gameStep {
    int (*runThese[5])(int, int);
};

struct pattern {
    int a1, a2;
    int b1, b2;
    int c1, c2;
};
