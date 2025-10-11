#ifndef QUESTION_H
#define QUESTION_H

#include <string>

// 题目类型枚举
enum QuestionType {
    ARITHMETIC,     // 算术运算
    ALGEBRA,        // 代数方程
    FACTORIZATION   // 因式分解
};

// 题目结构体
struct Question {
    int id;                 // 题目ID
    QuestionType type;      // 题目类型
    std::string content;    // 题目内容
    std::string answer;     // 标准答案
    std::string explanation;// 解题解释
};

#endif