#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

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

// 通用随机数工具函数声明
int randomInt(int min, int max);
double randomDecimal1(int min, int max);
double randomDecimal2(int min, int max);

// 关卡题目生成函数声明
std::vector<Question> generateLevel1Questions();
std::vector<Question> generateLevel2Questions();
std::vector<Question> generateLevel3Questions();
std::vector<Question> generateLevel4Questions();
std::vector<Question> generateLevel5Questions();
std::vector<Question> generateLevel6Questions();
std::vector<Question> generateLevel7Questions();
std::vector<Question> generateLevel8Questions();
std::vector<Question> generateLevel9Questions();
std::vector<Question> generateLevel10Questions();
std::vector<Question> generateLevel11Questions();
std::vector<Question> generateLevel12Questions();
std::vector<Question> generateLevel13Questions();
std::vector<Question> generateLevel14Questions();
std::vector<Question> generateLevel15Questions();
std::vector<Question> generateLevel16Questions();
std::vector<Question> generateDailyChallengeLevel16();

#endif // QUESTION_H