#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<termios.h>
#include<unistd.h>
#include<helper.h>


int main(){
    int r = 0;
    int c = 0;
    int ar = 0;
    int ac = 0;
    int rmaxDiag = 2;
    int cmaxDiag = 1;
    int rmaxGame = 3;
    int cmaxGame = 3;
    int userPlayer = -1; // X = 1, O = 2
    int turn = 1; // flips between 1 and 2 (X and O)
    int board[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };

    char *startingDialogue = "\n\n\nLets play tic tac toe!\nWhich symbol do you want to play as?\n";

    char *options[] = {
        "X",
        "O"
    };

    char currentRender[20][200];

    while(1) {
        makeDialogueRender(&startingDialogue, &options, &r, &c, &currentRender);
        render(&currentRender);
        handleMovement(&r, &c, &rmaxDiag, &cmaxDiag, &canMoveInDialogue);
        handleSelectionDialogue(&r, &c, &userPlayer);
    }

    while (1)
    {
        makeInGameRender(&board, &options, &r, &c, &currentRender);
        render(&currentRender);
        if (turn == userPlayer) {
            handleMovement(&r, &c, &rmaxGame, &cmaxGame, &turn);
            handleSelectionIngame(&r, &c, &turn);
        }
        else {
            
        }
        
    }
    
    

    return 0;
}

bool canMoveInDialogue() {
    return true;
}

void selectX() {

}

void selectO() {

}

