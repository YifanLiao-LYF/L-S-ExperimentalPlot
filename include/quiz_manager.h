#ifndef QUIZ_MANAGER_H
#define QUIZ_MANAGER_H

#include <vector>
#include "question.h"

class QuizManager {
private:
    std::vector<Question> questionBank; // 题库
    int currentQuestionIndex;           // 当前题目索引
    
public:
    QuizManager();
    
    // 加载题库
    void loadQuestionBank();
    
    // 获取下一道题
    Question getNextQuestion();
    
    // 检查答案
    bool checkAnswer(const std::string& userAnswer);
    
    // 获取当前题目的解释
    std::string getExplanation();
    
    // 重置题库（从头开始）
    void reset();
};

#endif