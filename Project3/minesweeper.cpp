#include "minesweeper.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>

using namespace std;

class Minesweeper {
private:
    int rows, cols, totalMines;
    vector<vector<char>> board;      // 사용자에게 보이는 보드
    vector<vector<int>> mineBoard;   // 내부 보드 (-1은 지뢰, 그 외는 주변 지뢰 개수)
    vector<vector<bool>> revealed;   // 열렸는지 여부
    vector<vector<bool>> flagged;    // 깃발 표시 여부

public:
    Minesweeper(int size, int mines) : rows(size), cols(size), totalMines(mines) {
        board = vector<vector<char>>(rows, vector<char>(cols, '.'));
        mineBoard = vector<vector<int>>(rows, vector<int>(cols, 0));
        revealed = vector<vector<bool>>(rows, vector<bool>(cols, false));
        flagged = vector<vector<bool>>(rows, vector<bool>(cols, false));
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

    void printBoard() {
        cout << "   ";
        for (int j = 0; j < cols; j++) cout << setw(2) << j;
        cout << "\n  ";
        for (int j = 0; j < cols; j++) cout << "--";
        cout << "-\n";

        for (int i = 0; i < rows; i++) {
            cout << setw(2) << i << "|";
            for (int j = 0; j < cols; j++) {
                cout << " " << board[i][j];
            }
            cout << "\n";
        }
    }

    bool reveal(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c]) return true;
        revealed[r][c] = true;
        if (mineBoard[r][c] == -1) {
            board[r][c] = '*';
            return false; // Game over
        }
        else {
            board[r][c] = mineBoard[r][c] + '0';
        }
        return true;
    }

    void toggleFlag(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c]) return;
        flagged[r][c] = !flagged[r][c];
        board[r][c] = flagged[r][c] ? 'F' : '.';
    }

    void play() {
        bool running = true;
        while (running) {
            system("cls");
            printBoard();
            cout << "\nEnter command (r row col to reveal, f row col to flag): ";
            char cmd;
            int r, c;
            cin >> cmd >> r >> c;

            if (cmd == 'r') {
                if (!reveal(r, c)) {
                    system("cls");
                    printBoard();
                    cout << "\n 지뢰를 밟았습니다! 게임 오버!\n";
                    break;
                }
            }
            else if (cmd == 'f') {
                toggleFlag(r, c);
            }
            else {
                cout << " 잘못된 명령입니다. 'r' 또는 'f'를 사용하세요.\n";
                Sleep(1000);
            }
        }
    }
};

void playMinesweeper() {
    int size, mines;
    system("cls");

    // 설명 출력
    cout << " === 지뢰찾기 게임 설명 ===\n\n";
    cout << " 목표: 모든 지뢰를 피해서 빈 칸을 모두 엽니다!\n\n";
    cout << " 조작법:\n";
    cout << " - 'r 행 열' : 해당 칸을 엽니다 (예: r 3 5)\n";
    cout << " - 'f 행 열' : 해당 칸에 깃발을 꽂거나 해제합니다 (예: f 2 4)\n\n";
    cout << " 지뢰를 밟으면 게임 오버!\n\n";
    cout << " 게임 설정 화면으로 이동 중";

    // 로딩 애니메이션 (5초)
    for (int i = 0; i < 5; ++i) {
        cout << ".";
        cout.flush();
        Sleep(1000);
    }

    system("cls");

    // 게임 설정 입력
    cout << "=== 지뢰찾기 게임 시작 ===\n";
    cout << "게임판 크기를 입력하세요 (예: 9): ";
    cin >> size;
    cout << "지뢰 개수를 입력하세요 (예: 10): ";
    cin >> mines;

    // 게임 시작
    Minesweeper game(size, mines);
    game.play();

    system("pause");
}
