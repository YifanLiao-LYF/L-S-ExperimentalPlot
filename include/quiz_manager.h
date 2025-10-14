#ifndef QUIZ_MANAGER_H
#define QUIZ_MANAGER_H

#include <vector>
#include "question.h"

class QuizManager {
private:
    std::vector<Question> questionBank;
    int currentQuestionIndex;
    int currentLevelId;
    
    // 声明关卡题目生成函数
    std::vector<Question> generateLevel1Questions();
    std::vector<Question> generateLevel2Questions();
    std::vector<Question> generateLevel3Questions();
    std::vector<Question> generateLevel4Questions();
    std::vector<Question> generateLevel5Questions();
    std::vector<Question> generateLevel6Questions();
    
public:
    QuizManager();
    
    void loadQuestionsForLevel(int levelId);
    Question getNextQuestion();
    bool checkAnswer(const std::string& userAnswer);
    std::string getExplanation();
    void reset();
    
    int getCurrentLevelId() const;
};

#endif