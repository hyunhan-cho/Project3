// chatbot.cpp

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <sstream>
#include <random>
#include <chrono>
#include <Windows.h>
#include "utils.h"
#include "chatbot.h"

using namespace std;

vector<Game> initializeGames() {
    return {
        {"1", "테트리스", {"테트리스", "tetris", "블록", "퍼즐"},
         "블록을 회전시켜 가로줄을 완성하는 고전 퍼즐 게임입니다. 점수를 높이기 위해 블록을 잘 배치하세요!",
         "중급", "퍼즐", 95},
        {"2", "지뢰찾기", {"지뢰찾기", "지뢰", "minesweeper", "추리"},
         "숫자를 힌트로 지뢰가 없는 칸을 추리하는 게임입니다. 안전한 칸을 모두 열면 승리합니다.",
         "중급", "퍼즐", 85},
        {"3", "미로찾기", {"미로", "maze", "탈출", "길찾기"},
         "P(플레이어)를 W/A/S/D로 이동시켜 E(출구)까지 도달하세요. Z로 되돌아갈 수 있어요.",
         "초급", "어드벤처", 70},
        {"4", "스네이크", {"스네이크", "뱀", "snake", "성장"},
         "먹이를 먹으며 성장하는 뱀을 조작하세요. 몸에 부딪히면 게임 오버!",
         "초급", "아케이드", 90},
        {"5", "스도쿠", {"스도쿠", "sudoku", "숫자", "논리"},
         "9x9 숫자 퍼즐! 행, 열, 3x3 구역에 1~9가 중복되지 않도록 채우세요.",
         "고급", "퍼즐", 80},
        {"6", "행맨", {"행맨", "hangman", "단어", "영어"},
         "영어 단어를 추측하세요. 틀릴수록 행맨이 그려집니다. 생명을 다 쓰면 패배!",
         "중급", "교육", 75},
        {"7", "우주선게임", {"우주선", "spaceship", "생존", "우주"},
         "운석을 피하고 아이템을 먹으며 생존하세요. 스페이스바로 폭탄을 사용!",
         "중급", "액션", 88},
        {"8", "슈팅 게임", {"슈팅", "shooting", "공격", "전투"},
         "비행기를 조작해 적을 공격하세요. 고득점을 노려보세요! (개발 중)",
         "중급", "액션", 82},
        {"9", "묵찌빠", {"묵찌빠", "가위바위보", "심리전"},
         "가위바위보 심리전 게임입니다. 같은 손을 내면 이긴 사람이 승리! (개발 중)",
         "초급", "전략", 60},
    };
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
            cout << "│                          ";
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
    cout << "난이도: " << game.difficulty << " | 장르: " << game.genre << "\n";
}

void chatbotMenu() {
    clearScreen();
    setColor(14);
    cout << "=== 게임 도우미 모드 선택 ===\n\n";
    setColor(7);
    cout << "1. [*] AI 대화형 모드 (권장)\n";
    cout << "2. [#] 기존 목록형 모드\n";
    cout << "\n선택하세요 (1-2): ";

    char choice = _getch();

    if (choice == '1') {
        enhancedChatbotMenu();
    }
    else if (choice == '2') {
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
    else {
        enhancedChatbotMenu();
    }
}
