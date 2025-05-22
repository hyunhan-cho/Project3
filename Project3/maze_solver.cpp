#include "maze_solver.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <queue>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
int _getch() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

using namespace std;

const int dr[4] = { -1, 1, 0, 0 };
const int dc[4] = { 0, 0, -1, 1 };
const char moveKeys[4] = { 'W', 'S', 'A', 'D' };

void clearScreen() {
#ifdef _WIN32
    system("CLS");
#else
    cout << "\033[2J\033[H";
#endif
}

void printMaze(const vector<vector<int>>& maze, pair<int, int> player, pair<int, int> end) {
    clearScreen();
    int n = maze.size(), m = maze[0].size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (player.first == i && player.second == j) cout << 'P';
            else if (end.first == i && end.second == j) cout << 'E';
            else if (maze[i][j] == 1) cout << '#';
            else cout << '.';
        }
        cout << '\n';
    }
    cout << "Controls: W=Up  S=Down  A=Left  D=Right\n";
}

vector<pair<int, int>> bfsPath(const vector<vector<int>>& maze, pair<int, int> start, pair<int, int> end) {
    int n = maze.size(), m = maze[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    vector<vector<pair<int, int>>> parent(n, vector<pair<int, int>>(m, { -1, -1 }));
    queue<pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        auto cur = q.front(); q.pop();
        if (cur == end) break;
        for (int i = 0; i < 4; ++i) {
            int nr = cur.first + dr[i], nc = cur.second + dc[i];
            if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
            if (maze[nr][nc] == 1 || visited[nr][nc]) continue;
            visited[nr][nc] = true;
            parent[nr][nc] = cur;
            q.push({ nr, nc });
        }
    }

    vector<pair<int, int>> path;
    if (!visited[end.first][end.second]) return path;
    for (pair<int, int> at = end; at != make_pair(-1, -1); at = parent[at.first][at.second])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}

bool dfsUtil(const vector<vector<int>>& maze, pair<int, int> cur, pair<int, int> end, vector<vector<bool>>& visited, vector<pair<int, int>>& path) {
    if (cur == end) {
        path.push_back(cur);
        return true;
    }
    visited[cur.first][cur.second] = true;
    path.push_back(cur);
    for (int i = 0; i < 4; ++i) {
        int nr = cur.first + dr[i], nc = cur.second + dc[i];
        if (nr < 0 || nr >= maze.size() || nc < 0 || nc >= maze[0].size()) continue;
        if (maze[nr][nc] == 1 || visited[nr][nc]) continue;
        if (dfsUtil(maze, { nr, nc }, end, visited, path)) return true;
    }
    path.pop_back();
    return false;
}

vector<pair<int, int>> dfsPath(const vector<vector<int>>& maze, pair<int, int> start, pair<int, int> end) {
    vector<pair<int, int>> path;
    vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
    dfsUtil(maze, start, end, visited, path);
    return path;
}

void animatePath(vector<vector<int>> maze, const vector<pair<int, int>>& path, pair<int, int> end) {
    for (auto& step : path) {
        printMaze(maze, step, end);
        this_thread::sleep_for(chrono::milliseconds(300));
    }
    cout << "���� �Ϸ�!\n";
}

void playMaze_Solver() {
    vector<vector<int>> maze = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0},
        {0, 0, 0, 1, 0},
    };
    pair<int, int> player = { 0, 0 };
    pair<int, int> end = { 4, 4 };

    cout << "�̷� ���ӿ� ���� ���� ȯ���մϴ�!\n";
    cout << "�̵� Ű: W=��  S=�Ʒ�  A=����  D=������\n";
    cout << "��ǥ: P���� E���� �����ϼ���!\n\n";

    cout << "[�ڵ� Ž�� ��� ����]\n";
    cout << "B: BFS (�ִ� ��� Ž��)\n";
    cout << "D: DFS (���� �켱 Ž��)\n";
    cout << "N: ���� �̵� (WASD ���� ����)\n\n";
    cout << "�����Ϸ��� �ƹ� Ű�� ��������...";
    _getch();

    cout << "\n��带 �����ϼ��� (B/D/N �Է�): ";
    char mode;
    cin >> mode;
    cin.ignore();
    mode = toupper(mode);

    vector<pair<int, int>> path;

    if (mode == 'B') path = bfsPath(maze, player, end);
    else if (mode == 'D') path = dfsPath(maze, player, end);

    if (mode == 'B' || mode == 'D') {
        if (path.empty()) cout << "��θ� ã�� �� �����ϴ�!\n";
        else animatePath(maze, path, end);
        system("pause");
        return;
    }

    printMaze(maze, player, end);
    this_thread::sleep_for(chrono::milliseconds(100));

    while (true) {
        char key = toupper(_getch());
        for (int dir = 0; dir < 4; ++dir) {
            if (key == moveKeys[dir]) {
                int nr = player.first + dr[dir];
                int nc = player.second + dc[dir];
                if (nr >= 0 && nr < maze.size() && nc >= 0 && nc < maze[0].size() && maze[nr][nc] == 0) {
                    player = { nr, nc };
                }
            }
        }
        printMaze(maze, player, end);
        if (player == end) {
            cout << "�����մϴ�! �������� �����߽��ϴ�!\n";
            break;
        }
    }
    system("pause");
}
