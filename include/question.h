#ifndef QUESTION_H
#define QUESTION_H

#include <string>

enum QuestionType {
    ARITHMETIC,
    ALGEBRA,
    FACTORIZATION
};

struct Question {
    int id;
    QuestionType type;
    std::string content;
    std::string answer;
    std::string explanation;
};

#endif