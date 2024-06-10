#pragma once

#include <windows.h>
#include <stdio.h>
#include <string>
#include <cstdlib> // for rand(), srand() and exit()
#include <ctime>   // for time()

#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf("[i] " msg "\n", ##__VA_ARGS__)
#define debug(msg, ...) printf("[%] " msg "\n", ##__VA_ARGS__)

// Use a pointer to a pointer to const char for the board type.
using BoardType = const char***;

struct UsedLines {
    int* row;
    int* col;
    int size;
};

const char* aiSymbol     = "Q";
