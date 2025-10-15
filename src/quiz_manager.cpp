#include "quiz_manager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <random>
#include <ctime>

// 构造函数
QuizManager::QuizManager() : currentQuestionIndex(0), currentLevelId(1) {
    loadQuestionsForLevel(1); // 默认加载第一关题目
}

// 加载特定关卡的题目
void QuizManager::loadQuestionsForLevel(int levelId) {
    currentLevelId = levelId;
    questionBank.clear();
    currentQuestionIndex = 0;
    // 直接调用各关卡的题目生成函数
    switch (levelId) {
        case 1: questionBank = generateLevel1Questions(); break;
        case 2: questionBank = generateLevel2Questions(); break;
        case 3: questionBank = generateLevel3Questions(); break;
        case 4: questionBank = generateLevel4Questions(); break;
        case 5: questionBank = generateLevel5Questions(); break;
        case 6: questionBank = generateLevel6Questions(); break;
        case 7: questionBank = generateLevel7Questions(); break;
        case 8: questionBank = generateLevel8Questions(); break;
        case 9: questionBank = generateLevel9Questions(); break;
        case 10: questionBank = generateLevel10Questions(); break;
        case 11: questionBank = generateLevel11Questions(); break;
        case 12: questionBank = generateLevel12Questions(); break;
        case 13: questionBank = generateLevel13Questions(); break;
        case 14: questionBank = generateLevel14Questions(); break;
        case 15: questionBank = generateLevel15Questions(); break;
        case 16: questionBank = generateLevel16Questions(); break;
        default: questionBank = generateLevel1Questions(); break;
    }
    // 如果生成的题目不足，补充备用题目
    if (questionBank.size() < 5) {
        std::vector<Question> fallbackQuestions = generateFallbackQuestions();
        questionBank.insert(questionBank.end(), fallbackQuestions.begin(), fallbackQuestions.end());
    }
}

// 加载每日挑战题目
void QuizManager::loadDailyChallenge(int challengeType) {
    currentLevelId = challengeType == 1 ? 13 : 16;
    questionBank.clear();
    currentQuestionIndex = 0;
    if (challengeType == 1) {
        // 关卡13每日挑战（如有特殊实现可补充）
        questionBank = generateLevel13Questions();
    } else {
        // 关卡16每日挑战
        questionBank = generateDailyChallengeLevel16();
    }
    if (questionBank.size() < 5) {
        std::vector<Question> fallbackQuestions = generateFallbackQuestions();
        questionBank.insert(questionBank.end(), fallbackQuestions.begin(), fallbackQuestions.end());
    }
}

// ...已移除文件IO题库加载函数...

// 生成备用题目（非常简单的题目，仅在文件加载失败时使用）
std::vector<Question> QuizManager::generateFallbackQuestions() {
    std::vector<Question> questions;
    
    // 添加一些非常基础的题目作为备用
    for (int i = 1; i <= 5; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        int a = randomInt(1, 10);
        int b = randomInt(1, 10);
        
        q.content = std::to_string(a) + " + " + std::to_string(b);
        q.answer = std::to_string(a + b);
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        
        questions.push_back(q);
    }
    
    return questions;
}

// 获取下一道题目
Question QuizManager::getNextQuestion() {
    if (currentQuestionIndex >= questionBank.size()) {
        return Question{-1, ARITHMETIC, "所有题目已完成!", "", ""};
    }
    
    return questionBank[currentQuestionIndex++];
}

// 检查答案是否正确
bool QuizManager::checkAnswer(const std::string& userAnswer) {
    if (currentQuestionIndex == 0) return false;
    
    Question current = questionBank[currentQuestionIndex - 1];
    return userAnswer == current.answer;
}

// 获取当前题目的解析
std::string QuizManager::getExplanation() {
    if (currentQuestionIndex == 0) return "";
    
    return questionBank[currentQuestionIndex - 1].explanation;
}

// 重置题目索引
void QuizManager::reset() {
    currentQuestionIndex = 0;
}

// 获取当前关卡ID
int QuizManager::getCurrentLevelId() const {
    return currentLevelId;
}

// 生成随机整数
int QuizManager::randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}