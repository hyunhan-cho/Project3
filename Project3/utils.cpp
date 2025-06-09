#include "utils.h"
#include <iostream>

#ifdef _WIN32
#include <cstdlib>
#include <windows.h>
#else
#include <unistd.h>
#endif

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\n계속하려면 엔터를 누르세요...";
    std::cin.ignore();
    std::cin.get();
}

void setColor(int color) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
#else
    std::cout << "\033[1;" << color << "m";
#endif
}
