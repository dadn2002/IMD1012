
#include "imports.h"



const char* Board[3][3] = {
        {"", "", ""},
        {"", "", ""},
        {"", "", ""}
    };
const char* AIBoard[3][3] = {
        {"", "", ""},
        {"", "", ""},
        {"", "", ""}
    };

bool reset_board(BoardType useThisBoard) {
    try {
        const char* default_board[3][3] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"}
        };

        // Assign default values to each element of the Board
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                useThisBoard[i][j] = default_board[i][j];
            }
        }
        return true;
    } catch (...) {
        return false;
    }
}

void print_board(BoardType useThisBoard = Board){
    // Print the updated board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%s ",useThisBoard[i][j]);
        }
        printf("\n");
    }
}

bool check_if_move_is_valid(BoardType useThisBoard, char* moveAttempt){
    try {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (strcmp(moveAttempt, useThisBoard[i][j]) == 0) {
                    //debug("Valid move");
                    return true;
                }
            }
        }
        //debug("Invalid move");
    } catch (...) {
        warn("check_if_move_is_valid unknown error");
    }
    return false;
}

bool place_move_on_board(BoardType useThisBoard, char* moveAttempt, const char* symbol){
    try {
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (strcmp(moveAttempt, useThisBoard[i][j]) == 0) {
                    debug("Placing symbol %s", symbol);
                    useThisBoard[i][j] = symbol;
                    return true;
                }
            }
        }
    } catch (...) {
        warn("place_move_on_board unknown error");
    }
    return false;
}

bool player_turn(BoardType useThisBoard){
    char playerInput[2];
    info("Your turn: ");
    scanf("%c", &playerInput);
    if (check_if_move_is_valid(useThisBoard, playerInput)) {
        place_move_on_board(useThisBoard, playerInput, playerSymbol);
    }
    return true;
}

bool ai_turn(BoardType useThisBoard){
    const char* positions[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int size = sizeof(positions) / sizeof(positions[0]);
    char aiInputBuffer[2]; // Buffer to store the converted str
    int randomIndex;
    
    do {
        randomIndex = rand() % size;
        const char* aiInput = positions[randomIndex];
        strcpy(aiInputBuffer, aiInput); // Convert const char* to char*
    } while (!check_if_move_is_valid(useThisBoard, aiInputBuffer));

    place_move_on_board(useThisBoard, aiInputBuffer, aiSymbol);
    return true;
}

int main(void){
    srand(time(nullptr));
    reset_board(Board);
    reset_board(AIBoard);
    print_board();
    while (0 < 1){
        player_turn(Board);
        print_board();
        ai_turn(Board);
        print_board();
        system("pause");
    }
    return 0;
}
