#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "tetris.h"

using namespace std;

const int WIDTH = 15;
const int HEIGHT = 20;

int blocks[4][4][4] = {
    { {1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },
    { {2,2}, {2,2} },
    { {0,3,0}, {3,3,3}, {0,0,0} },
    { {4,0,0}, {4,4,4}, {0,0,0} }
};

void setBlockColor(int blockType) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (blockType) {
    case 1: SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY); break;
    case 2: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
    case 3: SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
    case 4: SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); break;
    default: SetConsoleTextAttribute(hConsole, 7); break;
    }
}

void fixConsoleEnvironment() {
    system("mode con: cols=100 lines=40");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    cfi.dwFontSize.X = 10;
    cfi.dwFontSize.Y = 20;
    wcscpy_s(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}

class Block {
public:
    int shape[4][4] = { 0 };
    int x, y, size;
    Block() {
        int type = rand() % 4;
        size = (type == 0) ? 4 : (type == 1) ? 2 : 3;
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                shape[i][j] = blocks[type][i][j];
        x = WIDTH / 2 - size / 2;
        y = 0;
    }
    void rotate() {
        int temp[4][4] = { 0 };
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                temp[i][j] = shape[size - j - 1][i];
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j)
                shape[i][j] = temp[i][j];
    }
};

class Board {
public:
    int grid[HEIGHT][WIDTH] = { 0 };
    bool running = true;
    int score = 0;

    bool isCollision(Block& b, int dx = 0, int dy = 0) {
        for (int i = 0; i < b.size; ++i)
            for (int j = 0; j < b.size; ++j)
                if (b.shape[i][j]) {
                    int nx = b.x + j + dx;
                    int ny = b.y + i + dy;
                    if (nx < 0 || nx >= WIDTH || ny >= HEIGHT) return true;
                    if (ny >= 0 && grid[ny][nx]) return true;
                }
        return false;
    }

    void merge(Block& b) {
        for (int i = 0; i < b.size; ++i)
            for (int j = 0; j < b.size; ++j)
                if (b.shape[i][j]) {
                    int nx = b.x + j;
                    int ny = b.y + i;
                    if (ny >= 0) grid[ny][nx] = b.shape[i][j];
                    else running = false;
                }
    }

    void clearLines() {
        for (int i = HEIGHT - 1; i >= 0; --i) {
            bool full = true;
            for (int j = 0; j < WIDTH; ++j)
                if (!grid[i][j]) { full = false; break; }
            if (full) {
                for (int k = i; k > 0; --k)
                    for (int j = 0; j < WIDTH; ++j)
                        grid[k][j] = grid[k - 1][j];
                for (int j = 0; j < WIDTH; ++j) grid[0][j] = 0;
                score += 100;
                i++;
            }
        }
    }

    void draw(Block& b) {
        system("cls");
        cout << "====== TETRIS ======\t SCORE: " << score << "\n\n";
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                int value = grid[i][j];
                bool isBlock = false;
                int tempVal = 0;
                for (int bi = 0; bi < b.size; ++bi)
                    for (int bj = 0; bj < b.size; ++bj)
                        if (b.shape[bi][bj]) {
                            int bx = b.x + bj;
                            int by = b.y + bi;
                            if (i == by && j == bx) {
                                isBlock = true;
                                tempVal = b.shape[bi][bj];
                            }
                        }
                if (isBlock) { setBlockColor(tempVal); cout << "■ "; }
                else if (value) { setBlockColor(value); cout << "■ "; }
                else { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); cout << ". "; }
            }
            cout << endl;
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
};

void drawExplosion(Board& board) {
    for (int t = 0; t < 6; ++t) {
        system("cls");
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if ((i + j + t) % 2 == 0) { setBlockColor((t % 4) + 1); cout << "※ "; }
                else { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0); cout << "  "; }
            }
            cout << endl;
        }
        Beep(300 + t * 100, 100);
        Sleep(150);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

class Game {
public:
    Board board;
    Block block;

    void processInput() {
        if (_kbhit()) {
            char first = _getch();
            if (first == -32) {
                char arrow = _getch();
                switch (arrow) {
                case 75: if (!board.isCollision(block, -1, 0)) block.x--; break;
                case 77: if (!board.isCollision(block, 1, 0)) block.x++; break;
                case 80: if (!board.isCollision(block, 0, 1)) block.y++; break;
                case 72:
                    block.rotate();
                    if (board.isCollision(block))
                        block.rotate(), block.rotate(), block.rotate();
                    break;
                }
            }
        }
    }

    void update() {
        if (!board.isCollision(block, 0, 1)) block.y++;
        else {
            if (block.y == 0) board.running = false;
            board.merge(block);
            board.clearLines();
            if (board.running) block = Block();
        }
    }

    void render() {
        board.draw(block);
    }

    void gameLoop() {
        fixConsoleEnvironment();
        while (board.running) {
            processInput(); update(); render(); Sleep(300);
        }
        drawExplosion(board);
        cout << "\n\n===== GAME OVER =====\n최종 점수: " << board.score << "점\n\n";
        system("pause");
    }
};

void playTetris() {
    fixConsoleEnvironment();
    system("cls");
    cout << R"(
┌────────────────────────────────────┐
│          🎮 TETRIS GAME           │
├────────────────────────────────────┤
│ ◼ 방향키 ← → ↓ : 블록 이동       │
│ ◼ 방향키 ↑       : 블록 회전       │
│ ◼ 한 줄 제거 시 +100점            │
│ ◼ 블록이 쌓이면 게임 종료         │
└────────────────────────────────────┘
)" << endl;
    cout << "\n▶ 게임을 시작하시겠습니까? (Y/N): ";
    char input;
    cin >> input;
    if (input == 'Y' || input == 'y') {
        cout << "\n▶ 게임을 시작합니다...\n";
        Sleep(2000);
        srand(time(0));
        Game game;
        game.gameLoop();
    }
    else {
        cout << "\n▶ 테트리스 게임이 취소되었습니다.\n";
        system("pause");
    }
}