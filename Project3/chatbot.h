#ifndef CHATBOT_H
#define CHATBOT_H

#include <string>
#include <vector>

struct Game {
    std::string id;
    std::string name;
    std::vector<std::string> keywords;
    std::string description;
    std::string difficulty;
    std::string genre;
    int popularity;
};

struct UserProfile {
    std::vector<std::string> preferredGenres;
    std::vector<std::string> playedGames;
    std::string skillLevel;
    int totalInteractions;
};

struct ChatContext {
    std::vector<std::string> conversationHistory;
    std::string lastTopic;
    UserProfile userProfile;
    bool isFirstTime;
};

std::vector<Game> initializeGames();
void printGameList(const std::vector<Game>& games);
void printGameDescription(const Game& game);
void chatbotMenu();
void enhancedChatbotMenu();
void printChatbotHeader();
std::string toLower(std::string s);
std::vector<std::string> tokenize(const std::string& text);
bool containsKeyword(const std::string& text, const std::vector<std::string>& keywords);
bool isGreeting(const std::string& input);
bool isQuestion(const std::string& input);
bool isDifficultyQuery(const std::string& input);
bool isRecommendationRequest(const std::string& input);
bool isGenreQuery(const std::string& input);
std::string generateGreeting(ChatContext& context);
std::vector<Game> getRecommendedGames(const std::vector<Game>& games, const ChatContext& context);
std::vector<Game> getGamesByDifficulty(const std::vector<Game>& games, const std::string& difficulty);
std::vector<Game> getGamesByGenre(const std::vector<Game>& games, const std::string& genre);
std::string generateContextualResponse(const std::string& input, const std::vector<Game>& games, ChatContext& context);
void printTypingEffect(const std::string& text, int delay);

#endif
