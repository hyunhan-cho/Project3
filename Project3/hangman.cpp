#include "hangman.h"
#include "utils.h"      // setColor, clearScreen �Լ��� ����ϱ� ���� ����
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <conio.h>      // _kbhit, _getch�� ����ϱ� ���� ���� (Windows ���)

using namespace std;
using namespace chrono;

// ��� �׸��� ����ϴ� �Լ�
// ANSI �ڵ� ��� setColor �Լ��� ����ϵ��� ����
void printHangman(int life, int maxLife) {
    // 0:��� ~ 7:Ǯ��
    const string hangmanPics[][5] = {
        {"____ " , " |  \\", " |   O", " |  /|\\", "_|__/_"}, // 0
        {"____ " , " |  \\", " |   O", " |  /|\\", "_|___ "}, // 1
        {"____ " , " |  \\", " |   O", " |  /| ", "_|___ "}, // 2
        {"____ " , " |  \\", " |   O", " |   | ", "_|___ "}, // 3
        {"____ " , " |  \\", " |   O", " |     ", "_|___ "}, // 4
        {"____ " , " |  \\", " |    ", " |     ", "_|___ "}, // 5
        {"     " , " |    ", " |    ", " |     ", "_|___ "}, // 6
        {"     " , "      ", "      ", "      ", "      "}  // 7
    };

    // ���� ����(life)�� ���� ǥ���� �׸��� �ε����� ���
    int index = max(0, life);
    if (life == 0) index = 0;
    else index = maxLife - life;


    setColor(12); // ���� ������
    for (const auto& line : hangmanPics[index]) {
        cout << line << endl;
    }
    setColor(7); // �⺻ �������� ����
}

// ���� ���� �Լ�
void playHangmanGame() {
    // �ܾ� ���
    vector<string> wordList = {
        "apple", "banana", "computer", "elephant", "mountain",
        "school", "breakfast", "library", "notebook", "programming",
        "language", "science", "history", "engineer", "universe"
    };
    srand(static_cast<unsigned int>(time(0)));
    int score = 0;

    while (true) {
        string word = wordList[rand() % wordList.size()];
        unordered_set<char> guessed;
        string display(word.size(), '_');
        int life = 7;
        const int maxLife = 7;
        auto startTime = steady_clock::now();
        const int timeLimit = 120; // 2��

        while (life > 0 && display != word) {
            clearScreen(); // utils.h�� clearScreen() ���
            auto now = steady_clock::now();
            int elapsed = duration_cast<seconds>(now - startTime).count();
            int remaining = timeLimit - elapsed;

            if (remaining <= 0) {
                clearScreen();
                printHangman(0, maxLife);
                setColor(12);
                cout << "\n�ð� �ʰ�! ������: " << word << "\n";
                setColor(7);
                score = max(0, score - 5);
                break;
            }

            printHangman(life, maxLife);

            setColor(11); cout << "�ܾ�: ";
            setColor(15);
            for (char c : display) cout << c << ' ';

            cout << "\n";
            setColor(14); cout << "������ ���ڵ�: ";
            setColor(15);
            for (char c : guessed) cout << c << ' ';

            cout << "\n";
            setColor(13); cout << "���� ����: " << life;
            setColor(7); cout << " | ";
            setColor(10); cout << "���� ����: " << score;
            setColor(7); cout << " | ";
            setColor(11); cout << "���� �ð�: " << remaining << "��" << endl;

            setColor(11); cout << "���� �ϳ��� �Է��ϼ��� (ESC ����): ";
            setColor(7);

            // �Է� ��� (�ð� �ʰ� Ȯ�� ����)
            char guess;
            while (!_kbhit()) {
                this_thread::sleep_for(milliseconds(100));
                now = steady_clock::now();
                if (duration_cast<seconds>(now - startTime).count() >= timeLimit) {
                    remaining = 0;
                    break;
                }
            }
            if (remaining <= 0) continue;

            guess = _getch();
            if (guess == 27) { // ESC Ű
                setColor(12);
                cout << "\n������ �����մϴ�.\n";
                setColor(7);
                return; // ���� �޴��� ����
            }

            guess = tolower(guess);
            // ���ĺ��� �ƴ� ��� ����
            if (!isalpha(guess)) {
                continue;
            }

            // �̹� ������ �������� Ȯ��
            if (guessed.count(guess)) {
                setColor(14);
                cout << "\n�̹� ������ �����Դϴ�.\n";
                setColor(7);
                this_thread::sleep_for(seconds(1));
                continue;
            }
            guessed.insert(guess);

            // �ܾ ���ڰ� �ִ��� Ȯ��
            bool found = false;
            size_t pos = word.find(guess);
            if (pos != string::npos) {
                for (size_t i = 0; i < word.size(); ++i) {
                    if (word[i] == guess) {
                        display[i] = guess;
                    }
                }
                found = true;
            }

            cout << endl;
            if (!found) {
                --life;
                setColor(12);
                cout << "��! Ʋ�Ƚ��ϴ�! ������ �ϳ� �پ����.\n";
                setColor(7);
            }
            else {
                setColor(10);
                cout << "����! �ܾ ���Ե� �����Դϴ�!\n";
                setColor(7);
            }
            this_thread::sleep_for(seconds(1));
        }

        clearScreen();
        if (display == word) {
            printHangman(life, maxLife);
            setColor(10);
            cout << "\n�����մϴ�! �ܾ ������ϴ�: " << word << endl;
            score += 10;
        }
        else if (life == 0) {
            printHangman(0, maxLife);
            setColor(12);
            cout << "\n���� ����! ������: " << word << "\n";
            score = max(0, score - 5);
        }

        setColor(10);
        cout << "���� ����: " << score << "\n";
        setColor(7);
        cout << "\n����Ϸ��� �ƹ� Ű�� �������� (ESC�� ������ �޴��� ���ư��ϴ�)...\n";

        char key = _getch();
        if (key == 27) {
            return;
        }
    }
}