#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include "tetris.h"

using namespace std;

const int WIDTH = 15;
const int HEIGHT = 20;

// 블록 타입마다 고유 숫자 (1~4)
int blocks[4][4][4] = {
    { {1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} }, // I (파랑)
    { {2,2}, {2,2} },                             // O (빨강)
    { {0,3,0}, {3,3,3}, {0,0,0} },                // T (초록)
    { {4,0,0}, {4,4,4}, {0,0,0} }                 // L (노랑)
};

// 콘솔 색상 설정
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

class Block {
public:
    int shape[4][4] = { 0 };
    int x, y;
    int size;

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

    bool isCollision(Block& b, int dx = 0, int dy = 0) {
        for (int i = 0; i < b.size; ++i)
            for (int j = 0; j < b.size; ++j)
                if (b.shape[i][j]) {
                    int nx = b.x + j + dx;
                    int ny = b.y + i + dy;
                    if (nx < 0 || nx >= WIDTH || ny >= HEIGHT)
                        return true;
                    if (ny >= 0 && grid[ny][nx])
                        return true;
                }
        return false;
    }

    void merge(Block& b) {
        for (int i = 0; i < b.size; ++i)
            for (int j = 0; j < b.size; ++j)
                if (b.shape[i][j]) {
                    int nx = b.x + j;
                    int ny = b.y + i;
                    if (ny >= 0) grid[ny][nx] = b.shape[i][j]; // 색 유지
                    else running = false;
                }
    }

    void clearLines() {
        for (int i = HEIGHT - 1; i >= 0; --i) {
            bool full = true;
            for (int j = 0; j < WIDTH; ++j)
                if (!grid[i][j]) {
                    full = false;
                    break;
                }
            if (full) {
                for (int k = i; k > 0; --k)
                    for (int j = 0; j < WIDTH; ++j)
                        grid[k][j] = grid[k - 1][j];
                for (int j = 0; j < WIDTH; ++j)
                    grid[0][j] = 0;
                i++;
            }
        }
    }

    void draw(Block& b) {
        system("cls");

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

                if (isBlock) {
                    setBlockColor(tempVal);
                    cout << "■ ";
                }
                else if (value) {
                    setBlockColor(value);
                    cout << "■ ";
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    cout << ". ";
                }
            }
            cout << endl;
        }

        // 색 초기화
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }
};

void drawExplosion(Board& board) {
    for (int t = 0; t < 6; ++t) {
        system("cls");
        for (int i = 0; i < HEIGHT; ++i) {
            for (int j = 0; j < WIDTH; ++j) {
                if ((i + j + t) % 2 == 0) {
                    setBlockColor((t % 4) + 1);
                    cout << "※ ";
                }
                else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
                    cout << "  ";
                }
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
        if (!board.isCollision(block, 0, 1)) {
            block.y++;
        }
        else {
            if (block.y == 0) {
                board.running = false;
            }
            board.merge(block);
            board.clearLines();
            if (board.running)
                block = Block();
        }
    }

    void render() {
        board.draw(block);
    }

    void gameLoop() {
        while (board.running) {
            processInput();
            update();
            render();
            Sleep(300);
        }

        drawExplosion(board);
        cout << "\n\n===== GAME OVER =====\n\n";
        system("pause");
    }
};

void playTetris() {
    system("cls");
    cout << "   테트리스 게임을 시작하시겠습니까? (Y/N): ";

    char input;
    cin >> input;

    if (input == 'Y' || input == 'y') {
        cout << "\n\n▶ 흥미진진한 블록쌓기 게임을 즐겨보세요!!\n";
        Sleep(2000); // 2초 대기
        system("cls"); // 화면 지우고 게임 시작
        srand(time(0));

        Game game;
        game.gameLoop();
    }
    else {
        cout << "\n▶ 테트리스 게임이 취소되었습니다.\n";
        system("pause");
    }
}
