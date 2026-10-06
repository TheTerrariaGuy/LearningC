#define _POSIX_C_SOURCE 200809L
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<termios.h>
#include<unistd.h>
#include<signal.h>
#include"helper.h"

static struct termios old;
static bool terminalChanged = false;
static volatile sig_atomic_t stopped = 0;

bool canMoveInDialogue(void);
void selectX(int board[3][3], int r, int c);
void selectO(int board[3][3], int r, int c);

static void restoreTerminal(void) {
    if (terminalChanged) {
        tcsetattr(STDIN_FILENO, TCSANOW, &old);
        printf("\033[?25h" RESET);
        fflush(stdout);
    }
}

static void stopGame(int signalNumber) {
    (void)signalNumber;
    stopped = 1;
}

static double currentTime(void) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

int main(){
    int r = 0;
    int c = 0;
    int ar = 0;
    int ac = 0;
    int apr[6] = {-1};
    int apc[6] = {-1};
    double startTime = 0;
    double currTime = 0;
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

    char *options[5] = {
        "X",
        "O"
    };

    char currentRender[20][200];

    if (tcgetattr(STDIN_FILENO, &old) != 0) {
        fprintf(stderr, "Please run this game in an interactive terminal.\n");
        return 1;
    }
    struct termios raw = old;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    atexit(restoreTerminal);
    struct sigaction action = {0};
    action.sa_handler = stopGame;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) != 0) {
        perror("Could not set terminal input mode");
        return 1;
    }
    terminalChanged = true;
    printf("\033[?25l");

    while(1) {
        makeDialogueRender(&startingDialogue, &options, &r, &c, &currentRender);
        render(&currentRender);
        int ch = readInput();
        if (stopped || ch == 'q' || ch == 'Q' || ch == 4) return 0;
        handleMovement(&r, &c, &rmaxDiag, &cmaxDiag, &canMoveInDialogue, ch);
        handleSelectionDialogue(&r, &c, &userPlayer, ch);
        if (userPlayer != -1) break;
    }

    r = 0;
    c = 0;
    initBord(board);
    bool winner = false;
    bool won = false;
    while (1)
    {
        makeInGameRender(&board, userPlayer, turn, turn == userPlayer ? &r : &ar, turn == userPlayer ? &c : &ac, &currentRender);
        render(&currentRender);
        won = checkIfEnd(board, &winner);
        if (won || boardIsFull(board)) break;
        int ch = readInput();
        if (stopped || ch == 'q' || ch == 'Q' || ch == 4) return 0;
        if (turn == userPlayer) {
            handleMovement(&r, &c, &rmaxGame, &cmaxGame, &canMoveInDialogue, ch);
            handleSelectionIngame(&r, &c, &turn, board, turn == 1 ? selectX : selectO, ch);
        }
        else {
            currTime = currentTime();
            handleAIMove(&board, &ar, &ac, &apr, &apc, &userPlayer, &startTime, &currTime, &turn);
        }
        
    }
    if (won) {
        printf("\n%s\n", winner + 1 == userPlayer ? "You win!" : "I win!");
    } else {
        printf("\nIt's a draw!\n");
    }

    return 0;
}

bool canMoveInDialogue(void) {
    return true;
}

void selectX(int board[3][3], int r, int c) {
    board[r][c] = 1;
}

void selectO(int board[3][3], int r, int c) {
    board[r][c] = 2;
}
