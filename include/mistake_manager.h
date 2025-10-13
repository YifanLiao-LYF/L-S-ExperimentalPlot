#ifndef MISTAKE_MANAGER_H
#define MISTAKE_MANAGER_H

#include <vector>
#include "question.h"

class MistakeManager {
private:
    std::vector<Question> mistakes;
    
public:
    void addMistake(const Question& question, const std::string& userAnswer);
    std::vector<Question> getMistakes();
    void clear();
};

#endif