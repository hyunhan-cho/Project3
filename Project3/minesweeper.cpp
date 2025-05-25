#include "minesweeper.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

void setColor(int text, int bg = 0) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (bg << 4) | text);
}

class Minesweeper {
private:
    int rows, cols, totalMines;
    int cellsToReveal, revealedCount;
    vector<vector<char>> board;
    vector<vector<int>> mineBoard;
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;

public:
    Minesweeper(int size, int mines) : rows(size), cols(size), totalMines(mines), revealedCount(0) {
        board = vector<vector<char>>(rows, vector<char>(cols, '.'));
        mineBoard = vector<vector<int>>(rows, vector<int>(cols, 0));
        revealed = vector<vector<bool>>(rows, vector<bool>(cols, false));
        flagged = vector<vector<bool>>(rows, vector<bool>(cols, false));
        cellsToReveal = rows * cols - totalMines;
        srand(time(nullptr));
        placeMines();
        calculateNumbers();
    }

    void placeMines() {
        int placed = 0;
        while (placed < totalMines) {
            int r = rand() % rows;
            int c = rand() % cols;
            if (mineBoard[r][c] != -1) {
                mineBoard[r][c] = -1;
                placed++;
            }
        }
    }

    void calculateNumbers() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (mineBoard[i][j] == -1) continue;
                int count = 0;
                for (int dr = -1; dr <= 1; dr++) {
                    for (int dc = -1; dc <= 1; dc++) {
                        int ni = i + dr, nj = j + dc;
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && mineBoard[ni][nj] == -1)
                            count++;
                    }
                }
                mineBoard[i][j] = count;
            }
        }
    }

    void revealAll() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (mineBoard[i][j] == -1) board[i][j] = '*';
                else board[i][j] = mineBoard[i][j] + '0';
            }
        }
    }

    void printBoard() {
        cout << "   ";
        for (int j = 0; j < cols; j++) cout << setw(2) << j;
        cout << "\n  ";
        for (int j = 0; j < cols; j++) cout << "--";
        cout << "-\n";

        for (int i = 0; i < rows; i++) {
            cout << setw(2) << i << "|";
            for (int j = 0; j < cols; j++) {
                char ch = board[i][j];
                if (ch == '*') setColor(12);
                else if (ch == 'F') setColor(14);
                else if (ch != '.') setColor(11);
                else setColor(7);
                cout << " " << ch;
            }
            setColor(7);
            cout << "\n";
        }
    }

    bool reveal(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c]) return true;
        revealed[r][c] = true;
        if (mineBoard[r][c] == -1) {
            board[r][c] = '*';
            return false;
        }
        else {
            board[r][c] = mineBoard[r][c] + '0';
            revealedCount++;
            if (revealedCount == cellsToReveal) {
                revealAll();
                system("cls");
                printBoard();
                setColor(10);
                cout << "\n🎉 모든 칸을 열었습니다! 게임 클리어!\n";
                setColor(7);
                system("pause");
                exit(0);
            }
        }
        return true;
    }

    void toggleFlag(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c]) return;
        flagged[r][c] = !flagged[r][c];
        board[r][c] = flagged[r][c] ? 'F' : '.';
    }

    void play() {
        while (true) {
            system("cls");
            printBoard();
            cout << "\n명령 입력 (r 행 열: 열기, f 행 열: 깃발): ";
            char cmd;
            int r, c;
            cin >> cmd >> r >> c;

            if (cmd == 'r') {
                if (!reveal(r, c)) {
                    system("cls");
                    revealAll();
                    printBoard();
                    setColor(12);
                    cout << "\n💥 지뢰를 밟았습니다! 게임 오버!\n";
                    setColor(7);
                    break;
                }
            }
            else if (cmd == 'f') {
                toggleFlag(r, c);
            }
            else {
                cout << "\n잘못된 명령입니다. 'r' 또는 'f'를 사용하세요.\n";
                Sleep(1000);
            }
        }
    }
};

void playMinesweeper() {
    int size, mines;
    string name;

    system("cls");
    cout << "지뢰찾기 게임에 오신 것을 환영합니다!\n\n";
    cout << "플레이어 닉네임을 입력하세요: ";
    cin >> name;

    cout << "\n" << name << " 님을 위한 게임 로딩 중";
    for (int i = 0; i < 5; ++i) {
        cout << ".";
        cout.flush();
        Sleep(500);
    }

    system("cls");
    cout << "=== 지뢰찾기 게임 설정 ===\n";
    cout << "게임판 크기를 입력하세요 (예: 9): ";
    cin >> size;
    cout << "지뢰 개수를 입력하세요 (예: 10): ";
    cin >> mines;

    Minesweeper game(size, mines);
    game.play();
    system("pause");
}