
#include "imports.h"


/*
    Generate .exe file to run this code
g++ -o .\main.exe .\main.cpp .\initialize.cpp
*/

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
int turn_counter = 1;

bool reset_board(BoardType useThisBoard) {
    try {
        const char* default_board[3][3] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"}
        };

        // Assign default values to each element of the Board
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
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
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%s ",useThisBoard[i][j]);
        }
        printf("\n");
    }
}

void finish_program(const char* winnerSymbol){
    if (strcmp(winnerSymbol, "X") == 0){
        info("You won!");
    }
    else {
        info("You lost!");
    }
    exit(0);
}

void call_draw(){
    info("The game is a draw!");
    exit(0);
}

bool check_if_move_is_valid(BoardType useThisBoard, char* moveAttempt){
    try {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
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
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (strcmp(moveAttempt, useThisBoard[i][j]) == 0) {
                    //debug("Placing symbol %s", symbol);
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
    scanf(" %1s", playerInput); // Read a single character as a string
    if (check_if_move_is_valid(useThisBoard, playerInput)) {
        place_move_on_board(useThisBoard, playerInput, playerSymbol);
        return true;
    }
    else {
        warn("Invalid move");
        system("pause");
        system("cls");
        info("Turn: %d", turn_counter);
        print_board();
        player_turn(useThisBoard);
        return false;
    }
}

bool ai_turn(BoardType useThisBoard){
    const char* positions[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int size = sizeof(positions) / sizeof(positions[0]);
    char aiInputBuffer[2]; // Buffer to store the converted str
    int randomIndex;
    
    info("Ai turn:");
    int number_attempts = 0;
    do {
        randomIndex = rand() % size;
        const char* aiInput = positions[randomIndex];
        strcpy(aiInputBuffer, aiInput); // Convert const char* to char*
        number_attempts++;
        if (number_attempts >= 10){
            call_draw();
        }
    } while (!check_if_move_is_valid(useThisBoard, aiInputBuffer));
    place_move_on_board(useThisBoard, aiInputBuffer, aiSymbol);
    return true;
}

bool cleanse_winning_lines(int winning_lines[8][3], int position) {
    // Cant define functions inside others in C saddly
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            if (winning_lines[i][j] == position) {
                winning_lines[i][j] = -1;
                break;
            }
        }
    }
    return true;
}

void print_winning_lines(int winning_lines[8][3]){
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%d ", winning_lines[i][j]);
        }
        printf("\n");
    }
}

bool check_if_winner(BoardType useThisBoard, const char* symbol){
    const char* collection_of_symbols[2] = {playerSymbol, aiSymbol};
    try {
        //debug("%s", symbol);
        int winning_lines[8][3] = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9},
            {1, 4, 7},
            {2, 5, 8},
            {3, 6, 9},
            {1, 5, 9},
            {3, 5, 7}
        };
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (strcmp(symbol, useThisBoard[i][j]) == 0) {
                    //debug("Detected Symbol %s", symbol);
                    cleanse_winning_lines(winning_lines, ((i + 1) + (j) * 3));
                }
            }
        }
        for (int i = 0; i < 8; i++) {
            bool allNegatives = true;
            for (int j = 0; j < 3; j++) {
                if (winning_lines[i][j] != -1) {
                    allNegatives = false;
                    break;
                }
            }
            if (allNegatives) {
                // Handle the case where the array is {-1, -1, -1}
                okay("Winner!");
                finish_program(symbol);
            }
        }
        //print_winning_lines(winning_lines);
        return true;
    } catch (...) {
        warn("place_move_on_board unknown error");
    }
    return false;
}

void turns(BoardType useThisBoard){
    while (true){
        system("cls");
        info("Turn: %d", turn_counter);
        print_board();
        player_turn(Board);
        print_board();
        check_if_winner(Board, playerSymbol);
        ai_turn(Board);
        print_board();
        check_if_winner(Board, aiSymbol);
        turn_counter++;
        system("pause");
    }
}

int main(void){
    srand(time(nullptr));
    reset_board(Board);
    reset_board(AIBoard); // For possible calculations
    turns(Board);
    return 0;
}
