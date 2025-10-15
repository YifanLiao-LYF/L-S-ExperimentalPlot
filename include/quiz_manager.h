#ifndef QUIZ_MANAGER_H
#define QUIZ_MANAGER_H

#include "question.h"
#include <vector>
#include <string>

class QuizManager {
private:
    std::vector<Question> questionBank;
    int currentQuestionIndex;
    int currentLevelId;
    
    // 从文件加载题目
    std::vector<Question> loadQuestionsFromFile(const std::string& filename);
    
    // 生成备用题目（仅在文件加载失败时使用）
    std::vector<Question> generateFallbackQuestions();
    
    // 生成随机整数
    int randomInt(int min, int max);

public:
    QuizManager();
    void loadQuestionsForLevel(int levelId);
    void loadDailyChallenge(int challengeType); // 1: 关卡13每日挑战, 2: 关卡16每日挑战
    Question getNextQuestion();
    bool checkAnswer(const std::string& userAnswer);
    std::string getExplanation();
    void reset();
    int getCurrentLevelId() const;
};

#endif // QUIZ_MANAGER_H