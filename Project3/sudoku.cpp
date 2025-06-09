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
#include "utils.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#endif

using namespace std;

// 함수 프로토타입 추가
bool solveSudoku(vector<vector<int>>& grid);
void generateSudoku(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid, int clues);
void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid, int timeLimitMinutes);


// 비차단 키 입력 함수 (Windows)
#ifdef _WIN32
int kbhit_nonblock() {
    return _kbhit();
}
#else
// Unix 계열 시스템용 비차단 키 입력 함수
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

// 그리드 출력 함수
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

// 유효한 이동인지 확인하는 함수
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
// 스도쿠 퍼즐 생성 함수
void generateSudoku(vector<vector<int>>& grid, vector<vector<int>>& fixedGrid, int clues) {
    // 초기화
    grid = vector<vector<int>>(9, vector<int>(9, 0));
    fixedGrid = vector<vector<int>>(9, vector<int>(9, 0));

    // 난수 생성기 초기화
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 9);

    // 대각선 3x3 박스에 숫자 채우기
    for (int box = 0; box < 3; box++) {
        int startRow = box * 3;
        int startCol = box * 3;
        vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        shuffle(nums.begin(), nums.end(), gen);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                grid[startRow + i][startCol + j] = nums[i * 3 + j];
            }
        }
    }

    // 스도쿠 퍼즐 해결
    solveSudoku(grid);

    // 힌트 숫자 랜덤 제거
    vector<pair<int, int>> allCells;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            allCells.push_back({ i, j });
        }
    }
    shuffle(allCells.begin(), allCells.end(), gen);

    // 고정된 힌트 숫자 설정
    for (int i = 0; i < clues; i++) {
        int row = allCells[i].first;
        int col = allCells[i].second;
        fixedGrid[row][col] = grid[row][col];
    }

    // 나머지 숫자 제거
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fixedGrid[i][j] == 0) {
                grid[i][j] = 0;
            }
        }
    }
}

