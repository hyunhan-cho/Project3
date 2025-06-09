#include "minesweeper.h"
#include "utils.h"      // setColor, clearScreen 함수를 사용하기 위해 포함
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>      // 문자열 기반 입력을 위해 포함
#include <windows.h>

using namespace std;

class Minesweeper {
private:
    int rows, cols, totalMines;
    int revealedCount;
    bool gameOver;
    bool firstClick; // [개선] 첫 클릭인지 확인하는 변수

    vector<vector<int>> board;      // -1: 지뢰, 0~8: 주변 지뢰 수
    vector<vector<bool>> revealed;
    vector<vector<bool>> flagged;

public:
    Minesweeper(int r, int c, int mines) :
        rows(r), cols(c), totalMines(mines), revealedCount(0), gameOver(false), firstClick(true) {
        board.assign(r, vector<int>(c, 0));
        revealed.assign(r, vector<bool>(c, false));
        flagged.assign(r, vector<bool>(c, false));
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    // [개선] 첫 클릭 이후에 지뢰를 배치하는 함수
    void placeMines(int firstR, int firstC) {
        int placed = 0;
        while (placed < totalMines) {
            int r = rand() % rows;
            int c = rand() % cols;
            // 지뢰가 아니면서, 첫 클릭 위치가 아닌 경우에만 배치
            if (board[r][c] != -1 && (r != firstR || c != firstC)) {
                board[r][c] = -1;
                placed++;
            }
        }
        calculateNumbers();
    }

    void calculateNumbers() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (board[r][c] == -1) continue;
                int count = 0;
                for (int dr = -1; dr <= 1; ++dr) {
                    for (int dc = -1; dc <= 1; ++dc) {
                        int nr = r + dr;
                        int nc = c + dc;
                        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && board[nr][nc] == -1) {
                            count++;
                        }
                    }
                }
                board[r][c] = count;
            }
        }
    }

    void printBoard() {
        clearScreen();
        cout << "\n    ";
        for (int c = 0; c < cols; ++c) cout << setw(3) << c;
        cout << "\n  +";
        for (int c = 0; c < cols; ++c) cout << "---";
        cout << "+\n";

        for (int r = 0; r < rows; ++r) {
            cout << setw(2) << r << "|";
            for (int c = 0; c < cols; ++c) {
                cout << " ";
                if (flagged[r][c]) {
                    setColor(14); cout << "F"; // 노란색 깃발
                }
                else if (!revealed[r][c]) {
                    setColor(8); cout << "."; // 회색 안 열린 칸
                }
                else {
                    if (board[r][c] == -1) {
                        setColor(12); cout << "*"; // 빨간색 지뢰
                    }
                    else if (board[r][c] == 0) {
                        setColor(8); cout << " "; // 빈 칸
                    }
                    else {
                        // 숫자에 따라 다른 색상
                        int colors[] = { 7, 9, 10, 13, 1, 3, 5, 4 }; // 기본,파랑,초록,보라...
                        setColor(colors[board[r][c] % 8]);
                        cout << board[r][c];
                    }
                }
                setColor(7); cout << " ";
            }
            cout << "|\n";
        }
        cout << "  +";
        for (int c = 0; c < cols; ++c) cout << "---";
        cout << "+\n";
    }

    // [개선] 빈 칸을 열었을 때 주변을 연쇄적으로 여는 재귀 함수
    void reveal(int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || revealed[r][c] || flagged[r][c]) {
            return;
        }

        revealed[r][c] = true;
        revealedCount++;

        if (board[r][c] == 0) {
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    reveal(r + dr, c + dc);
                }
            }
        }
    }

    void processMove(char cmd, int r, int c) {
        if (r < 0 || r >= rows || c < 0 || c >= cols) {
            cout << "\n잘못된 좌표입니다. 범위 내의 숫자를 입력하세요.\n";
            Sleep(1000);
            return;
        }

        if (cmd == 'f') {
            if (!revealed[r][c]) {
                flagged[r][c] = !flagged[r][c];
            }
            return;
        }

        if (cmd == 'r') {
            if (firstClick) {
                placeMines(r, c);
                firstClick = false;
            }

            if (board[r][c] == -1) {
                gameOver = true;
                // 모든 지뢰 표시
                for (int i = 0; i < rows; ++i) {
                    for (int j = 0; j < cols; ++j) {
                        if (board[i][j] == -1) revealed[i][j] = true;
                    }
                }
            }
            else {
                reveal(r, c);
            }
        }
    }

    void checkWinCondition() {
        if (!gameOver && revealedCount == (rows * cols - totalMines)) {
            gameOver = true;
            printBoard();
            setColor(10);
            cout << "\n ★★★ 모든 지뢰를 찾았습니다! 게임 클리어! ★★★\n\n";
            setColor(7);
        }
    }

    void play() {
        while (!gameOver) {
            printBoard();
            checkWinCondition();
            if (gameOver) break;

            cout << "\n명령 입력 (예: r 3 4 -> (3,4) 열기, f 1 2 -> (1,2) 깃발, exit -> 나가기)\n";
            cout << ">> ";

            string line;
            getline(cin, line);
            stringstream ss(line);

            string cmd_str;
            ss >> cmd_str;

            if (cmd_str == "exit") {
                cout << "\n메인 메뉴로 돌아갑니다...\n";
                Sleep(1000);
                return;
            }

            char cmd = cmd_str[0];
            int r, c;
            if ((cmd == 'r' || cmd == 'f') && (ss >> r >> c)) {
                processMove(cmd, r, c);
            }
            else {
                cout << "\n잘못된 명령입니다. 'r 3 4' 와 같은 형식으로 입력해주세요.\n";
                Sleep(1500);
            }
        }

        // 게임 오버 또는 승리 시
        if (board[0][0] != -2) { // gameOver가 true일 때만 실행되도록
            printBoard(); // 최종 보드 상태 한번 더 출력
            if (revealedCount != (rows * cols - totalMines)) {
                setColor(12);
                cout << "\nGAME OVER! 지뢰를 밟았습니다.\n\n";
                setColor(7);
            }
        }
    }
};

void playMinesweeper() {
    string name;
    int size, mines;

    clearScreen();
    setColor(11);
    cout << "\n   *** Minesweeper ***\n\n";
    setColor(7);
    cout << "지뢰찾기 게임에 오신 것을 환영합니다!\n\n";
    cout << "플레이어 닉네임을 입력하세요: ";
    getline(cin, name);

    cout << "\n" << name << " 님을 위한 게임 로딩 중";
    for (int i = 0; i < 3; ++i) { cout << "."; cout.flush(); Sleep(300); }

    while (true) {
        clearScreen();
        cout << "=== 지뢰찾기 게임 설정 ===\n";
        cout << "게임판 크기를 입력하세요 (예: 9 -> 9x9): ";
        cin >> size;
        cout << "지뢰 개수를 입력하세요 (예: 10): ";
        cin >> mines;

        if (cin.fail() || size <= 0 || mines <= 0 || mines >= size * size) {
            cout << "\n잘못된 입력입니다. 다시 입력해주세요.\n";
            cin.clear(); // 오류 상태 초기화
            cin.ignore(10000, '\n'); // 버퍼 비우기
            Sleep(1000);
        }
        else {
            cin.ignore(10000, '\n'); // 정상 입력 후 버퍼 비우기
            break;
        }
    }

    Minesweeper game(size, size, mines);
    game.play();

    cout << "게임을 종료합니다. 아무 키나 누르세요...\n";
    system("pause");
}