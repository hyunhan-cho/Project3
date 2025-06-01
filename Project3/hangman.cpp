// hangman.cpp
#include "hangman.h"
#include "utils.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

void printHangman(int life, int maxLife) {
    const string hangman[] = {
        "\033[1;31m____\n |  \\\n |   O\n |  /|\\\n_|__/_\033[0m",
        "\033[1;31m____\n |  \\\n |   O\n |  /|\\\n_|___\033[0m",
        "\033[1;31m____\n |  \\\n |   O\n |  /|\n_|___\033[0m",
        "\033[1;31m____\n |  \\\n |   O\n |   |\n_|___\033[0m",
        "\033[1;31m____\n |  \\\n |   O\n |\n_|___\033[0m",
        "\033[1;31m____\n |  \\\n |\n |\n_|___\033[0m",
        "\033[1;31m\n |\n |\n |\n_|___\033[0m",
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
        clearScreen();
        string word = wordList[rand() % wordList.size()];
        unordered_set<char> guessed;
        string display(word.size(), '_');
        int life = 7, maxLife = 7;

        while (life > 0 && display != word) {
            clearScreen();
            printHangman(life, maxLife);
            cout << "\033[1;36m단어: \033[0m";
            for (char c : display) cout << c << ' ';
            cout << "\n\033[1;33m추측한 문자들: \033[0m";
            for (char c : guessed) cout << c << ' ';

            cout << "\n\033[1;35m남은 생명: \033[0m" << life << " | \033[1;32m현재 점수: \033[0m" << score << endl;
            cout << "\033[1;36m문자 하나를 입력하세요: \033[0m";
            char guess;
            cin >> guess;
            guess = tolower(guess);

            if (guessed.count(guess)) {
                cout << "\033[1;33m이미 추측한 문자입니다.\033[0m\n";
                cin.ignore(); cin.get();
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
                cout << "\033[1;31m틀렸습니다! 생명 -1\033[0m\n";
            }
            else {
                cout << "\033[1;32m정답입니다!\033[0m\n";
            }
            cin.ignore(); cin.get();
        }

        clearScreen();
        if (display == word) {
            printHangman(life, maxLife);
            cout << "\n\033[1;32m정답! 단어를 맞췄습니다: \033[0m" << word << endl;
            score += 10;
        }
        else {
            printHangman(0, maxLife);
            cout << "\n\033[1;31m실패! 정답은: \033[0m" << word << "\n";
            score = max(0, score - 5);
        }

        cout << "\033[1;36m현재 점수: \033[0m" << score << "\n";
        cout << "\033[1;36m계속하려면 아무 키나 누르세요...\033[0m";
        cin.ignore(); cin.get();
    }
}
