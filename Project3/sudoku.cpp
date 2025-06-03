#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>
#include <string>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include "sudoku.h"
#include "utils.h"

using namespace std;

#ifdef _WIN32
int kbhit_nonblock() {
    return _kbhit();
}
#else
int kbhit_nonblock() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
#endif

// 이하 게임 함수 정의들

void printGrid(const vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid) {
    cout << "  +===================================+\n";
    for (int i = 0; i < 9; i++) {
        cout << "  | ";
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                cout << "  | ";
            }
            else if (fixedGrid[i][j] != 0) {
                cout << "\033[34m" << grid[i][j] << "\033[0m | ";
            }
            else {
                cout << "\033[33m" << grid[i][j] << "\033[0m | ";
            }
        }
        cout << "\n";
        if ((i + 1) % 3 == 0 && i != 8)
            cout << "  |===========|===========|===========|\n";
        else if (i != 8)
            cout << "  |-----------|-----------|-----------|\n";
    }
    cout << "  +===================================+\n";
}

bool isValidMove(const vector<vector<int>>& grid, int row, int col, int num) {
    int startRow = 3 * (row / 3);
    int startCol = 3 * (col / 3);
    for (int x = 0; x < 9; x++) {
        if (grid[row][x] == num || grid[x][col] == num ||
            grid[startRow + x / 3][startCol + x % 3] == num) {
            return false;
        }
    }
    return true;
}

bool solveSudoku(vector<vector<int>>& grid) {
    std::random_device rd;
    std::mt19937 g(rd());
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                vector<int> numbers = { 1,2,3,4,5,6,7,8,9 };
                std::shuffle(numbers.begin(), numbers.end(), g);
                for (int num : numbers) {
                    if (isValidMove(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid)) return true;
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

void initializeGrid(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid, int clues) {
    solveSudoku(grid);
    int remaining = 81 - clues;
    while (remaining > 0) {
        int i = rand() % 9;
        int j = rand() % 9;
        if (grid[i][j] != 0) {
            grid[i][j] = 0;
            remaining--;
        }
    }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            fixedGrid[i][j] = grid[i][j];
}

bool validateNumberInput() {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool isSolvableMove(vector<vector<int>> grid, int row, int col, int num) {
    grid[row][col] = num;
    return solveSudoku(grid);
}

void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid, int timeLimitMinutes) {
    auto startTime = std::chrono::steady_clock::now();
    int wrongAttempts = 0;
    int row = 0, col = 0, num = 0;
    int inputStep = 0;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        int totalRemainingSeconds = timeLimitMinutes * 60 - static_cast<int>(
            std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count());

        if (totalRemainingSeconds <= 0) {
            clearScreen();
            cout << "\n시간 종료! 퍼즐을 제시간에 풀지 못했습니다.\n";
            printGrid(grid, fixedGrid);
            pauseScreen();
            break;
        }

        int minutes = totalRemainingSeconds / 60;
        int seconds = totalRemainingSeconds % 60;

        clearScreen();
        cout << "남은 시간: " << minutes << "분 " << seconds << "초\n";
        cout << "오답 횟수: " << wrongAttempts << " / 3\n";
        cout << "ESC를 누르면 메인 메뉴로 돌아갑니다.\n\n";
        printGrid(grid, fixedGrid);

        if (wrongAttempts >= 3) {
            cout << "\n오답 3회 초과! 게임 오버.\n";
            pauseScreen();
            break;
        }

        if (inputStep == 0) cout << "\n행 입력 (1-9, 자동 풀이:-1): ";
        else if (inputStep == 1) cout << "열 입력 (1-9): ";
        else cout << "숫자 입력 (1-9): ";

        std::string inputStr;
        bool gotInput = false, isEscPressed = false;

        while (!gotInput) {
#ifdef _WIN32
            if (_kbhit()) {
                int ch = _getch();
                if (ch == 27) {
                    isEscPressed = true;
                    break;
                }
                ungetc(ch, stdin);
                cin >> inputStr;
                gotInput = true;
            }
#else
            if (kbhit_nonblock()) {
                int ch = getchar();
                if (ch == 27) {
                    isEscPressed = true;
                    break;
                }
                ungetc(ch, stdin);
                cin >> inputStr;
                gotInput = true;
            }
#endif
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (isEscPressed) {
            cout << "\nESC를 눌렀습니다. 메인 메뉴로 돌아갑니다.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            break;
        }

        int value;
        try {
            value = std::stoi(inputStr);
        }
        catch (...) {
            cout << "\n숫자를 입력하세요.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        if (inputStep == 0) {
            row = value;
            if (row == -1) {
                solveSudoku(grid);
                clearScreen();
                cout << "\n해결된 스도쿠 판:\n";
                printGrid(grid, fixedGrid);
                pauseScreen();
                break;
            }
            inputStep = 1;
        }
        else if (inputStep == 1) {
            col = value;
            inputStep = 2;
        }
        else {
            num = value;
            if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
                cout << "잘못된 입력 범위입니다.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                inputStep = 0;
                continue;
            }
            if (fixedGrid[row - 1][col - 1] != 0) {
                cout << "이 칸은 고정된 숫자입니다.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                inputStep = 0;
                continue;
            }
            if (!isValidMove(grid, row - 1, col - 1, num)) {
                wrongAttempts++;
                cout << "스도쿠 규칙 위반입니다.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                inputStep = 0;
                continue;
            }
            if (!isSolvableMove(grid, row - 1, col - 1, num)) {
                wrongAttempts++;
                cout << "이 숫자로는 풀이가 불가능합니다.\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                inputStep = 0;
                continue;
            }
            grid[row - 1][col - 1] = num;
            bool complete = true;
            for (int i = 0; i < 9 && complete; i++)
                for (int j = 0; j < 9; j++)
                    if (grid[i][j] == 0)
                        complete = false;

            if (complete) {
                clearScreen();
                cout << "\n축하합니다! 퍼즐을 완성했습니다.\n";
                printGrid(grid, fixedGrid);
                pauseScreen();
                break;
            }
            inputStep = 0;
        }
    }
}

void playSudoku() {
    while (true) {
        clearScreen();
        srand(static_cast<unsigned int>(time(0)));
        vector<vector<int>> grid(9, vector<int>(9, 0));
        vector<vector<int>> fixedGrid(9, vector<int>(9, 0));
        int choice, clues = 30, timeLimitMinutes = 15;

        cout << "난이도 선택:\n";
        cout << "1. 쉬움 (10분)\n2. 보통 (15분)\n3. 어려움 (20분)\n";
        cout << "선택 입력 (종료: 0): ";
        cin >> choice;

        if (!validateNumberInput() || choice < 0 || choice > 3) {
            cout << "잘못된 선택입니다. 기본 난이도(보통)으로 시작합니다.\n";
            choice = 2;
        }
        if (choice == 0) {
            cout << "게임을 종료합니다.\n";
            break;
        }

        switch (choice) {
        case 1: clues = 36; timeLimitMinutes = 10; break;
        case 2: clues = 30; timeLimitMinutes = 15; break;
        case 3: clues = 24; timeLimitMinutes = 20; break;
        }

        initializeGrid(grid, fixedGrid, clues);
        cout << "\n스도쿠 게임을 시작합니다...\n";
        playMainGame(grid, fixedGrid, timeLimitMinutes);
    }
}
