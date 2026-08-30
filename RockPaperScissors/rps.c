#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<termios.h>
#include<unistd.h>

int main() {
    // char s[50] = "Hello world";
    // char b[3];
    // snprintf(b, 3, "%d", TEN);
    // strcat(s, b);
    // printf("%s\n", s);

    // int age;
    // printf("Enter your age: ");
    // scanf("%d", &age);
    // char printout[100];
    // snprintf(printout, 100, "Age: %d.", age);
    // printf(printout);

    srand(time(NULL)); // sets random seed based on time

    int choice = rand() % 3;

    char choiceName[10];

    switch (choice) {
        case 0:
            strcpy(choiceName, "Rock");
            break;
        case 1:
            strcpy(choiceName, "Paper");
            break;
        case 2:
            strcpy(choiceName, "Scissors");
            break;
    }

    struct termios old;

    tcgetattr(STDIN_FILENO, &old);

    struct termios raw = old;

    raw.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &raw);

    int currentSelection = 0;

    char choices[3][15] = {
        " [ ]  Rock",
        " [ ]  Paper",
        " [ ]  Scissors"
    };

    

    while (1) {
        printf("\033[2J\033[H"); // This is a scary command

        for (int r = 0; r < 3; r ++) {
            if (r == currentSelection){
                choices[r][2] = '>';
            } else {
                choices[r][2] = ' ';
            }
        }
        printf("\n\n\n");
        printf("Lets play Rock Paper Scissors!\n");
        for (int i = 0; i < 3; i ++) {
            printf("%s\n", choices[i]);
        }

        // input handling
        int c = getchar();

        if (c == '\033') {
            getchar(); // skip '['
            int direction = getchar();

            if (direction == 'A') { // up
                currentSelection --;
                currentSelection %= 3;
                if (currentSelection < 0)
                    currentSelection += 3;
            }

            if (direction == 'B') {
                currentSelection ++;
                currentSelection %= 3;
                if (currentSelection < 0)
                    currentSelection += 3;
            }
        }

        if (c == '\n')
            break;
    }
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    char playerChoice[10];

    switch (currentSelection) {
        case 0:
            strcpy(playerChoice, "Rock");
            break;
        case 1:
            strcpy(playerChoice, "Paper");
            break;
        case 2:
            strcpy(playerChoice, "Scissors");
            break;
    }

    printf("You chose: %s\n", playerChoice);
    printf("I chose: %s\n", choiceName);

    if (choice == currentSelection) {
        printf("We tied!");
        return 0;
    }

    if (choice == 0 && currentSelection == 2) {
        printf("I win!");
    } else if (choice == 2 && currentSelection == 0) {
        printf("You win!");
    } else {
        if (currentSelection > choice) {
            printf("You win!");
        } else {
            printf("I win!");
        }
    }
    
    return 0;
}