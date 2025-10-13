#include "mistake_manager.h"

void MistakeManager::addMistake(const Question& question, const std::string& userAnswer) {
    mistakes.push_back(question);
}

std::vector<Question> MistakeManager::getMistakes() {
    return mistakes;
}

void MistakeManager::clear() {
    mistakes.clear();
}