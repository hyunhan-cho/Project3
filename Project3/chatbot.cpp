#include <iostream>
#include <iomanip> // 추가: setw 사용을 위한 헤더
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <conio.h>
#include <windows.h>

using namespace std;

struct Game {
    string id;
    string name;
    vector<string> keywords;
    string description;
};

vector<Game> initializeGames() {
    vector<Game> games;
    games.push_back({ "1", "테트리스", {"테트리스", "tetris"}, "- 블록을 회전시켜 가로줄을 완성하면 점수가 올라가는 고전 퍼즐 게임입니다. 빠르게 내려오는 블록을 잘 조절해 높은 점수를 기록해보세요." });
    games.push_back({ "2", "지뢰찾기", {"지뢰찾기", "지뢰", "minesweeper"}, "- 숫자를 단서로 삼아 지뢰가 없는 칸을 찾아내는 논리 추리 게임입니다. 모든 안전한 칸을 열면 승리합니다." });
    games.push_back({ "3", "미로찾기", {"미로찾기", "미로", "maze"}, "- 플레이어 'P'를 출구 'E'로 이동시키는 길찾기 게임입니다. W/A/S/D 키로 이동하며, Z 키로 이전 위치로 돌아갈 수 있습니다." });
    games.push_back({ "4", "스네이크", {"스네이크", "뱀", "snake"}, "- 먹이를 먹으며 점점 길어지는 뱀을 조작하여 최대한 오래 생존하는 게임입니다. 몸에 부딪히면 게임 오버!" });
    games.push_back({ "5", "스도쿠", {"스도쿠", "sudoku"}, "- 9x9 퍼즐 판에 숫자 1부터 9까지를 겹치지 않도록 채워넣는 숫자 퍼즐입니다. 가로, 세로, 3x3 구역을 모두 만족시켜야 합니다." });
    games.push_back({ "6", "행맨", {"행맨", "hangman"}, "- 영어 단어를 추측하는 게임으로, 글자를 틀릴 때마다 행맨이 조금씩 그려집니다. 기회 내에 단어를 완성해야 합니다." });
    games.push_back({ "7", "묵찌빠", {"묵찌빠", "묵찌빠 게임"}, "- 가위바위보에서 이긴 사람이 공격권을 얻고, 같은 손동작을 내면 승리하는 심리전 게임입니다." });
    games.push_back({ "8", "슈팅 게임", {"슈팅", "슈팅 게임", "shooting"}, "- 방향키로 비행기를 조종하며 적을 피하고 공격하는 슈팅 게임입니다. 탄환을 잘 피하고 높은 점수를 노려보세요." });
    games.push_back({ "9", "풍 게임", {"풍", "풍 게임", "pong"}, "- 양쪽 막대를 조작해 튕겨나오는 공을 받아치는 대전형 고전 게임입니다. 반사 타이밍이 승부의 열쇠입니다." });
    return games;
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void printChatbotHeader() {
    system("cls");
    cout << "\n==============================" << endl;
    cout << "[ 게임 설명 챗봇에 오신 걸 환영합니다! ]" << endl;
    cout << "==============================\n" << endl;
    cout << "게임 이름이나 번호를 입력하세요." << endl;
    cout << "뒤로 가려면 ESC를 누르세요.\n" << endl;
}

void printGameList(const vector<Game>& games) {
    cout << "┌──────────────────────────────────────────────┐\n";
    cout << "│                게임 목록                     │\n";
    cout << "├──────────────────────────────────────────────┤\n";
    for (size_t i = 0; i < games.size(); ++i) {
        cout << "│ [" << games[i].id << "] " << left << setw(25) << games[i].name;
        if (i % 2 == 1 || i == games.size() - 1) cout << "│\n";
        else cout << "\t";
    }
    cout << "└──────────────────────────────────────────────┘\n" << endl;
}

void chatbotMenu() {
    vector<Game> games = initializeGames();

    while (true) {
        printChatbotHeader();
        printGameList(games);

        cout << "게임 이름 또는 번호 입력 >> ";
        string input;

        // ESC 체크
        char ch = _getch();
        if (ch == 27) {
            cout << "\n메인 메뉴로 돌아갑니다...\n";
            Sleep(1000);
            return;
        }
        else {
            cout << ch;
            getline(cin, input);
            input = ch + input;  // 첫 문자 + 나머지
        }

        string lowerInput = toLower(input);
        bool found = false;

        for (const auto& game : games) {
            if (lowerInput == game.id || toLower(game.name).find(lowerInput) != string::npos) {
                found = true;
                system("cls");
                cout << "\n==============================" << endl;
                cout << "[ " << game.name << " 설명 ]" << endl;
                cout << "==============================" << endl;
                cout << game.description << "\n" << endl;
                break;
            }
            for (const auto& keyword : game.keywords) {
                if (toLower(keyword).find(lowerInput) != string::npos) {
                    found = true;
                    system("cls");
                    cout << "\n==============================" << endl;
                    cout << "[ " << game.name << " 설명 ]" << endl;
                    cout << "==============================" << endl;
                    cout << game.description << "\n" << endl;
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            cout << "\n[ 찾을 수 없습니다 ]\n";
            cout << "입력한 '" << input << "' 에 해당하는 게임이 없습니다. 다시 입력해 주세요.\n";
        }

        cout << "\n아무 키나 누르면 계속합니다...";
        _getch();
    }
}
