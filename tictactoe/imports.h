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

// pointer to an array of 3 pointers to const char.
using BoardType = const char*(*)[3]; 

const char* playerSymbol = "X";
const char* aiSymbol     = "O";