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

        cout << "\n���ο� �ܾ �־������ϴ�!" << endl;

        while (life > 0 && display != word) {
            printHangman(life, maxLife);
            cout << "�ܾ�: ";
            for (char c : display) cout << c << ' ';
            cout << "\n������ ���ڵ�: ";
            for (char c : guessed) cout << c << ' ';

            cout << "\n���� ����: " << life << " | ���� ����: " << score << endl;
            cout << "���� �ϳ��� �Է��ϼ���: ";
            char guess;
            cin >> guess;
            guess = tolower(guess);

            if (guessed.count(guess)) {
                cout << "�̹� ������ �����Դϴ�.\n";
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
                cout << "Ʋ�Ƚ��ϴ�! ���� -1\n";
            }
            else {
                cout << "�����Դϴ�!\n";
            }
        }

        if (display == word) {
            cout << "\n����! �ܾ ������ϴ�: " << word << endl;
            score += 10;
        }
        else {
            printHangman(0, maxLife);
            cout << "\n����! ������: " << word << "\n";
            score = max(0, score - 5);
        }

        cout << "���� ����: " << score << "\n";
        cout << "���� ������ �Ѿ�ϴ�...\n\n";
    }
}
