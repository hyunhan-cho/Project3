#include "spaceship.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 20;
const int MAX_HP = 3;
const int BOMB_RADIUS = 7;  // ��ź �ݰ�

struct Point {
    int x, y;
};

class Spaceship {
public:
    Point ship;
    vector<Point> asteroids;
    vector<Point> stars;
    vector<Point> bombs;

    int score;
    int speed;
    int hp;
    int bombCount;
    bool gameOver;

    Spaceship() {
        ship = { WIDTH / 2, HEIGHT - 2 };
        score = 0;
        speed = 100;
        hp = MAX_HP;
        bombCount = 0;
        gameOver = false;
        generateAsteroids(15);
    }

    void generateAsteroids(int count) {
        asteroids.clear();
        for (int i = 0; i < count; i++) {
            Point a;
            a.x = rand() % (WIDTH - 2) + 1;
            a.y = rand() % (HEIGHT / 2);
            asteroids.push_back(a);
        }
    }

    // ���� ������ Ȯ��: �� 1/30 (3.3%)
    void maybeGenerateStar() {
        if (rand() % 30 == 0) {
            Point s;
            s.x = rand() % (WIDTH - 2) + 1;
            s.y = 1;
            stars.push_back(s);
        }
    }

    // ��ź�� ������ Ȯ��: �� 1/30 (3.3%)
    void maybeGenerateBomb() {
        if (rand() % 30 == 0) {
            Point b;
            b.x = rand() % (WIDTH - 2) + 1;
            b.y = 1;
            bombs.push_back(b);
        }
    }

    // ��ź �ݰ� �� ��� ����
    void detonateBomb() {
        if (bombCount <= 0) return;
        bombCount--;

        vector<Point> survivors;
        survivors.reserve(asteroids.size());
        for (auto& a : asteroids) {
            double dx = a.x - ship.x;
            double dy = a.y - ship.y;
            double dist = sqrt(dx * dx + dy * dy);
            if (dist > BOMB_RADIUS) {
                survivors.push_back(a);
            }
        }
        asteroids.swap(survivors);

        draw();
    }

    // ���ּ��� �����ϴ� ��� ��ǥ ��� (6��):
    // ���� 3ĭ(ship.y-1), �Ʒ��� 3ĭ(ship.y)
    vector<Point> getShipCells() {
        return {
            { ship.x - 1, ship.y - 1 },
            { ship.x    , ship.y - 1 },
            { ship.x + 1, ship.y - 1 },
            { ship.x - 1, ship.y     },
            { ship.x    , ship.y     },
            { ship.x + 1, ship.y     }
        };
    }

    void draw() {
        // Ŀ�� (0,0)���� �̵��Ͽ� ȭ�� ��ü�� �ٽ� �׸���
        COORD cursorPos = { 0, 0 };
        SetConsoleCursorPosition(hOut, cursorPos);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // 1) �׵θ�
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    SetConsoleTextAttribute(hOut, 14);
                    cout << "#";
                    continue;
                }

