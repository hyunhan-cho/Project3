#pragma once
#include <string>
#include <vector>

using namespace std;

// 게임 정보 구조체
struct Game {
    string name;
    string genre;
    string difficulty;
    string description;
    int popularity = 0; // 경고 해결: 기본값 초기화
};

// 챗봇 컨텍스트 구조체
struct UserProfile {
    int totalInteractions = 0; // 경고 해결: 기본값 초기화
};

struct ChatContext {
    bool isFirstTime = true;
    UserProfile userProfile;
};

// 챗봇 메인 함수 선언
void enhancedChatbotMenu();