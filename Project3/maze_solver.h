#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
int _getch();
#endif

// 방향 이동
extern const int dr[4];
extern const int dc[4];
extern const char moveKeys[4];

// 화면 초기화
void clearScreen();

// 미로 출력
void printMaze(const std::vector<std::vector<int>>& maze, std::pair<int, int> player, std::pair<int, int> end);

// 난이도별 미리 정의된 미로에서 선택
std::vector<std::vector<int>> selectMaze(int level);

// 미로 게임 실행
void playMazeGame();

#endif // MAZE_SOLVER_H
