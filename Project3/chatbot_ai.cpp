// ======= chatbot_ai.cpp =======
#include "chatbot.h"
#include "utils.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <conio.h>
#include <Windows.h>

using namespace std;

// 문자열 유틸
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

vector<string> tokenize(const string& text) {
    vector<string> tokens;
    istringstream iss(text);
    string token;
    while (iss >> token) tokens.push_back(toLower(token));
    return tokens;
}

bool containsKeyword(const string& text, const vector<string>& keywords) {
    string lowerText = toLower(text);
    for (const auto& k : keywords)
        if (lowerText.find(toLower(k)) != string::npos)
            return true;
    return false;
}

// 인식 로직
bool isGreeting(const string& input) {
    return containsKeyword(input, { "안녕", "hi", "hello", "반가", "처음" });
}

bool isQuestion(const string& input) {
    return containsKeyword(input, { "뭐", "무엇", "어떻게", "왜", "언제", "?" });
}

bool isDifficultyQuery(const string& input) {
    return containsKeyword(input, { "쉬운", "어려운", "난이도", "초급", "고급" });
}

bool isRecommendationRequest(const string& input) {
    return containsKeyword(input, { "추천", "재밌는", "인기", "베스트" });
}

bool isGenreQuery(const string& input) {
    return containsKeyword(input, { "퍼즐", "액션", "전략", "아케이드", "어드벤처", "교육" });
}

// 응답 생성
string generateGreeting(ChatContext& context) {
    if (context.isFirstTime) {
        context.isFirstTime = false;
        return "안녕하세요! 어떤 게임을 도와드릴까요?";
    }
    return "다시 오셨군요! 게임에 대해 물어보세요.";
}

vector<Game> getRecommendedGames(const vector<Game>& games, const ChatContext&) {
    vector<Game> sorted = games;
    sort(sorted.begin(), sorted.end(), [](auto& a, auto& b) {
        return a.popularity > b.popularity;
        });
    if (sorted.size() > 3) sorted.resize(3);
    return sorted;
}

vector<Game> getGamesByDifficulty(const vector<Game>& games, const string& difficulty) {
    vector<Game> out;
    for (const auto& g : games)
        if (toLower(g.difficulty) == toLower(difficulty))
            out.push_back(g);
    return out;
}

vector<Game> getGamesByGenre(const vector<Game>& games, const string& genre) {
    vector<Game> out;
    for (const auto& g : games)
        if (toLower(g.genre) == toLower(genre))
            out.push_back(g);
    return out;
}

string generateContextualResponse(const string& input, const vector<Game>& games, ChatContext& context) {
    if (isGreeting(input)) return generateGreeting(context);

    if (isRecommendationRequest(input)) {
        string res = "추천 게임:\n";
        for (auto& g : getRecommendedGames(games, context))
            res += "- " + g.name + ": " + g.description + "\n";
        return res;
    }

    if (isDifficultyQuery(input)) {
        string d = containsKeyword(input, { "어려운", "고급" }) ? "고급" : "초급";
        auto list = getGamesByDifficulty(games, d);
        string res = d + " 난이도 게임:\n";
        for (auto& g : list)
            res += "- " + g.name + "\n";
        return res;
    }

    if (isGenreQuery(input)) {
        for (string genre : { "퍼즐", "액션", "전략", "아케이드", "어드벤처", "교육" }) {
            if (containsKeyword(input, { genre })) {
                auto list = getGamesByGenre(games, genre);
                string res = genre + " 게임:\n";
                for (auto& g : list)
                    res += "- " + g.name + "\n";
                return res;
            }
        }
    }

    return "죄송합니다. 관련된 게임 정보를 찾을 수 없습니다.";
}

// 출력 관련
void printChatbotHeader() {
    clearScreen();
    setColor(11);
    cout << "\n=== 게임 챗봇 ===\n";
    setColor(7);
}

void printTypingEffect(const string& text, int delay) {
    for (char c : text) {
        cout << c << flush;
        Sleep(delay);
    }
    cout << endl;
}

// 메인 실행 루프
void enhancedChatbotMenu() {
    ChatContext context;
    context.isFirstTime = true;
    context.userProfile.totalInteractions = 0;
    vector<Game> games = initializeGames(); 

    printChatbotHeader();
    printTypingEffect(generateGreeting(context), 30);

    while (true) {
        cout << "\n▶ 질문: ";
        string input;
        getline(cin, input);
        if (toLower(input) == "exit") break;

        string response = generateContextualResponse(input, games, context);
        printTypingEffect(response, 20);
        context.userProfile.totalInteractions++;
    }
}
