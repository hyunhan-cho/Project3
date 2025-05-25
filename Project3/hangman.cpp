#include "hangman.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

void printHangman(int life, int maxLife) {
    const string hangman[] = {
        "____\n |  \\\n |   O\n |  /|\\\n_|__/_",
        "____\n |  \\\n |   O\n |  /|\\\n_|___",
        "____\n |  \\\n |   O\n |  /|\n_|___",
        "____\n |  \\\n |   O\n |   |\n_|___",
        "____\n |  \\\n |   O\n |\n_|___",
        "____\n |  \\\n |\n |\n_|___",
        "\n |\n |\n |\n_|___",
        "\n\n\n\n\n"
    };
    int index = max(0, maxLife - life);
    cout << hangman[index] << "\n";
}

void playHangmanGame() {
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
        int life = 7, maxLife = 7;

        cout << "\n새로운 단어가 주어졌습니다!" << endl;

        while (life > 0 && display != word) {
            printHangman(life, maxLife);
            cout << "단어: ";
            for (char c : display) cout << c << ' ';
            cout << "\n추측한 문자들: ";
            for (char c : guessed) cout << c << ' ';

            cout << "\n남은 생명: " << life << " | 현재 점수: " << score << endl;
            cout << "문자 하나를 입력하세요: ";
            char guess;
            cin >> guess;
            guess = tolower(guess);

            if (guessed.count(guess)) {
                cout << "이미 추측한 문자입니다.\n";
                continue;
            }
            guessed.insert(guess);

            bool found = false;
            for (int i = 0; i < word.size(); ++i) {
                if (word[i] == guess) {
                    display[i] = guess;
                    found = true;
                }
            }

            if (!found) {
                --life;
                cout << "틀렸습니다! 생명 -1\n";
            }
            else {
                cout << "정답입니다!\n";
            }
        }

        if (display == word) {
            cout << "\n정답! 단어를 맞췄습니다: " << word << endl;
            score += 10;
        }
        else {
            printHangman(0, maxLife);
            cout << "\n실패! 정답은: " << word << "\n";
            score = max(0, score - 5);
        }

        cout << "현재 점수: " << score << "\n";
        cout << "다음 문제로 넘어갑니다...\n\n";
    }
}