// 스도쿠 퍼즐 해결 함수
bool solveSudoku(vector<vector<int>>& grid) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValidMove(grid, row, col, num)) {
                        grid[row][col] = num;
                        if (solveSudoku(grid)) {
                            return true;
                        }
                        grid[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}
#include <thread>
#include <atomic>
#include <mutex>

void playMainGame(vector<vector<int>>& grid, const vector<vector<int>>& fixedGrid, int timeLimitMinutes) {
    std::atomic<bool> gameRunning(true);
    std::atomic<int> remainingSeconds(timeLimitMinutes * 60);

    std::thread timerThread([&]() {
        while (gameRunning && remainingSeconds > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            remainingSeconds--;
        }
        });

    int wrongAttempts = 0;
    int inputStage = 0;
    int row = -1, col = -1;

    while (gameRunning) {
        if (remainingSeconds <= 0) {
            gameRunning = false;
            clearScreen();
            cout << "\n시간 종료! 퍼즐을 제시간에 풀지 못했습니다.\n";
            printGrid(grid, fixedGrid);
            pauseScreen();
            break;
        }

        clearScreen();
        cout << "남은 시간: " << remainingSeconds / 60 << "분 "
            << remainingSeconds % 60 << "초\n";
        cout << "오답 횟수: " << wrongAttempts << " / 3\n";
        printGrid(grid, fixedGrid);

        // 입력 안내 메시지 (자동 풀이 옵션 추가)
        switch (inputStage) {
        case 0:
            cout << "\n숫자(1-9)를 입력하여 행을 선택하세요.\n";
            cout << "-1: 자동 풀이\n";
            break;
        case 1:
            cout << "\n선택된 행: " << row + 1 << "\n";
            cout << "열을 입력하세요 (1-9): \n";
            break;
        case 2:
            cout << "\n선택된 행: " << row + 1
                << ", 선택된 열: " << col + 1 << "\n";
            cout << "입력할 숫자를 선택하세요 (1-9): \n";
            break;
        }
        cout << "ESC: 메인 메뉴로 돌아가기\n";

        if (_kbhit()) {
            int ch = _getch();

            if (ch == 27) {
                gameRunning = false;
                break;
            }

            // 자동 풀이 추가
            if (inputStage == 0 && ch == '-') {
                ch = _getch();
                if (ch == '1') {
                    // 자동 풀이 로직
                    if (solveSudoku(grid)) {
                        gameRunning = false;
                        clearScreen();
                        cout << "\n자동으로 퍼즐을 해결했습니다!\n";
                        printGrid(grid, fixedGrid);
                        pauseScreen();
                    }
                    else {
                        cout << "퍼즐을 해결할 수 없습니다.\n";
                        pauseScreen();
                    }
                    break;
                }
            }

            // 기존 숫자 입력 로직 (이전 코드와 동일)
            if (ch >= '1' && ch <= '9') {
                switch (inputStage) {
                case 0:
                    row = ch - '0' - 1;
                    inputStage = 1;
                    break;

                case 1:
                    col = ch - '0' - 1;
                    inputStage = 2;
                    break;

                case 2:
                    int num = ch - '0';

                    // 기존 유효성 검사 및 입력 로직 (이전 코드와 동일)
                    if (fixedGrid[row][col] != 0) {
                        cout << "고정된 숫자는 변경할 수 없습니다!\n";
                        pauseScreen();
                        inputStage = 0;
                        continue;
                    }

                    if (isValidMove(grid, row, col, num)) {
                        grid[row][col] = num;

                        // 퍼즐 완성 확인 로직
                        bool isPuzzleComplete = true;
                        for (int i = 0; i < 9; i++) {
                            for (int j = 0; j < 9; j++) {
                                if (grid[i][j] == 0) {
                                    isPuzzleComplete = false;
                                    break;
                                }
                            }
                            if (!isPuzzleComplete) break;
                        }

                        if (isPuzzleComplete) {
                            gameRunning = false;
                            clearScreen();
                            cout << "\n축하합니다! 퍼즐을 완성했습니다!\n";
                            printGrid(grid, fixedGrid);
                            pauseScreen();
                            break;
                        }
                    }
                    else {
                        wrongAttempts++;
                        cout << "잘못된 입력입니다. 다시 시도하세요.\n";
                        pauseScreen();
                    }

                    // 오답 횟수 초과
                    if (wrongAttempts >= 3) {
                        gameRunning = false;
                        cout << "오답 3회 초과! 게임 오버.\n";
                        pauseScreen();
                        break;
                    }

                    inputStage = 0;
                    break;
                }
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    // 스레드 정리
    if (timerThread.joinable()) {
        timerThread.join();
    }
}


// 난이도 선택 함수
int chooseDifficulty() {
    while (true) {
        clearScreen();
        cout << "=== 스도쿠 난이도 선택 ===\n";
        cout << "1. 쉬움 (40개 힌트)\n";
        cout << "2. 보통 (30개 힌트)\n";
        cout << "3. 어려움 (20개 힌트)\n";
        cout << "4. 뒤로 가기\n";
        cout << "난이도를 선택하세요: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: return 40;
        case 2: return 30;
        case 3: return 20;
        case 4: return -1;
        default:
            cout << "잘못된 선택입니다. 다시 선택해주세요.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

// 시간 선택 함수
int chooseTimeLimit() {
    while (true) {
        clearScreen();
        cout << "=== 제한 시간 선택 ===\n";
        cout << "1. 10분\n";
        cout << "2. 20분\n";
        cout << "3. 30분\n";
        cout << "4. 뒤로 가기\n";
        cout << "제한 시간을 선택하세요: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: return 10;
        case 2: return 20;
        case 3: return 30;
        case 4: return -1;
        default:
            cout << "잘못된 선택입니다. 다시 선택해주세요.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

// 메인 메뉴 함수
// 함수 프로토타입
void playSudoku();

// 함수 구현
void playSudoku() {
    while (true) {
        clearScreen();
        cout << "=== 스도쿠 게임 ===\n";
        cout << "1. 새 게임 시작\n";
        cout << "2. 게임 종료\n";
        cout << "메뉴를 선택하세요: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            int clues = chooseDifficulty();
            if (clues == -1) break;

            int timeLimit = chooseTimeLimit();
            if (timeLimit == -1) break;

            vector<vector<int>> grid(9, vector<int>(9, 0));
            vector<vector<int>> fixedGrid(9, vector<int>(9, 0));

            generateSudoku(grid, fixedGrid, clues);
            playMainGame(grid, fixedGrid, timeLimit);
            break;
        }
        case 2:
            cout << "게임을 종료합니다.\n";
            return;
        default:
            cout << "잘못된 선택입니다. 다시 선택해주세요.\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
}

// main 함수 수정
int Sudokuhain() {
    // 랜덤 시드 설정
    srand(static_cast<unsigned>(time(nullptr)));

    // 게임 시작
    playSudoku();

    return 0;
}


// 메인 함수
int hain() {
    // 랜덤 시드 설정
    srand(static_cast<unsigned>(time(nullptr)));

    // 메인 메뉴 실행
    playSudoku();

    return 0;
}
