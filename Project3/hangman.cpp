#include "hangman.h"
#include "utils.h"      // setColor, clearScreen 함수를 사용하기 위해 포함
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <conio.h>      // _kbhit, _getch를 사용하기 위해 포함 (Windows 방식)

using namespace std;
using namespace chrono;

// 행맨 그림을 출력하는 함수
// ANSI 코드 대신 setColor 함수를 사용하도록 수정
void printHangman(int life, int maxLife) {
    // 0:사망 ~ 7:풀피
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

    // 남은 생명(life)에 따라 표시할 그림의 인덱스를 계산
    int index = max(0, life);
    if (life == 0) index = 0;
    else index = maxLife - life;


    setColor(12); // 밝은 빨간색
    for (const auto& line : hangmanPics[index]) {
        cout << line << endl;
    }
    setColor(7); // 기본 색상으로 복원
}

// 메인 게임 함수
void playHangmanGame() {
    // 단어 목록
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
        const int timeLimit = 120; // 2분

        while (life > 0 && display != word) {
            clearScreen(); // utils.h의 clearScreen() 사용
            auto now = steady_clock::now();
            int elapsed = duration_cast<seconds>(now - startTime).count();
            int remaining = timeLimit - elapsed;

            if (remaining <= 0) {
                clearScreen();
                printHangman(0, maxLife);
                setColor(12);
                cout << "\n시간 초과! 정답은: " << word << "\n";
                setColor(7);
                score = max(0, score - 5);
                break;
            }

            printHangman(life, maxLife);

            setColor(11); cout << "단어: ";
            setColor(15);
            for (char c : display) cout << c << ' ';

            cout << "\n";
            setColor(14); cout << "추측한 문자들: ";
            setColor(15);
            for (char c : guessed) cout << c << ' ';

            cout << "\n";
            setColor(13); cout << "남은 생명: " << life;
            setColor(7); cout << " | ";
            setColor(10); cout << "현재 점수: " << score;
            setColor(7); cout << " | ";
            setColor(11); cout << "남은 시간: " << remaining << "초" << endl;

            setColor(11); cout << "문자 하나를 입력하세요 (ESC 종료): ";
            setColor(7);

            // 입력 대기 (시간 초과 확인 포함)
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
            if (guess == 27) { // ESC 키
                setColor(12);
                cout << "\n게임을 종료합니다.\n";
                setColor(7);
                return; // 메인 메뉴로 복귀
            }

            guess = tolower(guess);
            // 알파벳이 아닌 경우 무시
            if (!isalpha(guess)) {
                continue;
            }

            // 이미 추측한 문자인지 확인
            if (guessed.count(guess)) {
                setColor(14);
                cout << "\n이미 추측한 문자입니다.\n";
                setColor(7);
                this_thread::sleep_for(seconds(1));
                continue;
            }
            guessed.insert(guess);

            // 단어에 문자가 있는지 확인
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
                cout << "땡! 틀렸습니다! 생명이 하나 줄어들어요.\n";
                setColor(7);
            }
            else {
                setColor(10);
                cout << "정답! 단어에 포함된 문자입니다!\n";
                setColor(7);
            }
            this_thread::sleep_for(seconds(1));
        }

        clearScreen();
        if (display == word) {
            printHangman(life, maxLife);
            setColor(10);
            cout << "\n축하합니다! 단어를 맞췄습니다: " << word << endl;
            score += 10;
        }
        else if (life == 0) {
            printHangman(0, maxLife);
            setColor(12);
            cout << "\n게임 오버! 정답은: " << word << "\n";
            score = max(0, score - 5);
        }

        setColor(10);
        cout << "현재 점수: " << score << "\n";
        setColor(7);
        cout << "\n계속하려면 아무 키나 누르세요 (ESC를 누르면 메뉴로 돌아갑니다)...\n";

        char key = _getch();
        if (key == 27) {
            return;
        }
    }
}