#include "snake.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 20;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  // �ܼ� ��� �ڵ�

struct Point {
    int x, y;
};

class SnakeGame {
public:
    Point snake[200];
    int length;
    Point food;
    char dir;
    bool gameOver;
    int score;
    int level;
    int speed;
    vector<Point> obstacles;

    SnakeGame() {
        length = 1;
        snake[0] = { WIDTH / 2, HEIGHT / 2 };
        dir = 'd';
        gameOver = false;
        score = 0;
        level = 1;
        speed = 100;
        placeFood();
    }

    void placeFood() {
        while (true) {
            food.x = rand() % (WIDTH - 4) + 2;
            food.y = rand() % (HEIGHT - 4) + 2;

            bool onSnake = false;
            for (int i = 0; i < length; i++) {
                if (snake[i].x == food.x && snake[i].y == food.y) {
                    onSnake = true;
                    break;
                }
            }

            bool onObstacle = false;
            for (auto& o : obstacles) {
                if (o.x == food.x && o.y == food.y) {
                    onObstacle = true;
                    break;
                }
            }

            if (!onSnake && !onObstacle) break;
        }
    }

    void generateObstacles() {
        obstacles.clear();
        int num = 10 + (level - 1) * 5;
        for (int i = 0; i < num; i++) {
            Point obs;
            do {
                obs.x = rand() % (WIDTH - 4) + 2;
                obs.y = rand() % (HEIGHT - 4) + 2;
            } while (isOnSnake(obs) || (obs.x == food.x && obs.y == food.y));
            obstacles.push_back(obs);
        }
    }

    bool isOnSnake(Point p) {
        for (int i = 0; i < length; i++) {
            if (snake[i].x == p.x && snake[i].y == p.y)
                return true;
        }
        return false;
    }

    void draw() {
        COORD cursorPos = { 0, 0 };
        SetConsoleCursorPosition(hOut, cursorPos);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    SetConsoleTextAttribute(hOut, 14);  // ��: �����
                    cout << "#";
                }
                else if (x == food.x && y == food.y) {
                    SetConsoleTextAttribute(hOut, 12); // ����: ������
                    cout << "*";
                }
                else {
                    bool printed = false;
                    for (int i = 0; i < length; i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            SetConsoleTextAttribute(hOut, 10); // ��: ���
                            cout << "@";
                            printed = true;
                            break;
                        }
                    }

                    if (!printed) {
                        for (auto& o : obstacles) {
                            if (o.x == x && o.y == y) {
                                SetConsoleTextAttribute(hOut, 9); // ��ֹ�: �Ķ�
                                cout << "X";
                                printed = true;
                                break;
                            }
                        }
                    }

                    if (!printed) cout << " ";
                }
            }
            cout << "\n";
        }
        SetConsoleTextAttribute(hOut, 7);
        cout << "Score : " << score << "   Level : " << level << "\n";
        cout << "Esc Ű�� ������ ���� �޴��� ���ư��ϴ�\n";
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {
                gameOver = true;
                return;
            }
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                case 72: if (dir != 's') dir = 'w'; break;
                case 80: if (dir != 'w') dir = 's'; break;
                case 75: if (dir != 'd') dir = 'a'; break;
                case 77: if (dir != 'a') dir = 'd'; break;
                }
            }
        }
    }

    void move() {
        Point next = snake[0];
        switch (dir) {
        case 'w': next.y--; break;
        case 's': next.y++; break;
        case 'a': next.x--; break;
        case 'd': next.x++; break;
        }

        if (next.x <= 0 || next.x >= WIDTH - 1 || next.y <= 0 || next.y >= HEIGHT - 1) {
            gameOver = true;
            return;
        }

        for (int i = 0; i < length; i++) {
            if (snake[i].x == next.x && snake[i].y == next.y) {
                gameOver = true;
                return;
            }
        }

        for (auto& o : obstacles) {
            if (o.x == next.x && o.y == next.y) {
                gameOver = true;
                return;
            }
        }

        for (int i = length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = next;

        if (next.x == food.x && next.y == food.y) {
            length++;
            score++;
            placeFood();

            if (score >= 3 && score % 3 == 0) {
                level++;
                speed = max(10, speed - 30);
                generateObstacles();
            }
        }
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }

    void run() {
        system("cls");
        hideCursor();

        SetConsoleTextAttribute(hOut, 11);
        cout << "\n";
        cout << " ============ ������ũ ���� ���۹� ============\n";
        cout << "\n";
        cout << " - ��(@)�� �ʱ� �̵������� �������Դϴ�\n";
        cout << " - ����Ű(�����)�� ���� ������ �ٲپ� �̵��ϼ���\n";
        cout << " - ����(*)�� ������ ���� ������ϴ�\n";
        cout << " - ���̳� ���뿡 �ε����� ������ ����˴ϴ�\n";
        cout << " - 3�� ����� �������� ���� �ӵ��� �������� ��ֹ��� �����մϴ�\n";
        cout << " - ���� ���� ���� ������ ���� �����Դϴ�\n";
        cout << " - Esc Ű�� ������ ���� �޴��� ���ư��ϴ�\n";
        cout << "\n";
        SetConsoleTextAttribute(hOut, 7);
        cout << " Enter Ű�� ���� ������ �����ϼ���...\n";

        while (true) {
            if (_kbhit()) {
                char c = _getch();
                if (c == 13) break;
            }
            Sleep(100);
        }

        DWORD lastMoveTime = GetTickCount();

        while (!gameOver) {
            draw();
            input();

            DWORD currentTime = GetTickCount();
            if (currentTime - lastMoveTime >= (DWORD)speed) {
                move();
                lastMoveTime = currentTime;
            }

            Sleep(10);
        }

        cout << "Game Over! Your score : " << score << "\n";
        system("pause");
    }
};

void playSnake() {
    srand((unsigned)time(NULL));
    SnakeGame game;
    game.run();
}
