#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>

#include "chatbot.h"
#include "utils.h"

using namespace std;

// --- [추가] 재미 요소를 위한 함수들 ---

// 로딩 애니메이션 출력
void print_thinking_animation() {
    cout << "  ";
    setColor(8); // 어두운 회색
    string animation[] = { "[ .    ]", "[ ..   ]", "[ ...  ]", "[ .... ]", "[ .....]" };
    for (int i = 0; i < 5; ++i) {
        cout << "\b\b\b\b\b\b\b\b\b" << animation[i] << flush;
        Sleep(100);
    }
    cout << "\b\b\b\b\b\b\b\b\b           \b\b\b\b\b\b\b\b\b\b\b"; // 애니메이션 지우기
}

// 챗봇 말풍선 (깨지지 않는 ASCII 버전)
void print_chatbot_bubble(const string& text, int delay = 30) {
    setColor(11); // 밝은 청록색
    cout << " +---[ AI Bot ]" << endl;
    cout << " | " << flush;

    setColor(15); // 밝은 흰색
    stringstream ss(text);
    string line;
    bool firstLine = true;
    while (getline(ss, line, '\n')) {
        if (!firstLine) {
            cout << " | " << flush;
        }
        for (char c : line) {
            cout << c << flush;
            Sleep(delay);
        }
        cout << endl;
        firstLine = false;
    }

    setColor(11);
    cout << " +-------------------" << endl;
    setColor(7);
}

// 유저 말풍선 (깨지지 않는 ASCII 버전)
void print_user_bubble(const string& text) {
    setColor(14); // 밝은 노란색
    cout << "                                       +---[ Player ]+" << endl;
    cout << "                                       | " << flush;
    setColor(15);
    cout << text << " |" << endl;
    setColor(14);
    cout << "                                       +------------+" << endl;
    setColor(7);
}

// 게임 데이터 초기화
vector<Game> initializeGames() {
    return {
        {"테트리스", "퍼즐", "초급", "블록이 쏟아지는 스릴! 전설의 퍼즐 게임, 테트리스는 어떠세요?", 95},
        {"지뢰찾기", "퍼즐", "초급", "두근두근 심장쫄깃! 지뢰를 피해 모든 안전한 칸을 여는 게임입니다.", 80},
        {"스네이크", "아케이드", "초급", "먹을수록 길어지는 즐거움! 뱀을 조종해 벽이나 자기 몸에 닿지 않게 하세요.", 85},
        {"스도쿠", "퍼즐", "고급", "뇌섹남녀를 위한 시간 순삭 논리 퍼즐! 9x9 격자를 숫자로 채워보세요.", 90},
        {"행맨", "교육", "초급", "단어를 맞춰 교수형에 처한 사람을 구해주세요! 영어 단어 공부에도 좋아요.", 75},
        {"우주선게임", "액션", "초급", "단순하지만 중독성 강한! 장애물을 피하며 우주를 누비는 파일럿이 되어보세요.", 88},
    };
}

// 문자열 소문자 변환
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

// 키워드 포함 확인
bool containsKeyword(const string& text, const vector<string>& keywords) {
    string lowerText = toLower(text);
    for (const auto& k : keywords)
        if (lowerText.find(toLower(k)) != string::npos)
            return true;
    return false;
}

// --- 인식 로직 ---
bool isGreeting(const string& input) { return containsKeyword(input, { "안녕", "hi", "hello", "반가", "처음" }); }
bool isRecommendationRequest(const string& input) { return containsKeyword(input, { "추천", "재밌는", "인기", "베스트" }); }
bool isDifficultyQuery(const string& input) { return containsKeyword(input, { "쉬운", "어려운", "난이도", "초급", "고급" }); }
bool isGenreQuery(const string& input) { return containsKeyword(input, { "퍼즐", "액션", "전략", "아케이드", "어드벤처", "교육" }); }