                bool drawn = false;
                // 2) ���ּ� (�� �� ��� '^'�� ǥ��)
                for (auto& cell : getShipCells()) {
                    if (cell.x == x && cell.y == y) {
                        SetConsoleTextAttribute(hOut, 11); // �ϴû�
                        cout << "^";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 3) �
                for (auto& a : asteroids) {
                    if (a.x == x && a.y == y) {
                        SetConsoleTextAttribute(hOut, 12); // ������
                        cout << "*";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 4) ��
                for (auto& s : stars) {
                    if (s.x == x && s.y == y) {
                        SetConsoleTextAttribute(hOut, 10); // �ʷϻ�
                        cout << "��";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 5) ��ź ������
                for (auto& b : bombs) {
                    if (b.x == x && b.y == y) {
                        SetConsoleTextAttribute(hOut, 13); // ��ȫ��
                        cout << "B";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 6) �� ����
                cout << " ";
            }
            cout << "\n";
        }

        // �ϴ� ���� ǥ��
        SetConsoleTextAttribute(hOut, 7);
        cout << "Score: " << score << "   (Esc Ű: ����)\n";

        // ü�� ǥ�� (��Ʈ)
        cout << "HP: ";
        SetConsoleTextAttribute(hOut, 12);
        for (int i = 0; i < MAX_HP; i++) {
            if (i < hp) cout << "��";
            else         cout << "��";
        }
        cout << "\n";

        // ��ź ���� ǥ��
        cout << "Bombs: ";
        SetConsoleTextAttribute(hOut, 13);
        for (int i = 0; i < bombCount; i++) {
            cout << "B ";
        }
        cout << "\n";

        SetConsoleTextAttribute(hOut, 7);
    }

    void input() {
        if (_kbhit()) {
            int key = _getch();
            // ESC = 27 �� ��� ���� ����
            if (key == 27) {
                gameOver = true;
                return;
            }
            // ����Ű
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                case 72: // ��
                    if (ship.y > 1) ship.y--;
                    break;
                case 80: // �Ʒ�
                    if (ship.y < HEIGHT - 2) ship.y++;
                    break;
                case 75: // ����
                    if (ship.x > 1) ship.x--;
                    break;
                case 77: // ������
                    if (ship.x < WIDTH - 2) ship.x++;
                    break;
                }
            }
            // �����̽��� = 32 �� ��ź ���
            else if (key == 32) {
                detonateBomb();
            }
        }
    }

    void update() {
        // 1) �/��/��ź ��ġ �̵�
        for (auto& a : asteroids) a.y++;
        for (auto& s : stars)   s.y++;
        for (auto& b : bombs)   b.y++;

        // 2) ��� �ٴ� ������ ���� + ���� ���� + �ӵ� ����
        for (auto& a : asteroids) {
            if (a.y >= HEIGHT - 1) {
                a.x = rand() % (WIDTH - 2) + 1;
                a.y = 1;
                score++;
                if (score % 5 == 0 && speed > 60) {
                    speed -= 2;
                }
            }
        }

        // 3) �ٴ� ���� �� ����
        for (int i = 0; i < (int)stars.size(); i++) {
            if (stars[i].y >= HEIGHT - 1) {
                stars.erase(stars.begin() + i);
                i--;
            }
        }

        // 4) �ٴ� ���� ��ź ����
        for (int i = 0; i < (int)bombs.size(); i++) {
            if (bombs[i].y >= HEIGHT - 1) {
                bombs.erase(bombs.begin() + i);
                i--;
            }
        }

        // 5) ��� �浹 ��: ü�� ���� + � ����
        for (auto& a : asteroids) {
            for (auto& cell : getShipCells()) {
                if (a.x == cell.x && a.y == cell.y) {
                    hp--;
                    a.y = 1;
                    a.x = rand() % (WIDTH - 2) + 1;
                    if (hp <= 0) {
                        gameOver = true;
                    }
                    break;
                }
            }
        }

        // 6) �� �Ա� (ü�� ȸ�� + �� ����)
        for (int i = 0; i < (int)stars.size(); i++) {
            for (auto& cell : getShipCells()) {
                if (stars[i].x == cell.x && stars[i].y == cell.y) {
                    if (hp < MAX_HP) hp++;
                    stars.erase(stars.begin() + i);
                    i--;
                    break;
                }
            }
        }

        // 7) ��ź �Ա� (bombCount ���� + ��ź ����)
        for (int i = 0; i < (int)bombs.size(); i++) {
            for (auto& cell : getShipCells()) {
                if (bombs[i].x == cell.x && bombs[i].y == cell.y) {
                    bombCount++;
                    bombs.erase(bombs.begin() + i);
                    i--;
                    break;
                }
            }
        }

        // 8) ��/��ź ���� ����
        maybeGenerateStar();
        maybeGenerateBomb();
    }

    void hideCursor() {
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }

    void showInstructions() {
        system("cls");
        hideCursor();
        SetConsoleTextAttribute(hOut, 11);
        cout << "\n";
        cout << " ============== ���ּ� ���ϱ� ���� ���۹� ==============\n\n";
        cout << " - ����Ű(�����)�� ���ּ��� ������ �(*)�� ���ϼ���\n";
        cout << " - ��� ������ ü���� �پ���, 0�� �Ǹ� ������ ����˴ϴ�\n";
        cout << " - �������� ��(��)�� ������ ü���� 1 ȸ���˴ϴ�\n";
        cout << " - �������� ��ź(B)�� ������ �ݰ� " << BOMB_RADIUS
            << "ĭ �̳� ��� ���ŵ˴ϴ�\n";
        cout << " - �����̽��ٸ� ���� ��ź�� ��� ����� �� �ֽ��ϴ�\n";
        cout << " - ������ �������� �ӵ��� �������ϴ�\n";
        cout << " - Esc Ű�� ������ ������ ������ �� �ֽ��ϴ�\n\n";
        SetConsoleTextAttribute(hOut, 7);
        cout << " Enter Ű�� ���� ������ �����ϼ���...\n";

        while (true) {
            if (_kbhit()) {
                char c = _getch();
                if (c == 13) break;
            }
            Sleep(100);
        }
    }

    void run() {
        system("cls");
        hideCursor();

        showInstructions();

        DWORD lastUpdateTime = GetTickCount();

        while (!gameOver) {
            draw();
            input();

            DWORD currentTime = GetTickCount();
            if (currentTime - lastUpdateTime >= (DWORD)speed) {
                update();
                lastUpdateTime = currentTime;
            }

            Sleep(10);
        }

        SetConsoleTextAttribute(hOut, 12);
        cout << "\nGame Over! ���� ���� : " << score << "\n";
        SetConsoleTextAttribute(hOut, 7);
        system("pause");
    }
};

void playSpaceship() {
    srand((unsigned)time(NULL));
    Spaceship game;
    game.run();
}
