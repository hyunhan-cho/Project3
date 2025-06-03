// chatbot.cpp
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include "utils.h" // clearScreen, pauseScreen 사용
#include <Windows.h>
#include "chatbot.h"

using namespace std;

struct Game {
    string id;
    string name;
    vector<string> keywords;
    string description;
};

vector<Game> initializeGames() {
    return {
        {"1", "테트리스", {"테트리스", "tetris"}, "- 블록을 회전시켜 가로줄을 완성하는 고전 퍼즐 게임입니다. 점수를 높이기 위해 블록을 잘 배치하세요!"},
        {"2", "지뢰찾기", {"지뢰찾기", "지뢰", "minesweeper"}, "- 숫자를 힌트로 지뢰가 없는 칸을 추리하는 게임입니다. 안전한 칸을 모두 열면 승리합니다."},
        {"3", "미로찾기", {"미로", "maze"}, "- P(플레이어)를 W/A/S/D로 이동시켜 E(출구)까지 도달하세요. Z로 되돌아갈 수 있어요."},
        {"4", "스네이크", {"스네이크", "뱀", "snake"}, "- 먹이를 먹으며 성장하는 뱀을 조작하세요. 몸에 부딪히면 게임 오버!"},
        {"5", "스도쿠", {"스도쿠", "sudoku"}, "- 9x9 숫자 퍼즐! 행, 열, 3x3 구역에 1~9가 중복되지 않도록 채우세요."},
        {"6", "행맨", {"행맨", "hangman"}, "- 영어 단어를 추측하세요. 틀릴수록 행맨이 그려집니다. 생명을 다 쓰면 패배!"},
        {"7", "우주선게임", {"우주선", "spaceship"}, "- 운석을 피하고 아이템을 먹으며 생존하세요. 스페이스바로 폭탄을 사용!"},
        {"8", "슈팅 게임", {"슈팅", "shooting"}, "- 비행기를 조작해 적을 공격하세요. 고득점을 노려보세요! (개발 중)"},
        {"9", "묵찌빠", {"묵찌빠"}, "- 가위바위보 심리전 게임입니다. 같은 손을 내면 이긴 사람이 승리! (개발 중)"},
    };
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

extern void setColor(int color); // main.cpp에서 정의된 setColor 사용

void printChatbotHeader() {
    clearScreen();
    setColor(11);
    cout << R"(
   ____ _           _     _           _     
  / ___| |__   __ _| |_  | |__   ___ | |_   
 | |   | '_ \ / _` | __| | '_ \ / _ \| __|  
 | |___| | | | (_| | |_  | | | | (_) | |_   
  \____|_| |_|\__,_|\__| |_|_|_|\___/ \__|  
                                            
    )" << endl;
    setColor(14);
    cout << "게임 이름 또는 번호를 입력해 주세요! (ESC: 나가기)\n\n";
    setColor(7);
}

void printGameList(const vector<Game>& games) {
    cout << "┌──────────────────────────────────────────────┐\n";
    cout << "│                게임 목록                     │\n";
    cout << "├──────────────────────────────────────────────┤\n";

    for (size_t i = 0; i < games.size(); i += 2) {
        cout << "│ [" << games[i].id << "] " << setw(12) << left << games[i].name;

        if (i + 1 < games.size()) {
            cout << "│ [" << games[i + 1].id << "] " << setw(12) << left << games[i + 1].name;
        }
        else {
            cout << "│                          ";  // 빈 열 여백 확보
        }

        cout << "│" << endl;
    }

    cout << "└──────────────────────────────────────────────┘\n\n";
}



void printGameDescription(const Game& game) {
    setColor(10);
    cout << "\n[ " << game.name << " 설명 ]\n";
    setColor(7);
    cout << game.description << "\n";
}

void chatbotMenu() {
    vector<Game> games = initializeGames();

    while (true) {
        printChatbotHeader();
        printGameList(games);

        cout << "▶ 게임 이름 또는 번호 입력 >> ";
        string input;

        char ch = _getch();
        if (ch == 27) {
            cout << "\n메인 메뉴로 돌아갑니다...\n";
            Sleep(1000);
            return;
        }
        else {
            cout << ch;
            getline(cin, input);
            input = ch + input;
        }

        string lowerInput = toLower(input);
        bool found = false;

        for (const auto& game : games) {
            if (lowerInput == game.id || toLower(game.name).find(lowerInput) != string::npos) {
                found = true;
                printGameDescription(game);
                break;
            }
            for (const auto& keyword : game.keywords) {
                if (toLower(keyword).find(lowerInput) != string::npos) {
                    found = true;
                    printGameDescription(game);
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            setColor(12);
            cout << "\n[ 입력한 '" << input << "' 에 해당하는 게임을 찾을 수 없습니다 ]\n";
            setColor(7);
        }

        pauseScreen();
    }
}