#include "mistake_manager.h"

void MistakeManager::addMistake(const Question& question, const std::string& userAnswer) {
    // 可以在这里添加逻辑，避免重复添加相同的错题
    mistakes.push_back(question);
}

std::vector<Question> MistakeManager::getMistakes() {
    return mistakes;
}

void MistakeManager::clear() {
    mistakes.clear();
}