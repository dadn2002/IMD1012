#include "imports.h"

// Saddly i could only think of redefining my macro :(
#undef debug
#define debug(msg, ...) do { if (debugEnabled) printf("[%] " msg "\n", ##__VA_ARGS__); } while (0)

int size_of_board_nxn;
bool debugEnabled = false;
UsedLines usedLines;

/*
    Generate .exe file to run this code
    g++ -o .\main.exe .\main.cpp .\initialize.cpp
*/

void how_big_is_the_board(void) {
    info("Insert how big (NxN) is the board");
    scanf("%d", &size_of_board_nxn);
}

BoardType generate_board(void) {
    BoardType useThisBoard = (BoardType)malloc(size_of_board_nxn * sizeof(const char**));
    for (int i = 0; i < size_of_board_nxn; i++) {
        useThisBoard[i] = (const char**)malloc(size_of_board_nxn * sizeof(const char*));
        for (int j = 0; j < size_of_board_nxn; j++) {
            useThisBoard[i][j] = "."; // Initialize with dot representing an empty cell
        }
    }
    return useThisBoard;
}

void create_used_lines_variable() {
    usedLines.size = size_of_board_nxn;
    usedLines.row = (int*)malloc(size_of_board_nxn * sizeof(int));
    usedLines.col = (int*)malloc(size_of_board_nxn * sizeof(int));

    for (int i = 0; i < size_of_board_nxn; ++i) {
        usedLines.row[i] = -1;
        usedLines.col[i] = -1;
    }
    usedLines.size = 0;
}

void print_usedLines() {
    for (int i = 0; i < usedLines.size; ++i) {
        info("%d %d", usedLines.row[i], usedLines.col[i]);
    }
    printf("\n");
}

void update_usedLines(int row, int col) {
    usedLines.row[usedLines.size] = row;
    usedLines.col[usedLines.size] = col;
    usedLines.size++;
}

void reset_board(BoardType useThisBoard) {
    for (int i = 0; i < size_of_board_nxn; i++) {
        for (int j = 0; j < size_of_board_nxn; j++) {
            useThisBoard[i][j] = ".";
        }
    }
    usedLines.size = 0;
}

void print_board(BoardType useThisBoard) {
    for (int i = 0; i < size_of_board_nxn; i++) {
        for (int j = 0; j < size_of_board_nxn; j++) {
            printf("%s ", useThisBoard[i][j]);
        }
        printf("\n");
    }
}

bool check_if_game_won(BoardType useThisBoard) {
    int number_of_queens = 0;
    for (int i = 0; i < size_of_board_nxn; i++) {
        for (int j = 0; j < size_of_board_nxn; j++) {
            if (strcmp(useThisBoard[i][j], "Q") == 0) {
                number_of_queens++;
            }
        }
    }
    return number_of_queens == size_of_board_nxn;
}

bool check_valid_move_queenAttacking(BoardType useThisBoard, int row, int col) {
    for (int i = 0; i < usedLines.size; ++i) {
        if (usedLines.row[i] == row || usedLines.col[i] == col) {
            return false;
        }
    }
    for (int i = 0; i < usedLines.size; ++i) {
        if (abs(usedLines.row[i] - row) == abs(usedLines.col[i] - col)) {
            return false;
        }
    }
    return true;
}

bool check_valid_move_insideBoard(BoardType useThisBoard, int row, int col) {
    return (row >= 0 && row < size_of_board_nxn && col >= 0 && col < size_of_board_nxn);
}

bool check_valid_move_emptyCell(BoardType useThisBoard, int row, int col) {
    return (strcmp(useThisBoard[row][col], ".") == 0);
}

bool check_valid_move_allCases(BoardType useThisBoard, int row, int col) {
    if (!check_valid_move_insideBoard(useThisBoard, row, col)) {
        debug("check_valid_move_insideBoard");
        return false;
    }
    if (!check_valid_move_emptyCell(useThisBoard, row, col)) {
        debug("check_valid_move_emptyCell");
        return false;
    }
    if (!check_valid_move_queenAttacking(useThisBoard, row, col)) {
        debug("check_valid_move_queenAttacking");
        return false;
    }
    return true;
}

int try_random_move(BoardType useThisBoard, int* available_positions, int* available_count) {
    if (*available_count == 0) {
        debug("No more available positions.");
        return 0;
    }

    int index = rand() % (*available_count);
    int pos = available_positions[index];
    int row = pos / size_of_board_nxn;
    int col = pos % size_of_board_nxn;

    debug("T %d ", row * size_of_board_nxn + col);

    if (check_valid_move_allCases(useThisBoard, row, col)) {
        //okay("Found valid position: (%d, %d)", row, col);
        update_usedLines(row, col);
        useThisBoard[row][col] = aiSymbol;

        // Remove the chosen position from available_positions
        available_positions[index] = available_positions[--(*available_count)];
        return 1;
    }

    // Remove the invalid position from available_positions
    available_positions[index] = available_positions[--(*available_count)];
    return try_random_move(useThisBoard, available_positions, available_count);
}

void free_memory(BoardType useThisBoard) {
    for (int i = 0; i < size_of_board_nxn; i++) {
        free(useThisBoard[i]);
    }
    free(useThisBoard);
    free(usedLines.row);
    free(usedLines.col);
}

BoardType starting_routine() {
    srand(time(NULL));
    how_big_is_the_board();
    BoardType Board = generate_board();
    reset_board(Board);
    return Board;
}

void main_game(BoardType useThisBoard) {
    int number_of_attempts = 1;

    int* available_positions = (int*)malloc(size_of_board_nxn * size_of_board_nxn * sizeof(int));
    for (int i = 0; i < size_of_board_nxn * size_of_board_nxn; ++i) {
        available_positions[i] = i;
    }
    int available_count = size_of_board_nxn * size_of_board_nxn;

    while (true) {
        info("Attempt %d", number_of_attempts);
        while (true) {
            if (!try_random_move(useThisBoard, available_positions, &available_count)) {
                warn("Failed to generate max capacity board");
                print_board(useThisBoard);
                reset_board(useThisBoard);
                free(usedLines.row);
                free(usedLines.col);
                create_used_lines_variable();

                // Reset available positions
                for (int i = 0; i < size_of_board_nxn * size_of_board_nxn; ++i) {
                    available_positions[i] = i;
                }
                available_count = size_of_board_nxn * size_of_board_nxn;
                break;
            }
            if (check_if_game_won(useThisBoard)) {
                printf("\n");
                okay("The game is won!\n");
                print_board(useThisBoard);
                free_memory(useThisBoard);
                free(available_positions);
                return;
            }
            //print_board(useThisBoard);
        }
        number_of_attempts++;
    }
}

void setup_debug_mode(int argc, char* argv[]){
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-debug") == 0) {
            debugEnabled = true;
            printf("Debug mode enabled.\n");
        }
    }
}

int main(int argc, char* argv[]) {
    system("cls");
    setup_debug_mode(argc, argv);
    BoardType Board = starting_routine();
    create_used_lines_variable();
    main_game(Board);
    return 0;
}