#include "maze_solver.h"
#include "utils.h"

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>

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

void printMaze(const vector<vector<int>>& maze, pair<int, int> player, pair<int, int> end) {
    clearScreen();
    size_t n = maze.size(), m = maze[0].size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            if (player.first == i && player.second == j) cout << "\033[1;32mP\033[0m";
            else if (end.first == i && end.second == j) cout << "\033[1;34mE\033[0m";
            else if (maze[i][j] == 1) cout << "\033[1;31m#\033[0m";
            else cout << " ";
        }
        cout << '\n';
    }
    cout << "\nW: 위  S: 아래  A: 왼쪽  D: 오른쪽  Z: 뒤로가기  ESC: 처음으로\n";
}

bool isSolvable(const vector<vector<int>>& maze, pair<int, int> start, pair<int, int> end) {
    size_t n = maze.size(), m = maze[0].size();
    vector<vector<bool>> visited(n, vector<bool>(m, false));
    queue<pair<int, int>> q;
    q.push(start);
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        pair<int, int> pos = q.front(); q.pop();
        int r = pos.first;
        int c = pos.second;
        if (r == end.first && c == end.second) return true;
        for (int dir = 0; dir < 4; ++dir) {
            int nr = r + dr[dir], nc = c + dc[dir];
            if (nr >= 0 && nr < (int)n && nc >= 0 && nc < (int)m && maze[nr][nc] == 0 && !visited[nr][nc]) {
                visited[nr][nc] = true;
                q.push(make_pair(nr, nc));
            }
        }
    }
    return false;
}

vector<vector<int>> generateMaze(int size) {
    vector<vector<int>> maze(size, vector<int>(size, 1));
    stack<pair<int, int>> st;
    st.push(make_pair(0, 0));
    maze[0][0] = 0;

    while (!st.empty()) {
        pair<int, int> cur = st.top();
        int r = cur.first;
        int c = cur.second;

        vector<pair<int, int>> neighbors;
        for (int d = 0; d < 4; ++d) {
            int nr = r + dr[d] * 2;
            int nc = c + dc[d] * 2;
            if (nr >= 0 && nc >= 0 && nr < size && nc < size && maze[nr][nc] == 1) {
                neighbors.push_back(make_pair(nr, nc));
            }
        }
        if (!neighbors.empty()) {
            pair<int, int> next = neighbors[rand() % neighbors.size()];
            int nr = next.first;
            int nc = next.second;
            maze[(r + nr) / 2][(c + nc) / 2] = 0;
            maze[nr][nc] = 0;
            st.push(next);
        }
        else {
            st.pop();
        }
    }
    return maze;
}

void playMazeGame() {
    srand(static_cast<unsigned int>(time(0))); // <- 캐스팅으로 경고 제거
    pair<int, int> player = make_pair(0, 0);
    stack<pair<int, int>> history;

    cout << "\033[1;36m\n미로 탐험 게임에 오신 것을 환영합니다!\033[0m\n";
    cout << "당신은 미로에 갇힌 탐험가입니다. 탈출하려면 출구까지 도달하세요!\n\n";
    cout << "[조작 방법 안내]\n";
    cout << "  W: 위로 이동\n";
    cout << "  S: 아래로 이동\n";
    cout << "  A: 왼쪽으로 이동\n";
    cout << "  D: 오른쪽으로 이동\n";
    cout << "  Z: 한 칸 뒤로가기\n";
    cout << "  ESC: 처음으로 돌아가기\n\n";
    cout << "[모드 선택]\n";
    cout << "  1: 초보자 (11x11)\n";
    cout << "  2: 중급자 (15x15)\n";
    cout << "  3: 고급자 (17x17)\n\n";

    int level = 0, size = 11;
    while (true) {
        cout << "원하는 난이도를 선택하세요 (1/2/3): ";
        cin >> level;
        if (level == 1) size = 11;
        else if (level == 2) size = 15;
        else if (level == 3) size = 17;
        else continue;

        vector<vector<int>> maze;
        while (true) {
            maze = generateMaze(size);
            if (isSolvable(maze, make_pair(0, 0), make_pair(size - 1, size - 1))) break;
        }

        player = make_pair(0, 0);
        history = stack<pair<int, int>>();
        pair<int, int> end = make_pair(size - 1, size - 1);
        printMaze(maze, player, end);

        while (true) {
            char key = _getch();
            key = toupper(key);

            if (key == 27) {
                cout << "\nESC 입력됨: 메인 메뉴로 돌아갑니다...\n";
                return;
            }
            if (key == 'Z' && !history.empty()) {
                player = history.top(); history.pop();
            }
            else {
                for (int dir = 0; dir < 4; ++dir) {
                    if (key == moveKeys[dir]) {
                        int nr = player.first + dr[dir];
                        int nc = player.second + dc[dir];
                        if (nr >= 0 && nr < size && nc >= 0 && nc < size && maze[nr][nc] == 0) {
                            history.push(player);
                            player = make_pair(nr, nc);
                        }
                    }
                }
            }
            printMaze(maze, player, end);
            if (player == end) {
                cout << "\033[1;33m\n축하합니다! 출구에 도착했습니다!\033[0m\n";
                break;
            }
        }
    }
}
