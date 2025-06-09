#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <string>

#include "utils.h" 
#include "tetris.h"

using namespace std;

// --- 상수 정의 ---
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;

// --- 함수 프로토타입 ---
void setCursorVisible(bool visible);
void gotoxy(int x, int y);

// --- 클래스 및 구조체 정의 ---
struct Block {
    int shape[4][4];
    int x, y;
    int type;
};

// --- 핵심 게임 로직 ---
const int blockShapes[7][4][4] = {
    {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}}, // I
    {{0,2,2,0}, {0,2,2,0}, {0,0,0,0}, {0,0,0,0}}, // O
    {{0,0,0,0}, {3,3,3,0}, {0,3,0,0}, {0,0,0,0}}, // T
    {{0,0,0,0}, {4,4,4,0}, {4,0,0,0}, {0,0,0,0}}, // L
    {{0,0,0,0}, {5,5,5,0}, {0,0,5,0}, {0,0,0,0}}, // J
    {{0,0,0,0}, {0,6,6,0}, {6,6,0,0}, {0,0,0,0}}, // S
    {{0,0,0,0}, {7,7,0,0}, {0,7,7,0}, {0,0,0,0}}  // Z
};
const int blockColors[] = { 7, 12, 14, 13, 4, 11, 10, 9 };

Block createNewBlock() {
    Block newBlock;
    newBlock.type = rand() % 7;
    newBlock.x = BOARD_WIDTH / 2 - 2;
    newBlock.y = 0;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) newBlock.shape[i][j] = blockShapes[newBlock.type][i][j];
    return newBlock;
}

bool checkCollision(const Block& block, int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        if (block.shape[i][j] != 0) {
            int boardX = block.x + j;
            int boardY = block.y + i;
            if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) return true;
            if (boardY >= 0 && board[boardY][boardX] != 0) return true;
        }
    }
    return false;
}

void mergeBlock(const Block& block, int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        if (block.shape[i][j] != 0) {
            int boardX = block.x + j;
            int boardY = block.y + i;
            if (boardY >= 0) board[boardY][boardX] = block.shape[i][j];
        }
    }
}

void clearLines(int board[BOARD_HEIGHT][BOARD_WIDTH], int& score) {
    int linesCleared = 0;
    for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
        bool lineIsFull = true;
        for (int j = 0; j < BOARD_WIDTH; j++) if (board[i][j] == 0) { lineIsFull = false; break; }
        if (lineIsFull) {
            linesCleared++;
            for (int k = i; k > 0; k--) for (int j = 0; j < BOARD_WIDTH; j++) board[k][j] = board[k - 1][j];
            for (int j = 0; j < BOARD_WIDTH; j++) board[0][j] = 0;
            i++;
        }
    }
    score += linesCleared * linesCleared * 100;
}

void rotateBlock(Block& block, int board[BOARD_HEIGHT][BOARD_WIDTH]) {
    Block temp = block;
    int newShape[4][4] = { 0 };
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) newShape[i][j] = temp.shape[3 - j][i];
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) temp.shape[i][j] = newShape[i][j];
    if (!checkCollision(temp, board)) {
        for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) block.shape[i][j] = temp.shape[i][j];
    }
}

// ✅ 화면 전체를 ASCII 문자로만 다시 그리는 렌더링 함수
void drawScreen_ASCII(int board[BOARD_HEIGHT][BOARD_WIDTH], int score, const Block& current, const Block& next) {
    gotoxy(0, 0);

    int displayBoard[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
    for (int i = 0; i < BOARD_HEIGHT; ++i) for (int j = 0; j < BOARD_WIDTH; ++j) displayBoard[i][j] = board[i][j];
    mergeBlock(current, displayBoard);

    setColor(7);
    cout << "  ####### TETRIS #######\n";
    cout << "  SCORE: " << score << "           \n\n";

    // 상단 테두리
    cout << "  +--------------------+\n";

    for (int i = 0; i < BOARD_HEIGHT; i++) {
        cout << "  |"; // 왼쪽 테두리
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (displayBoard[i][j] != 0) {
                setColor(blockColors[displayBoard[i][j]]);
                cout << "[]"; // ✅ 블록을 "[]"로 변경
            }
            else {
                setColor(8);
                cout << ". "; // ✅ 빈 칸을 ". "으로 변경
            }
        }
        setColor(7);
        cout << "|"; // 오른쪽 테두리

        // Next 블록 그리기
        if (i == 2) cout << "   NEXT:";
        if (i >= 4 && i < 8) {
            cout << "   ";
            for (int j = 0; j < 4; ++j) {
                if (next.shape[i - 4][j]) {
                    setColor(blockColors[next.type + 1]);
                    cout << "[]";
                }
                else {
                    cout << "  ";
                }
            }
        }
        cout << "\n";
    }
    // 하단 테두리
    cout << "  +--------------------+\n";
}

void playTetris() {
    system("mode con: cols=50 lines=28");
    setCursorVisible(false);

    int board[BOARD_HEIGHT][BOARD_WIDTH] = { 0 };
    int score = 0;
    Block currentBlock = createNewBlock();
    Block nextBlock = createNewBlock();
    bool gameOver = false;
    DWORD lastDropTime = GetTickCount();

    while (!gameOver) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();
                Block temp = currentBlock;
                switch (key) {
                case 75: temp.x--; if (!checkCollision(temp, board)) currentBlock.x--; break;
                case 77: temp.x++; if (!checkCollision(temp, board)) currentBlock.x++; break;
                case 80: temp.y++; if (!checkCollision(temp, board)) { currentBlock.y++; score++; } break;
                case 72: rotateBlock(currentBlock, board); break;
                }
            }
            else if (key == ' ') {
                Block temp = currentBlock;
                while (!checkCollision(temp, board)) {
                    currentBlock = temp;
                    temp.y++;
                    score += 2;
                }
                mergeBlock(currentBlock, board);
                clearLines(board, score);
                currentBlock = nextBlock;
                nextBlock = createNewBlock();
            }
            else if (key == 27) { gameOver = true; }
        }

        if (GetTickCount() - lastDropTime > 500) {
            Block temp = currentBlock;
            temp.y++;
            if (!checkCollision(temp, board)) {
                currentBlock.y++;
            }
            else {
                mergeBlock(currentBlock, board);
                clearLines(board, score);
                currentBlock = nextBlock;
                nextBlock = createNewBlock();
                if (checkCollision(currentBlock, board)) gameOver = true;
            }
            lastDropTime = GetTickCount();
        }

        drawScreen_ASCII(board, score, currentBlock, nextBlock);
        Sleep(50);
    }

    gotoxy(10, 10);
    setColor(12);
    cout << "GAME OVER";
    gotoxy(10, 11);
    cout << "FINAL SCORE: " << score;
    gotoxy(0, 25);
    setCursorVisible(true);
    system("pause");
}

void setCursorVisible(bool visible) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = visible;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}

void gotoxy(int x, int y) {
    COORD pos = { (short)x, (short)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}