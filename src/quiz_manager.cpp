#include "quiz_manager.h"
#include <fstream>
#include <iostream>
#include <random>

QuizManager::QuizManager() : currentQuestionIndex(0) {
    loadQuestionBank();
}

void QuizManager::loadQuestionBank() {
    // 这里先使用硬编码的题目，后续可以改为从文件读取
    questionBank = {
        {1, ARITHMETIC, "计算: 3 + 5 × 2", "13", "先乘除后加减: 5×2=10, 然后3+10=13"},
        {2, ARITHMETIC, "计算: (4 + 6) ÷ 2", "5", "先计算括号内: 4+6=10, 然后10÷2=5"},
        {3, ALGEBRA, "解方程: 2x + 3 = 11", "4", "2x = 11 - 3 => 2x = 8 => x = 4"},
        {4, ALGEBRA, "解方程: 3(x - 2) = 12", "6", "3x - 6 = 12 => 3x = 18 => x = 6"},
        {5, FACTORIZATION, "因式分解: x² - 4", "(x+2)(x-2)", "平方差公式: a² - b² = (a+b)(a-b)"}
    };
}

Question QuizManager::getNextQuestion() {
    if (currentQuestionIndex >= questionBank.size()) {
        // 所有题目已做完，可以重置或处理结束情况
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