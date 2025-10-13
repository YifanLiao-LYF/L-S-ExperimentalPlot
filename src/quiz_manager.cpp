#include "quiz_manager.h"
#include <iostream>

QuizManager::QuizManager() : currentQuestionIndex(0), currentLevelId(1) {
    loadQuestionsForLevel(1); // 默认加载第一关题目
}

void QuizManager::loadQuestionsForLevel(int levelId) {
    currentLevelId = levelId;
    questionBank.clear();
    currentQuestionIndex = 0;
    
    // 根据关卡ID加载不同题目（题目内容留空）
    switch (levelId) {
        case 1:
            // 关卡1题目：有理数运算
            questionBank = {
                {1, ARITHMETIC, "题目1内容（有理数运算）", "答案1", "解析1"},
                {2, ARITHMETIC, "题目2内容（有理数运算）", "答案2", "解析2"},
                {3, ARITHMETIC, "题目3内容（有理数运算）", "答案3", "解析3"},
                {4, ARITHMETIC, "题目4内容（有理数运算）", "答案4", "解析4"},
                {5, ARITHMETIC, "题目5内容（有理数运算）", "答案5", "解析5"}
            };
            break;
        case 2:
            // 关卡2题目：一元一次方程
            questionBank = {
                {1, ALGEBRA, "题目1内容（一元一次方程）", "答案1", "解析1"},
                {2, ALGEBRA, "题目2内容（一元一次方程）", "答案2", "解析2"},
                {3, ALGEBRA, "题目3内容（一元一次方程）", "答案3", "解析3"},
                {4, ALGEBRA, "题目4内容（一元一次方程）", "答案4", "解析4"},
                {5, ALGEBRA, "题目5内容（一元一次方程）", "答案5", "解析5"}
            };
            break;
        case 3:
            // 关卡3题目：因式分解
            questionBank = {
                {1, FACTORIZATION, "题目1内容（因式分解）", "答案1", "解析1"},
                {2, FACTORIZATION, "题目2内容（因式分解）", "答案2", "解析2"},
                {3, FACTORIZATION, "题目3内容（因式分解）", "答案3", "解析3"},
                {4, FACTORIZATION, "题目4内容（因式分解）", "答案4", "解析4"},
                {5, FACTORIZATION, "题目5内容（因式分解）", "答案5", "解析5"}
            };
            break;
        case 4:
            // 关卡4题目：综合挑战
            questionBank = {
                {1, ARITHMETIC, "题目1内容（综合挑战）", "答案1", "解析1"},
                {2, ALGEBRA, "题目2内容（综合挑战）", "答案2", "解析2"},
                {3, FACTORIZATION, "题目3内容（综合挑战）", "答案3", "解析3"},
                {4, ARITHMETIC, "题目4内容（综合挑战）", "答案4", "解析4"},
                {5, ALGEBRA, "题目5内容（综合挑战）", "答案5", "解析5"}
            };
            break;
        default:
            break;
    }
}

Question QuizManager::getNextQuestion() {
    if (currentQuestionIndex >= questionBank.size()) {
        return Question{-1, ARITHMETIC, "所有题目已完成!", "", ""};
    }
    
    return questionBank[currentQuestionIndex++];
}

bool QuizManager::checkAnswer(const std::string& userAnswer) {
    if (currentQuestionIndex == 0) return false;
    
    Question current = questionBank[currentQuestionIndex - 1];
    return userAnswer == current.answer;
}

std::string QuizManager::getExplanation() {
    if (currentQuestionIndex == 0) return "";
    
    return questionBank[currentQuestionIndex - 1].explanation;
}

void QuizManager::reset() {
    currentQuestionIndex = 0;
}