// --- 응답 생성 ---
string generateGreeting(ChatContext& context) {
    if (context.isFirstTime) {
        context.isFirstTime = false;
        vector<string> greetings = {
            "안녕하세요! 어떤 게임을 찾아오셨나요? '추천', '쉬운 게임', '퍼즐' 등으로 질문해보세요.",
            "삐리빗- 게임 데이터베이스 접속 완료! 궁금한 게임 정보를 물어보세요.",
            "오늘도 재밌는 게임을 찾아 오셨군요! 제가 최고의 게임을 추천해 드릴게요."
        };
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<size_t> dis(0, greetings.size() - 1); // 경고 해결: int -> size_t
        return greetings[dis(gen)];
    }
    return "다시 오셨군요! 이번엔 어떤 게임 정보가 궁금하세요?";
}

vector<Game> getRecommendedGames(const vector<Game>& games) {
    vector<Game> sorted = games;
    sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.popularity > b.popularity; });
    if (sorted.size() > 3) sorted.resize(3);
    return sorted;
}

vector<Game> getGamesByDifficulty(const vector<Game>& games, const string& difficulty) {
    vector<Game> out;
    string target = containsKeyword(difficulty, { "어려운", "고급" }) ? "고급" : "초급";
    for (const auto& g : games)
        if (toLower(g.difficulty) == toLower(target)) out.push_back(g);
    return out;
}

vector<Game> getGamesByGenre(const vector<Game>& games, const string& genre) {
    vector<Game> out;
    for (const auto& g : games)
        if (containsKeyword(g.genre, { genre })) out.push_back(g);
    return out;
}

string generateContextualResponse(const string& input, const vector<Game>& games, ChatContext& context) {
    if (isGreeting(input)) return generateGreeting(context);

    if (isRecommendationRequest(input)) {
        string res = "인기 폭발! 명예의 전당 게임 Top 3를 소개합니다!\n";
        int rank = 1;
        for (const auto& g : getRecommendedGames(games)) {
            res += to_string(rank++) + ". " + g.name + ": " + g.description + "\n";
        }
        return res;
    }

    if (isDifficultyQuery(input)) {
        string d = containsKeyword(input, { "어려운", "고급" }) ? "고급" : "초급";
        auto list = getGamesByDifficulty(games, d);
        string res = "요청하신 '" + d + "' 난이도 게임 목록입니다.\n";
        for (const auto& g : list) res += " - " + g.name + "\n";
        return list.empty() ? "앗, 해당 난이도의 게임 정보를 찾을 수 없어요." : res;
    }

    if (isGenreQuery(input)) {
        for (string genre : { "퍼즐", "액션", "전략", "아케이드", "어드벤처", "교육" }) {
            if (containsKeyword(input, { genre })) {
                auto list = getGamesByGenre(games, genre);
                string res = "장르가 '" + genre + "'인 게임 목록입니다!\n";
                for (auto& g : list) res += " - " + g.name + "\n";
                return list.empty() ? "이런, '" + genre + "' 장르의 게임이 없네요." : res;
            }
        }
    }

    return "음... 제 회로가 꼬였나 봐요. 잘 이해하지 못했어요.\n'추천', '쉬운 게임', '퍼즐 게임' 처럼 다시 질문해주실래요?";
}

// 메인 실행 루프
void enhancedChatbotMenu() {
    system("mode con: cols=100 lines=40");
    ChatContext context;
    vector<Game> games = initializeGames();

    clearScreen();
    setColor(11);
    cout << R"(
  ____ ____ ____ ____ ____ ____ _________ ____ ____ ____ 
 ||G |||A |||M |||E |||C |||H |||       |||B |||O |||T ||
 ||__|||__|||__|||__|||__|||__|||_______|||__|||__|||__||
 |/__\|/__\|/__\|/__\|/__\|/__\|/_______\|/__\|/__\|/__\|

'exit'를 입력하면 메인 메뉴로 돌아갑니다.
)";
    setColor(7);

    print_chatbot_bubble(generateGreeting(context), 20);

    while (true) {
        cout << endl;
        setColor(14);
        cout << "▶ 질문을 입력하세요: ";
        setColor(15);
        string input;
        getline(cin, input);

        if (toLower(input) == "exit") {
            print_chatbot_bubble("메인 메뉴로 돌아갈게요. 언제든지 다시 찾아주세요!", 20);
            Sleep(1000);
            break;
        }

        print_user_bubble(input);
        print_thinking_animation();

        string response = generateContextualResponse(input, games, context);
        print_chatbot_bubble(response, 20);
        context.userProfile.totalInteractions++;
    }
}