#ifndef QUIZ_MANAGER_H
#define QUIZ_MANAGER_H

#include <vector>
#include "question.h"

class QuizManager {
private:
    std::vector<Question> questionBank;
    int currentQuestionIndex;
    int currentLevelId;  // 当前题目所属关卡
    
public:
    QuizManager();
    
    // 关卡题目管理
    void loadQuestionsForLevel(int levelId);
    Question getNextQuestion();
    bool checkAnswer(const std::string& userAnswer);
    std::string getExplanation();
    void reset();
    
    int getCurrentLevelId() const { return currentLevelId; }
};

#endif