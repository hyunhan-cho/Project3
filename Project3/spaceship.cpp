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
const int BOMB_RADIUS = 7;  // 폭탄 반경

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

    // 별이 떨어질 확률: 약 1/30 (3.3%)
    void maybeGenerateStar() {
        if (rand() % 30 == 0) {
            Point s;
            s.x = rand() % (WIDTH - 2) + 1;
            s.y = 1;
            stars.push_back(s);
        }
    }

    // 폭탄이 떨어질 확률: 약 1/30 (3.3%)
    void maybeGenerateBomb() {
        if (rand() % 30 == 0) {
            Point b;
            b.x = rand() % (WIDTH - 2) + 1;
            b.y = 1;
            bombs.push_back(b);
        }
    }

    // 폭탄 반경 내 운석만 제거
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

    // 우주선이 차지하는 상대 좌표 목록 (6개):
    // 위쪽 3칸(ship.y-1), 아래쪽 3칸(ship.y)
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
        // 커서 (0,0)으로 이동하여 화면 전체를 다시 그린다
        COORD cursorPos = { 0, 0 };
        SetConsoleCursorPosition(hOut, cursorPos);

        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                // 1) 테두리
                if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1) {
                    SetConsoleTextAttribute(hOut, 14);
                    cout << "#";
                    continue;
                }

                bool drawn = false;
                // 2) 우주선 (각 셀 모두 '^'로 표시)
                for (auto& cell : getShipCells()) {
                    if (cell.x == x && cell.y == y) {
                        SetConsoleTextAttribute(hOut, 11); // 하늘색
                        cout << "^";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 3) 운석
                for (auto& a : asteroids) {
                    if (a.x == x && a.y == y) {
                        SetConsoleTextAttribute(hOut, 12); // 빨간색
                        cout << "*";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 4) 별
                for (auto& s : stars) {
                    if (s.x == x && s.y == y) {
                        SetConsoleTextAttribute(hOut, 10); // 초록색
                        cout << "★";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 5) 폭탄 아이템
                for (auto& b : bombs) {
                    if (b.x == x && b.y == y) {
                        SetConsoleTextAttribute(hOut, 13); // 분홍색
                        cout << "B";
                        drawn = true;
                        break;
                    }
                }
                if (drawn) continue;

                // 6) 빈 공간
                cout << " ";
            }
            cout << "\n";
        }

        // 하단 정보 표시
        SetConsoleTextAttribute(hOut, 7);
        cout << "Score: " << score << "   (Esc 키: 종료)\n";

        // 체력 표시 (하트)
        cout << "HP: ";
        SetConsoleTextAttribute(hOut, 12);
        for (int i = 0; i < MAX_HP; i++) {
            if (i < hp) cout << "♥";
            else         cout << "♡";
        }
        cout << "\n";

        // 폭탄 개수 표시
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
            // ESC = 27 → 즉시 게임 종료
            if (key == 27) {
                gameOver = true;
                return;
            }
            // 방향키
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                case 72: // 위
                    if (ship.y > 1) ship.y--;
                    break;
                case 80: // 아래
                    if (ship.y < HEIGHT - 2) ship.y++;
                    break;
                case 75: // 왼쪽
                    if (ship.x > 1) ship.x--;
                    break;
                case 77: // 오른쪽
                    if (ship.x < WIDTH - 2) ship.x++;
                    break;
                }
            }
            // 스페이스바 = 32 → 폭탄 사용
            else if (key == 32) {
                detonateBomb();
            }
        }
    }

    void update() {
        // 1) 운석/별/폭탄 위치 이동
        for (auto& a : asteroids) a.y++;
        for (auto& s : stars)   s.y++;
        for (auto& b : bombs)   b.y++;

        // 2) 운석이 바닥 닿으면 리셋 + 점수 증가 + 속도 조절
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

        // 3) 바닥 닿은 별 제거
        for (int i = 0; i < (int)stars.size(); i++) {
            if (stars[i].y >= HEIGHT - 1) {
                stars.erase(stars.begin() + i);
                i--;
            }
        }

        // 4) 바닥 닿은 폭탄 제거
        for (int i = 0; i < (int)bombs.size(); i++) {
            if (bombs[i].y >= HEIGHT - 1) {
                bombs.erase(bombs.begin() + i);
                i--;
            }
        }

        // 5) 운석과 충돌 시: 체력 감소 + 운석 리셋
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

        // 6) 별 먹기 (체력 회복 + 별 제거)
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

        // 7) 폭탄 먹기 (bombCount 증가 + 폭탄 제거)
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

        // 8) 별/폭탄 새로 생성
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
        cout << " ============== 우주선 피하기 게임 조작법 ==============\n\n";
        cout << " - 방향키(↑↓←→)로 우주선을 움직여 운석(*)을 피하세요\n";
        cout << " - 운석에 닿으면 체력이 줄어들고, 0이 되면 게임이 종료됩니다\n";
        cout << " - 떨어지는 별(★)을 먹으면 체력이 1 회복됩니다\n";
        cout << " - 떨어지는 폭탄(B)을 먹으면 반경 " << BOMB_RADIUS
            << "칸 이내 운석이 제거됩니다\n";
        cout << " - 스페이스바를 눌러 폭탄을 즉시 사용할 수 있습니다\n";
        cout << " - 점수가 오를수록 속도가 빨라집니다\n";
        cout << " - Esc 키를 누르면 게임을 종료할 수 있습니다\n\n";
        SetConsoleTextAttribute(hOut, 7);
        cout << " Enter 키를 눌러 게임을 시작하세요...\n";

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
        cout << "\nGame Over! 최종 점수 : " << score << "\n";
        SetConsoleTextAttribute(hOut, 7);
        system("pause");
    }
};

void playSpaceship() {
    srand((unsigned)time(NULL));
    Spaceship game;
    game.run();
}
