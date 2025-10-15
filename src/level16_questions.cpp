#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>
#include <algorithm>
#include <ctime>

// 生成随机整数
// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 包含前面所有关卡的题目生成函数声明（不包括关卡十三）
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
std::vector<Question> generateLevel14Questions();
std::vector<Question> generateLevel15Questions();

// 生成关卡16题目（综合关卡二，随机抽取前面所有关卡的题目，不包括关卡十三）
std::vector<Question> generateLevel16Questions() {
    std::vector<Question> questions;
    
    // 收集前面所有关卡的题目（不包括关卡十三）
    std::vector<std::vector<Question>> allQuestions;
    allQuestions.push_back(generateLevel1Questions());   // 十以内加减乘除
    allQuestions.push_back(generateLevel2Questions());   // 百以内加减乘除
    allQuestions.push_back(generateLevel3Questions());   // 多项式运算
    allQuestions.push_back(generateLevel4Questions());   // 十以内小数运算
    allQuestions.push_back(generateLevel5Questions());   // 百以内小数运算
    allQuestions.push_back(generateLevel6Questions());   // 复杂小数运算
    allQuestions.push_back(generateLevel7Questions());   // 幂函数计算
    allQuestions.push_back(generateLevel8Questions());   // 分数幂计算
    allQuestions.push_back(generateLevel9Questions());   // 平方根计算
    allQuestions.push_back(generateLevel10Questions());  // 简单特殊三角函数值
    allQuestions.push_back(generateLevel11Questions());  // 简单三角方程及应用
    allQuestions.push_back(generateLevel12Questions());  // 复合三角恒等式与不等式
    allQuestions.push_back(generateLevel14Questions());  // π值、e值、勾股定理
    allQuestions.push_back(generateLevel15Questions());  // 对数、阶乘
    
    // 确定题目数量 (10-15题)
    int numQuestions = randomInt(10, 15);
    
    // 从所有题目中随机选择
    for (int i = 0; i < numQuestions; i++) {
        // 随机选择一个关卡
        int level = randomInt(0, allQuestions.size() - 1);
        
        // 确保该关卡有题目
        if (allQuestions[level].empty()) {
            i--; // 重试
            continue;
        }
        
        // 从该关卡中随机选择一道题目
        int questionIndex = randomInt(0, allQuestions[level].size() - 1);
        Question selectedQuestion = allQuestions[level][questionIndex];
        
        // 更新题目ID
        selectedQuestion.id = i + 1;
        
        // 添加到结果集
        questions.push_back(selectedQuestion);
        
        // 从原关卡中移除该题目，避免重复
        allQuestions[level].erase(allQuestions[level].begin() + questionIndex);
    }
    
    return questions;
}

// 每日挑战关卡函数（基于关卡十六）
std::vector<Question> generateDailyChallengeLevel16() {
    // 获取当前日期作为随机种子，确保每天题目相同
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int seed = (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;
    
    static std::mt19937 gen(seed);
    std::uniform_int_distribution<> distrib(0, 13); // 14个关卡（1-12,14-15）
    
    std::vector<Question> dailyQuestions;
    
    // 每天固定5道题，从不同关卡中随机选择
    for (int i = 0; i < 5; i++) {
        int level = distrib(gen);
        
        // 根据关卡号调用相应的生成函数
        std::vector<Question> levelQuestions;
        switch (level) {
            case 0: levelQuestions = generateLevel1Questions(); break;
            case 1: levelQuestions = generateLevel2Questions(); break;
            case 2: levelQuestions = generateLevel3Questions(); break;
            case 3: levelQuestions = generateLevel4Questions(); break;
            case 4: levelQuestions = generateLevel5Questions(); break;
            case 5: levelQuestions = generateLevel6Questions(); break;
            case 6: levelQuestions = generateLevel7Questions(); break;
            case 7: levelQuestions = generateLevel8Questions(); break;
            case 8: levelQuestions = generateLevel9Questions(); break;
            case 9: levelQuestions = generateLevel10Questions(); break;
            case 10: levelQuestions = generateLevel11Questions(); break;
            case 11: levelQuestions = generateLevel12Questions(); break;
            case 12: levelQuestions = generateLevel14Questions(); break;
            case 13: levelQuestions = generateLevel15Questions(); break;
        }
        
        // 从该关卡题目中随机选择一道
        if (!levelQuestions.empty()) {
            int questionIndex = distrib(gen) % levelQuestions.size();
            Question selectedQuestion = levelQuestions[questionIndex];
            selectedQuestion.id = i + 1;
            dailyQuestions.push_back(selectedQuestion);
        } else {
            i--; // 重试
        }
    }
    
    return dailyQuestions;
}

// 获取关卡类型名称
std::string getLevelTypeName(int level) {
    switch (level) {
        case 1: return "十以内加减乘除";
        case 2: return "百以内加减乘除";
        case 3: return "多项式运算";
        case 4: return "十以内小数运算";
        case 5: return "百以内小数运算";
        case 6: return "复杂小数运算";
        case 7: return "幂函数计算";
        case 8: return "分数幂计算";
        case 9: return "平方根计算";
        case 10: return "简单特殊三角函数值";
        case 11: return "简单三角方程及应用";
        case 12: return "复合三角恒等式与不等式";
        case 14: return "π值、e值、勾股定理";
        case 15: return "对数、阶乘";
        default: return "未知类型";
    }
}