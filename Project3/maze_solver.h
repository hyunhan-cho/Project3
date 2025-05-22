#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <vector>
#include <string>
#include <utility>

// 방향 벡터 (상하좌우)
extern const int dr[4];
extern const int dc[4];
extern const char moveKeys[4];

// platform별 getch 정의는 cpp에서만 함
void clearScreen();
void printMaze(const std::vector<std::vector<int>>& maze, std::pair<int, int> player, std::pair<int, int> end);
std::vector<std::pair<int, int>> bfsPath(const std::vector<std::vector<int>>& maze, std::pair<int, int> start, std::pair<int, int> end);
std::vector<std::pair<int, int>> dfsPath(const std::vector<std::vector<int>>& maze, std::pair<int, int> start, std::pair<int, int> end);
void animatePath(std::vector<std::vector<int>> maze, const std::vector<std::pair<int, int>>& path, std::pair<int, int> end);

// 이 함수만 외부에서 호출되면 됨
void playMaze_Solver();

#endif
