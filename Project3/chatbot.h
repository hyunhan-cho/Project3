#pragma once
#include <string>
#include <vector>

using namespace std;

// ���� ���� ����ü
struct Game {
    string name;
    string genre;
    string difficulty;
    string description;
    int popularity = 0; // ��� �ذ�: �⺻�� �ʱ�ȭ
};

// ê�� ���ؽ�Ʈ ����ü
struct UserProfile {
    int totalInteractions = 0; // ��� �ذ�: �⺻�� �ʱ�ȭ
};

struct ChatContext {
    bool isFirstTime = true;
    UserProfile userProfile;
};

// ê�� ���� �Լ� ����
void enhancedChatbotMenu();