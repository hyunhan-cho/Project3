#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

void chatbotMenu() {
    while (true) {
        // 로봇 아스키 아트
        cout << "\n";
        cout << "     [ 미니게임 설명 챗봇 🤖 ]\n";
        cout << "        _______\n";
        cout << "       | .   . |    안녕하세요, 저는 게임 설명 로봇입니다!\n";
        cout << "       |   -   |    궁금한 게임 이름을 입력해주세요.\n";
        cout << "       |___^___|\n";
        cout << "       |||||||||\n";
        cout << "       |||||||||\n";
        cout << "       |||||||||\n";
        cout << "      (__|_|__)\n";

        // 메뉴 출력
        cout << "\n==================================================\n";
        cout << "설명 가능한 게임 목록 (종료하려면 'exit' 또는 '0')\n";
        cout << "--------------------------------------------------\n";
        cout << "  [1] 테트리스\t\t[2] 지뢰찾기\n";
        cout << "  [3] 미로찾기\t\t[4] 스네이크\n";
        cout << "  [5] 스도쿠\t\t[6] 행맨\n";
        cout << "  [7] 묵찌빠\t\t[8] 슈팅 게임\n";
        cout << "  [9] 풍 게임\n";
        cout << "==================================================\n";

        cout << "게임 이름 또는 번호 입력 >> ";
        string input;
        getline(cin, input);

        // 입력 처리
        transform(input.begin(), input.end(), input.begin(), ::tolower);
        if (input == "0" || input == "exit") {
            cout << "\n챗봇을 종료합니다. 메인 메뉴로 돌아갑니다.\n";
            break;
        }

        cout << "\n[게임 설명]\n";

        if (input == "1" || input.find("테트리스") != string::npos) {
            cout << "- 블록을 회전하며 줄을 맞춰서 없애는 퍼즐 게임입니다.\n";
        }
        else if (input == "2" || input.find("지뢰") != string::npos) {
            cout << "- 지뢰가 없는 칸을 열고, 숫자로 주변 지뢰 개수를 추리합니다.\n";
        }
        else if (input == "3" || input.find("미로") != string::npos) {
            cout << "- P에서 E까지 도착하는 길을 찾는 게임입니다.\n";
            cout << "  W A S D 키로 이동하며, Z 키로 한 칸 뒤로 돌아갈 수 있습니다.\n";
        }
        else if (input == "4" || input.find("스네이크") != string::npos) {
            cout << "- 뱀을 조작해 먹이를 먹고 점점 길어지는 고전 게임입니다.\n";
        }
        else if (input == "5" || input.find("스도쿠") != string::npos) {
            cout << "- 가로, 세로, 3x3 구역에 1~9 숫자를 겹치지 않게 채우는 퍼즐 게임입니다.\n";
        }
        else if (input == "6" || input.find("행맨") != string::npos) {
            cout << "- 단어를 맞추는 게임입니다. 틀릴수록 행맨이 점점 완성됩니다.\n";
        }
        else if (input == "7" || input.find("묵찌빠") != string::npos) {
            cout << "- 가위바위보 후 방향을 맞춰야 이기는 반응 속도 게임입니다.\n";
        }
        else if (input == "8" || input.find("슈팅") != string::npos) {
            cout << "- 적을 피하면서 총알로 공격하는 아케이드 스타일 게임입니다.\n";
        }
        else if (input == "9" || input.find("풍") != string::npos) {
            cout << "- 바람이나 풍선을 이용한 간단한 컨트롤 미니게임입니다.\n";
        }
        else {
            cout << "- 목록에 없는 게임이거나 이름을 잘못 입력하셨습니다.\n";
        }

        cout << "\n--------------------------------------------------\n";
    }
}
