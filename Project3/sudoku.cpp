#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <chrono>
#include <thread>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

void printGrid(const vector<vector<int>>& grid);
bool isValidMove(const vector<vector<int>>& grid, int row, int col, int num);
bool solveSudoku(vector<vector<int>>& grid);
void initializeGrid(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid, int clues);
void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid, int timeLimitMinutes);
bool validateNumberInput();
void playSudoku();
bool isSolvableMove(vector<vector<int>> grid, int row, int col, int num);

int hain() {
    playSudoku();
    return 0;
}

void playSudoku() {
    system("cls");
    srand(static_cast<unsigned int>(time(0)));

    vector<vector<int>> grid(9, vector<int>(9, 0));
    vector<vector<int>> fixedGrid(9, vector<int>(9, 0));

    int choice;
    int clues = 30;  // default medium
    int timeLimitMinutes = 15;

    cout << "Choose Difficulty:\n";
    cout << "1. Easy (10 min)\n2. Medium (15 min)\n3. Hard (20 min)\n";
    cout << "Enter choice: ";
    cin >> choice;

    if (!validateNumberInput() || choice < 1 || choice > 3) {
        cout << "Invalid choice. Defaulting to Medium.\n";
        choice = 2;
    }

    switch (choice) {
    case 1:
        clues = 36;
        timeLimitMinutes = 10;
        break;
    case 2:
        clues = 30;
        timeLimitMinutes = 15;
        break;
    case 3:
        clues = 24;
        timeLimitMinutes = 20;
        break;
    }

    initializeGrid(grid, fixedGrid, clues);

    cout << "\nStarting Sudoku game...\n";
    playMainGame(grid, fixedGrid, timeLimitMinutes);
}

void printGrid(const vector<vector<int>>& grid) {
    cout << "  +===================================+\n";
    for (int i = 0; i < 9; i++) {
        cout << "  | ";
        for (int j = 0; j < 9; j++) {
            cout << (grid[i][j] == 0 ? ' ' : char(grid[i][j] + '0')) << " | ";
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
    auto startTime = steady_clock::now();
    int row, col, num;

    while (true) {
        system("cls");

        auto now = steady_clock::now();
        auto elapsed = duration_cast<seconds>(now - startTime).count();
        int totalRemainingSeconds = timeLimitMinutes * 60 - static_cast<int>(elapsed);
        int minutes = totalRemainingSeconds / 60;
        int seconds = totalRemainingSeconds % 60;

        if (totalRemainingSeconds <= 0) {
            cout << "\nTime's up! You failed to solve the puzzle in time.\n";
            printGrid(grid);
            cout << "\nPress Enter to exit...";
            cin.ignore();
            cin.get();
            break;
        }

        cout << " Remaining time: " << minutes << "m " << seconds << "s\n";

        cout << "\nCurrent Sudoku Grid:\n";
        printGrid(grid);

        cout << "\nEnter row (1-9) or -1 to auto-solve: ";
        cin >> row;
        if (!validateNumberInput()) continue;

        if (row == -1) {
            if (solveSudoku(grid)) {
                cout << "\nSolved Sudoku Grid:\n";
                printGrid(grid);
            }
            else {
                cout << "\nNo solution exists.\n";
            }
            cout << "\nPress Enter to exit...";
            cin.ignore();
            cin.get();
            break;
        }

        cout << "Enter column (1-9): ";
        cin >> col;
        if (!validateNumberInput()) continue;

        cout << "Enter number (1-9): ";
        cin >> num;
        if (!validateNumberInput()) continue;

        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            cout << "Invalid input range.\n";
            system("pause");
            continue;
        }

        if (fixedGrid[row - 1][col - 1] != 0) {
            cout << "This cell is fixed.\n";
            system("pause");
            continue;
        }

        if (!isValidMove(grid, row - 1, col - 1, num)) {
            cout << "This number violates Sudoku rules.\n";
            system("pause");
            continue;
        }

        if (!isSolvableMove(grid, row - 1, col - 1, num)) {
            cout << "This move leads to no solution. Try another number.\n";
            system("pause");
            continue;
        }

        grid[row - 1][col - 1] = num;

        bool complete = true;
        for (int i = 0; i < 9 && complete; i++)
            for (int j = 0; j < 9; j++)
                if (grid[i][j] == 0)
                    complete = false;

        if (complete) {
            cout << "\n Congratulations! You completed the puzzle.\n";
            printGrid(grid);
            cout << "\nPress Enter to exit...";
            cin.ignore();
            cin.get();
            break;
        }
    }
}
