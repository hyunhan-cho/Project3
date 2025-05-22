#include "snake.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <cstdlib>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 20;

struct Point {
    int x, y;
};

class SnakeGame {
public:
    Point snake[100];
    int length;
    Point food;
    char dir;
    bool gameOver;

    SnakeGame() {
        length = 1;
        snake[0] = { WIDTH / 2, HEIGHT / 2 };
        placeFood();
        dir = 'd';
        gameOver = false;
    }

    void placeFood() {
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
    }

    void draw() {
        system("cls");
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    cout << "#";
                }
                else if (x == food.x && y == food.y) {
                    cout << "*";
                }
                else {
                    bool printed = false;
                    for (int i = 0; i < length; i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            cout << "O";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }
            }
            cout << "\n";
        }
        cout << "Score: " << length - 1 << "\n";
    }

    void input() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'w' && dir != 's') dir = 'w';
            else if (key == 's' && dir != 'w') dir = 's';
            else if (key == 'a' && dir != 'd') dir = 'a';
            else if (key == 'd' && dir != 'a') dir = 'd';
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

        // 벽 충돌 체크
        if (next.x <= 0 || next.x >= WIDTH - 1 || next.y <= 0 || next.y >= HEIGHT - 1) {
            gameOver = true;
            return;
        }

        // 자기 몸 충돌 체크
        for (int i = 0; i < length; i++) {
            if (snake[i].x == next.x && snake[i].y == next.y) {
                gameOver = true;
                return;
            }
        }

        // 몸 이동
        for (int i = length; i > 0; i--) {
            snake[i] = snake[i - 1];
        }
        snake[0] = next;

        // 먹이 먹으면 길이 증가
        if (next.x == food.x && next.y == food.y) {
            length++;
            placeFood();
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            move();
            Sleep(100);
        }
        cout << "Game Over! Your score: " << length - 1 << "\n";
        system("pause");
    }
};

void playSnake() {
    srand((unsigned)time(NULL));
    SnakeGame game;
    game.run();
}
