// utils.cpp
#include "utils.h"
#include <iostream>
#include <cstdlib>

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    std::cout << "\033[2J\033[H";
#endif
}
