#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<unistd.h>
#include"helper.h"



// output: 0 = no, 1 = yes
// player: 1 = false, 2 = true
// grid: 0 = nothing, 1 = player 1, 2 = player 2
bool checkIfEnd(int board[3][3], bool *winner) {
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

bool boardIsFull(int board[3][3]) {
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (board[r][c] == 0) return false;
        }
    }
    return true;
}

void makeInGameRender(int (*board)[3][3], int player, int turn, int *sr, int *sc, char (*lines)[20][200]) {
    int currLine = 0;
    char header[4][40] = {
        "|===================|\n",
        "|  YOUR TURN  (X)   |\n",
        "|  MY TURN    (O)   |\n",
        "|===================|\n\n"
    };
    snprintf(header[1], sizeof(header[1]), "|  YOUR TURN  (%c)   |\n", player == 1 ? 'X' : 'O');
    snprintf(header[2], sizeof(header[2]), "|  MY TURN    (%c)   |\n", player == 1 ? 'O' : 'X');
    strcpy(header[turn == player ? 2 : 1], "");
    bool winner;
    if (checkIfEnd(*board, &winner) || boardIsFull(*board)) {
        strcpy(header[1], "|     GAME OVER     |\n");
        strcpy(header[2], "");
    }

    for (int i = 0; i < 4; i ++){
        if (header[i][0] == '\0') continue;
        strcpy((*lines)[currLine++], header[i]);
    }

        // Template

     // "   [ ]  [ ]  [ ]   \n\n"

     // "   [ ]  [ ]  [ ]   \n\n"

     // "   [ ]  [ ]  [ ]   \n\n"
    

    char *bottom[] = {
        "|===================|\n"
    };

    for (int r = 0; r < 3; r ++){
        char s[200] = "";
        strcat(s, "|   ");
        for (int c = 0; c < 3; c ++){
            bool selected = r == *sr && c == *sc;
            strcat(s, (selected ? YELLOW "[" RESET: RESET "["));

            strcat(s, (*board)[r][c] == 0 ? " " : ((*board)[r][c] == 1 ? BLUE "X" RESET : RED "O" RESET));
            
            strcat(s, (selected ? YELLOW "]" RESET: RESET "]"));

            strcat(s, "  ");
        }
        strcat(s, r < 2 ? " |\n\n" : " |\n");
        strcpy((*lines)[currLine++], s);
    }

    strcpy((*lines)[currLine++], bottom[0]);
    strcpy((*lines)[currLine++], "Arrow keys: move | Enter: select | Q: quit\n");

    strcpy((*lines)[currLine], "");
}

void makeDialogueRender(char **prompt, char *(*options)[5], int *r, int *c, char (*lines)[20][200]) {
    (void)c;
    int currLine = 0;
    snprintf((*lines)[currLine++], 200, "%s\n", *prompt);
    for (int i = 0; i < 5; i++) {
        if ((*options)[i] == NULL || (*options)[i][0] == '\0') break;
        bool selected = (*r == i);
        snprintf((*lines)[currLine++], 200, "%s%s\n", selected ? RESET " [" YELLOW ">" RESET "] " : " [ ] ", (*options)[i]);
    }
    strcpy((*lines)[currLine++], "\nArrow keys: move | Enter: select | Q: quit\n");
    strcpy((*lines)[currLine], "");
}

void render(char (*lines)[20][200]) {
    printf("\033[2J\033[H");
    printf("\n\n\n");
    int currLine = 0;
    while (currLine < 20 && (*lines)[currLine][0] != '\0'){
        printf("%s", (*lines)[currLine++]);
    }
    fflush(stdout);
}

void initBord(int arr[3][3]) {
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j ++){
            arr[i][j] = 0;
        }
    }
}

