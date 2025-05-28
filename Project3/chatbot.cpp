#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // transform, find_if
#include <limits>    // numeric_limits (cin.ignore를 위해)

// C++11의 to_string을 사용하기 위함 (혹은 다른 방식으로 숫자-문자열 변환)
// #include <string> // 이미 포함됨

using namespace std;

// 게임 정보를 담을 구조체
struct Game {
    string id;                 // 게임 번호 (문자열로 처리)
    string name;               // 게임 정식 명칭
    vector<string> keywords;   // 검색을 위한 키워드 (게임 이름 포함)
    string description;        // 게임 설명
};

// 게임 목록을 초기화하는 함수
vector<Game> initializeGames() {
    vector<Game> games;
    games.push_back({ "1", "테트리스", {"테트리스", "tetris"}, "- 블록을 회전하며 줄을 맞춰서 없애는 퍼즐 게임입니다." });
    games.push_back({ "2", "지뢰찾기", {"지뢰찾기", "지뢰", "minesweeper"}, "- 지뢰가 없는 칸을 열고, 숫자로 주변 지뢰 개수를 추리합니다." });
    games.push_back({ "3", "미로찾기", {"미로찾기", "미로", "maze"}, "- P에서 E까지 도착하는 길을 찾는 게임입니다.\n  W A S D 키로 이동하며, Z 키로 한 칸 뒤로 돌아갈 수 있습니다." });
    games.push_back({ "4", "스네이크", {"스네이크", "뱀", "snake"}, "- 뱀을 조작해 먹이를 먹고 점점 길어지는 고전 게임입니다." });
    games.push_back({ "5", "스도쿠", {"스도쿠", "sudoku"}, "- 가로, 세로, 3x3 구역에 1~9 숫자를 겹치지 않게 채우는 퍼즐 게임입니다." });
    games.push_back({ "6", "행맨", {"행맨", "hangman"}, "- 단어를 맞추는 게임입니다. 틀릴수록 행맨이 점점 완성됩니다." });
    games.push_back({ "7", "묵찌빠", {"묵찌빠", "묵찌빠 게임"}, "- 가위바위보 후 공격/수비를 정하고, 같은 것을 내면 이기는 게임입니다." }); // 설명 약간 수정
    games.push_back({ "8", "슈팅 게임", {"슈팅", "슈팅 게임", "shooting"}, "- 적을 피하면서 총알로 공격하는 아케이드 스타일 게임입니다." });
    games.push_back({ "9", "풍 게임", {"풍", "풍 게임", "pong"}, "- 화면 양 끝의 막대를 움직여 공을 받아치는 고전 아케이드 게임입니다." }); // 설명 구체화
    // 새로운 게임 추가 시 여기에 추가
    return games;
}

// 문자열을 소문자로 변환하는 함수
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// 챗봇 메뉴 및 로직
void chatbotMenu() {
    vector<Game> games = initializeGames();
    string userName = ""; // 사용자 이름 (선택적 기능)

    // 초기 인사 (선택적: 사용자 이름 묻기)
    // cout << "안녕하세요! 저는 게임 설명 로봇입니다. 성함이 어떻게 되시나요? ";
    // getline(cin, userName);
    // if (userName.empty()) userName = "게이머";

    while (true) {
        // 로봇 아스키 아트
        cout << "\n";
        cout << "      [ 미니게임 설명 챗봇 🤖 ]\n";
        cout << "        _______\n";
        cout << "       | .   . |   안녕하세요" /*<< (userName.empty() ? "" : ", " + userName + "님")*/ << "! 저는 게임 설명 로봇입니다!\n";
        cout << "       |   -   |   궁금한 게임의 이름이나 번호를 말씀해주세요.\n";
        cout << "       |___^___|\n";
        cout << "       |||||||||\n";
        cout << "       |||||||||\n";
        cout << "       |||||||||\n";
        cout << "      (__|_|__)\n";

        // 메뉴 출력
        cout << "\n====================================================\n";
        cout << "  설명 가능한 게임 목록 (종료: 'exit' 또는 '0')\n";
        cout << "----------------------------------------------------\n";
        for (size_t i = 0; i < games.size(); ++i) {
            cout << "  [" << games[i].id << "] " << games[i].name;
            if ((i + 1) % 2 == 0 || i == games.size() - 1) { // 한 줄에 두 개씩 또는 마지막 항목
                cout << "\n";
            }
            else {
                cout << "\t\t";
                if (games[i].name.length() < 8) cout << "\t"; // 이름 길이에 따른 탭 조절 (간단한 방식)
            }
        }
        cout << "====================================================\n";

        cout << "게임 이름 또는 번호 입력 >> ";
        string input;
        getline(cin, input);

        // 입력 공백 제거 (선택적)
        // input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());

        string lowerInput = toLower(input);

        if (lowerInput == "0" || lowerInput == "exit") {
            cout << "\n챗봇을 종료합니다. 즐거운 시간 되셨길 바랍니다! 메인 메뉴로 돌아갑니다.\n";
            break;
        }

        bool found = false;
        for (const auto& game : games) {
            // ID로 검색 (정확히 일치)
            if (lowerInput == game.id) {
                found = true;
            }
            // 이름 또는 키워드로 검색 (포함하는지 확인)
            else {
                if (toLower(game.name).find(lowerInput) != string::npos) {
                    found = true;
                }
                else {
                    for (const string& keyword : game.keywords) {
                        if (toLower(keyword).find(lowerInput) != string::npos) {
                            found = true;
                            break;
                        }
                    }
                }
            }

            if (found) {
                cout << "\n[ ✨ " << game.name << " 설명 ✨ ]\n";
                cout << game.description << "\n";
                break; // 게임을 찾았으면 루프 종료
            }
        }

        if (!found) {
            cout << "\n[ 😥 이런! ]\n";
            cout << "- 입력하신 '" << input << "'에 대한 정보를 찾을 수 없어요.\n";
            cout << "- 목록에 있는 게임 이름이나 번호를 다시 한번 확인해주세요!\n";
        }

        cout << "\n----------------------------------------------------\n";
        cout << "다른 궁금한 게임이 있으신가요? (계속하려면 아무 키나 누르세요)\n";
    }
}

