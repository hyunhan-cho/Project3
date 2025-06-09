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

// ���ڿ� ��ƿ
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

// �ν� ����
bool isGreeting(const string& input) {
    return containsKeyword(input, { "�ȳ�", "hi", "hello", "�ݰ�", "ó��" });
}

bool isQuestion(const string& input) {
    return containsKeyword(input, { "��", "����", "���", "��", "����", "?" });
}

bool isDifficultyQuery(const string& input) {
    return containsKeyword(input, { "����", "�����", "���̵�", "�ʱ�", "���" });
}

bool isRecommendationRequest(const string& input) {
    return containsKeyword(input, { "��õ", "��մ�", "�α�", "����Ʈ" });
}

bool isGenreQuery(const string& input) {
    return containsKeyword(input, { "����", "�׼�", "����", "�����̵�", "��庥ó", "����" });
}

// ���� ����
string generateGreeting(ChatContext& context) {
    if (context.isFirstTime) {
        context.isFirstTime = false;
        return "�ȳ��ϼ���! � ������ ���͵帱���?";
    }
    return "�ٽ� ���̱���! ���ӿ� ���� �������.";
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
        string res = "��õ ����:\n";
        for (auto& g : getRecommendedGames(games, context))
            res += "- " + g.name + ": " + g.description + "\n";
        return res;
    }

    if (isDifficultyQuery(input)) {
        string d = containsKeyword(input, { "�����", "���" }) ? "���" : "�ʱ�";
        auto list = getGamesByDifficulty(games, d);
        string res = d + " ���̵� ����:\n";
        for (auto& g : list)
            res += "- " + g.name + "\n";
        return res;
    }

    if (isGenreQuery(input)) {
        for (string genre : { "����", "�׼�", "����", "�����̵�", "��庥ó", "����" }) {
            if (containsKeyword(input, { genre })) {
                auto list = getGamesByGenre(games, genre);
                string res = genre + " ����:\n";
                for (auto& g : list)
                    res += "- " + g.name + "\n";
                return res;
            }
        }
    }

    return "�˼��մϴ�. ���õ� ���� ������ ã�� �� �����ϴ�.";
}

// ��� ����
void printChatbotHeader() {
    clearScreen();
    setColor(11);
    cout << "\n=== ���� ê�� ===\n";
    setColor(7);
}

void printTypingEffect(const string& text, int delay) {
    for (char c : text) {
        cout << c << flush;
        Sleep(delay);
    }
    cout << endl;
}

// ���� ���� ����
void enhancedChatbotMenu() {
    ChatContext context;
    context.isFirstTime = true;
    context.userProfile.totalInteractions = 0;
    vector<Game> games = initializeGames(); 

    printChatbotHeader();
    printTypingEffect(generateGreeting(context), 30);

    while (true) {
        cout << "\n�� ����: ";
        string input;
        getline(cin, input);
        if (toLower(input) == "exit") break;

        string response = generateContextualResponse(input, games, context);
        printTypingEffect(response, 20);
        context.userProfile.totalInteractions++;
    }
}