// Read once per frame so movement and selection see the same key.
// Arrow keys use values above the range of ordinary characters.
int readInput(void) {
    static int escapeState = 0;
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) != 1) return -1;
    if (ch == '\033') {
        escapeState = 1;
        return -1;
    }
    if (escapeState == 1 && (ch == '[' || ch == 'O')) {
        escapeState = 2;
        return -1;
    }
    if (escapeState == 2) {
        escapeState = 0;
        if (ch >= 'A' && ch <= 'D') return 256 + ch;
    }
    escapeState = 0;
    return ch;
}

void handleMovement(int *r, int *c, int *rmax, int *cmax, bool (*canMove)(void), int ch) {
    if (!canMove()) return;
        if (ch >= 256) {
            int direction = ch - 256;

            if (direction == 'A') { // up
                (*r) --;
                *r %= *rmax;
                if (*r < 0)
                    *r += *rmax;
            }

            if (direction == 'B') { // down
                (*r) ++;
                *r %= *rmax;
                if (*r < 0)
                    *r += *rmax;
            }

            if (direction == 'D') { // left
                (*c) --;
                *c %= *cmax;
                if (*c < 0)
                    *c += *cmax;
            }

            if (direction == 'C') { // right
                (*c) ++;
                *c %= *cmax;
                if (*c < 0)
                    *c += *cmax;
            }
        }
}

void handleSelectionDialogue(int *r, int *c, int *player, int ch) {
    (void)c;
    if (ch == '\n' || ch == '\r') {
        *player = *r + 1;
    }
}

void handleSelectionIngame(int *r, int *c, int *currTurn, int board[3][3], void (*select)(int [3][3], int, int), int ch) {
    if ((ch == '\n' || ch == '\r') && board[*r][*c] == 0){
        select(board, *r, *c);
        *currTurn = 3 - *currTurn;
    }
}

// -1 = not decided, -2 = end
void handleAIMove(int (*board)[3][3], int *ar, int *ac, int (*apr)[6], int (*apc)[6], int *userPlayer, double *startTime, double *currTime, int *turn){
    if ((*apr)[0] == -1) {
        int tr, tc;
        move last = {0, -1, -1};
        move m = findOptimalMove(*board, 3 - *userPlayer, userPlayer, last);
        if (m.r < 0 || m.c < 0) return;
        tr = *ar;
        tc = *ac;
        int step = 0;
        (*apr)[step] = tr;
        (*apc)[step++] = tc;
        while (tr != m.r || tc != m.c) {
            if (tr != m.r) tr += tr < m.r ? 1 : -1;
            else tc += tc < m.c ? 1 : -1;
            (*apr)[step] = tr;
            (*apc)[step++] = tc;
        }
        (*apr)[step] = -2;
        (*apc)[step] = -2;
        *startTime = *currTime;
    }
    int dTime = (int)((*currTime - *startTime) / 0.25);
    for (int step = 0; step <= dTime && step < 6; step++) {
        if ((*apr)[step] == -2) {
            (*board)[*ar][*ac] = 3 - *userPlayer;
            *turn = *userPlayer;
            (*apr)[0] = -1;
            (*apc)[0] = -1;
            return;
        }
        *ar = (*apr)[step];
        *ac = (*apc)[step];
    }
}

move findOptimalMove(int board[3][3], int currMover, int *userPlayer, move last) {
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

    move movest[9] = {{0}};
    move movesw[9] = {{0}};
    move movesl[9] = {{0}};
    int cw = 0;
    int ct = 0;
    int cl = 0;

    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 3; j ++){
            if (board[i][j] != 1 && board[i][j] != 2) {
                // new board
                int newBoard[3][3] = {{0}};

                for (int r = 0; r < 3; r ++){
                    for (int c = 0; c < 3; c ++)
                        newBoard[r][c] = board[r][c];
                }
                newBoard[i][j] = (currMover);

                move l = {-2, i, j};
                move m = findOptimalMove(newBoard, 3 - currMover, userPlayer, l);
                m.wlt = -m.wlt;
                m.r = i;
                m.c = j;

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
    else return movesl[0];

}
