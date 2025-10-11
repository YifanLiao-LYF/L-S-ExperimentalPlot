#ifndef MISTAKE_MANAGER_H
#define MISTAKE_MANAGER_H

#include <vector>
#include <string>
#include "question.h"

class MistakeManager {
private:
    std::vector<Question> mistakes; // 错题集
    
public:
    // 添加错题
    void addMistake(const Question& question, const std::string& userAnswer);
    
    // 获取所有错题
    std::vector<Question> getMistakes();
    
    // 清空错题集
    void clear();
};

#endif