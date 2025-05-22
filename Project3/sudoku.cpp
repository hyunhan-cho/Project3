#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "sudoku.h"

using namespace std;

// 함수 원형 선언
void printGrid(const vector<vector<int>>& grid);
bool isValidMove(const vector<vector<int>>& grid, int row, int col, int num);
bool solveSudoku(vector<vector<int>>& grid);
void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid);
void initializeGrid(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid);
bool validateNumberInput();

// 스도쿠 게임 시작 함수
void playSudoku()
{
    system("cls");
    srand(time(0));

    vector<vector<int>> grid(9, vector<int>(9, 0));
    vector<vector<int>> fixedGrid(9, vector<int>(9, 0));

    initializeGrid(grid, fixedGrid);

    cout << "Welcome to the Sudoku game!\n";
    playMainGame(grid, fixedGrid);
}

// 스도쿠 보드를 출력하는 함수
void printGrid(const vector<vector<int>>& grid)
{
    cout << "  +===================================+\n";
    for (int i = 0; i < 9; i++)
    {
        cout << "  | ";
        for (int j = 0; j < 9; j++)
        {
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

// 숫자를 놓을 수 있는지 검사
bool isValidMove(const vector<vector<int>>& grid, int row, int col, int num)
{
    int startRow = 3 * (row / 3);
    int startCol = 3 * (col / 3);

    for (int x = 0; x < 9; x++)
    {
        if (grid[row][x] == num || grid[x][col] == num ||
            grid[startRow + x / 3][startCol + x % 3] == num)
        {
            return false;
        }
    }
    return true;
}

// 백트래킹으로 스도쿠 자동 풀이
bool solveSudoku(vector<vector<int>>& grid)
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                for (int num = 1; num <= 9; num++)
                {
                    if (isValidMove(grid, row, col, num))
                    {
                        grid[row][col] = num;

                        if (solveSudoku(grid))
                            return true;

                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// 랜덤 초기 숫자 배치
void initializeGrid(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (rand() % 4 == 0)
            {
                int num = rand() % 9 + 1;
                if (isValidMove(grid, i, j, num))
                {
                    grid[i][j] = num;
                    fixedGrid[i][j] = num;
                }
            }
        }
    }
}

// 유효한 숫자인지 검사
bool validateNumberInput()
{
    if (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// 메인 게임 루프
void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid)
{
    int row, col, num;

    while (true)
    {
        system("cls");

        cout << "Game Rules:\n";
        cout << "1. Enter a number between 1-9 into an empty cell.\n";
        cout << "2. You cannot change a fixed number.\n";
        cout << "3. Numbers must be unique in row, column, and 3x3 box.\n";
        cout << "4. Enter -1 for the row to auto-solve the puzzle.\n";

        cout << "\nCurrent Sudoku Grid:\n";
        printGrid(grid);

        cout << "\nEnter row (1-9) or -1 to auto-solve: ";
        cin >> row;
        if (!validateNumberInput())
        {
            cout << "Invalid input. Please enter a number.\n";
            system("pause");
            continue;
        }

        if (row == -1)
        {
            if (solveSudoku(grid))
            {
                cout << "\nSolved Sudoku Grid:\n";
                printGrid(grid);
            }
            else
            {
                cout << "\nNo solution exists for this grid.\n";
            }
            break;
        }

        cout << "Enter column (1-9): ";
        cin >> col;
        if (!validateNumberInput())
        {
            cout << "Invalid input. Please enter a number.\n";
            system("pause");
            continue;
        }

        if (row < 1 || row > 9 || col < 1 || col > 9)
        {
            cout << "Invalid input. Make sure all values are between 1 and 9.\n";
            system("pause");
            continue;
        }

        if (fixedGrid[row - 1][col - 1] != 0)
        {
            cout << "This cell is fixed and cannot be changed.\n";
            system("pause");
            continue;
        }

        cout << "Enter number (1-9): ";
        cin >> num;
        if (!validateNumberInput() || num < 1 || num > 9)
        {
            cout << "Invalid input. Please enter a number between 1 and 9.\n";
            system("pause");
            continue;
        }

        if (!isValidMove(grid, row - 1, col - 1, num))
        {
            cout << "Invalid move. This violates Sudoku rules.\n";
            system("pause");
            continue;
        }

        grid[row - 1][col - 1] = num;
    }
}
