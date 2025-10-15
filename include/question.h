#ifndef QUESTION_H
#define QUESTION_H

#include <string>

enum QuestionType {
    ARITHMETIC,          // 基础算术
    POWER,               // 幂函数
    FRACTION_POWER,      // 分数幂
    ROOT,                // 平方根
    TRIGONOMETRY,        // 三角函数
    CONSTANT_CALCULATION, // 常数计算（π、e）
    LOG_FACTORIAL        // 对数和阶乘
};

struct Question {
    int id;
    QuestionType type;
    std::string content;
    std::string answer;
    std::string explanation;
};

#endif // QUESTION_H