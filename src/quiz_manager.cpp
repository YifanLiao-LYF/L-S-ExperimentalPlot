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
    
    // 根据关卡ID从外部文件加载题目
    std::string filename;
    switch (levelId) {
        case 1: filename = "level1_questions.cpp"; break;
        case 2: filename = "level2_questions.cpp"; break;
        case 3: filename = "level3_questions.cpp"; break;
        case 4: filename = "level4_questions.cpp"; break;
        case 5: filename = "level5_questions.cpp"; break;
        case 6: filename = "level6_questions.cpp"; break;
        case 7: filename = "level7_questions.cpp"; break;
        case 8: filename = "level8_questions.cpp"; break;
        case 9: filename = "level9_questions.cpp"; break;
        case 10: filename = "level10_questions.cpp"; break;
        case 11: filename = "level11_questions.cpp"; break;
        case 12: filename = "level12_questions.cpp"; break;
        case 13: filename = "level13_questions.cpp"; break;
        case 14: filename = "level14_questions.cpp"; break;
        case 15: filename = "level15_questions.cpp"; break;
        case 16: filename = "level16_questions.cpp"; break;
        default: filename = "level1_questions.cpp"; break;
    }
    
    questionBank = loadQuestionsFromFile(filename);
    
    // 如果从文件加载的题目不足，补充备用题目
    if (questionBank.size() < 5) {
        std::vector<Question> fallbackQuestions = generateFallbackQuestions();
        questionBank.insert(questionBank.end(), fallbackQuestions.begin(), fallbackQuestions.end());
    }
}

// 加载每日挑战题目
void QuizManager::loadDailyChallenge(int challengeType) {
    currentLevelId = challengeType == 1 ? 13 : 16; // 1表示关卡13的每日挑战，2表示关卡16的每日挑战
    questionBank.clear();
    currentQuestionIndex = 0;
    
    std::string filename = challengeType == 1 ? "daily_challenge_13.cpp" : "daily_challenge_16.cpp";
    questionBank = loadQuestionsFromFile(filename);
    
    // 如果从文件加载的题目不足，补充备用题目
    if (questionBank.size() < 5) {
        std::vector<Question> fallbackQuestions = generateFallbackQuestions();
        questionBank.insert(questionBank.end(), fallbackQuestions.begin(), fallbackQuestions.end());
    }
}

// 从文件加载题目的函数
std::vector<Question> QuizManager::loadQuestionsFromFile(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "错误：无法打开文件 " << filename << std::endl;
        // 返回空列表，将由调用者处理
        return questions;
    }
    
    std::string line;
    Question currentQuestion;
    int questionId = 1;
    bool inQuestionBlock = false;
    
    while (std::getline(file, line)) {
        // 跳过空行和注释
        if (line.empty() || line.find("//") == 0) {
            continue;
        }
        
        // 检测题目开始标记
        if (line.find("QUESTION:") != std::string::npos) {
            if (inQuestionBlock) {
                // 保存当前题目
                questions.push_back(currentQuestion);
            }
            
            currentQuestion = Question();
            currentQuestion.id = questionId++;
            currentQuestion.type = ARITHMETIC;
            
            // 提取题目内容
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                currentQuestion.content = line.substr(pos + 1);
                // 去除前后空格
                currentQuestion.content.erase(0, currentQuestion.content.find_first_not_of(" \t"));
                currentQuestion.content.erase(currentQuestion.content.find_last_not_of(" \t") + 1);
            }
            inQuestionBlock = true;
        }
        // 检测答案标记
        else if (line.find("ANSWER:") != std::string::npos && inQuestionBlock) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                currentQuestion.answer = line.substr(pos + 1);
                // 去除前后空格
                currentQuestion.answer.erase(0, currentQuestion.answer.find_first_not_of(" \t"));
                currentQuestion.answer.erase(currentQuestion.answer.find_last_not_of(" \t") + 1);
            }
        }
        // 检测解析标记
        else if (line.find("EXPLANATION:") != std::string::npos && inQuestionBlock) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                currentQuestion.explanation = line.substr(pos + 1);
                // 去除前后空格
                currentQuestion.explanation.erase(0, currentQuestion.explanation.find_first_not_of(" \t"));
                currentQuestion.explanation.erase(currentQuestion.explanation.find_last_not_of(" \t") + 1);
            }
        }
    }
    
    // 添加最后一个题目
    if (inQuestionBlock && !currentQuestion.content.empty()) {
        questions.push_back(currentQuestion);
    }
    
    file.close();
    
    return questions;
}

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