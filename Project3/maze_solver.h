#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include <vector>
#include <string>
#include <utility>

// ���� ���� (�����¿�)
extern const int dr[4];
extern const int dc[4];
extern const char moveKeys[4];

// platform�� getch ���Ǵ� cpp������ ��
void clearScreen();
void printMaze(const std::vector<std::vector<int>>& maze, std::pair<int, int> player, std::pair<int, int> end);
std::vector<std::pair<int, int>> bfsPath(const std::vector<std::vector<int>>& maze, std::pair<int, int> start, std::pair<int, int> end);
std::vector<std::pair<int, int>> dfsPath(const std::vector<std::vector<int>>& maze, std::pair<int, int> start, std::pair<int, int> end);
void animatePath(std::vector<std::vector<int>> maze, const std::vector<std::pair<int, int>>& path, std::pair<int, int> end);

// �� �Լ��� �ܺο��� ȣ��Ǹ� ��
void playMaze_Solver();

#endif
