#include "quiz_manager.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cmath>

// 构造函数
QuizManager::QuizManager() : currentQuestionIndex(0), currentLevelId(1) {
    loadQuestionsForLevel(1); // 默认加载第一关题目
}

// 加载特定关卡的题目
void QuizManager::loadQuestionsForLevel(int levelId) {
    currentLevelId = levelId;
    questionBank.clear();
    currentQuestionIndex = 0;
    
    // 根据关卡ID加载不同题目
    switch (levelId) {
        case 1:
            questionBank = generateLevel1Questions();
            break;
        case 2:
            questionBank = generateLevel2Questions();
            break;
        case 3:
            questionBank = generateLevel3Questions();
            break;
        case 4:
            questionBank = generateLevel4Questions();
            break;
        case 5:
            questionBank = generateLevel5Questions();
            break;
        case 6:
            questionBank = generateLevel6Questions();
            break;
        default:
            // 默认加载第一关题目
            questionBank = generateLevel1Questions();
            break;
    }
}

// 生成随机整数
int randomInt(int min, int max) {
    static std::mt19937 rng(time(0));
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// 关卡1题目生成（十以内加减乘除）
std::vector<Question> QuizManager::generateLevel1Questions() {
    std::vector<Question> questions;
    std::vector<std::string> operators = {"+", "-", "×", "÷"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        int num1 = randomInt(1, 10);
        int num2 = randomInt(1, 10);
        std::string op = operators[randomInt(0, 3)];
        
        // 确保减法结果不为负数
        if (op == "-" && num1 < num2) {
            std::swap(num1, num2);
        }
        
        // 确保除法能整除
        if (op == "÷") {
            num2 = randomInt(1, 10);
            num1 = num2 * randomInt(1, 10);
            if (num1 > 10) num1 = num2 * randomInt(1, 5); // 确保不超过10
        }
        
        std::stringstream ss;
        ss << num1 << " " << op << " " << num2;
        q.content = ss.str();
        
        // 计算答案
        if (op == "+") {
            q.answer = std::to_string(num1 + num2);
        } else if (op == "-") {
            q.answer = std::to_string(num1 - num2);
        } else if (op == "×") {
            q.answer = std::to_string(num1 * num2);
        } else if (op == "÷") {
            q.answer = std::to_string(num1 / num2);
        }
        
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    
    return questions;
}

// 关卡2题目生成（百以内加减乘除）
std::vector<Question> QuizManager::generateLevel2Questions() {
    std::vector<Question> questions;
    std::vector<std::string> operators = {"+", "-", "×", "÷"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        int num1 = randomInt(10, 100);
        int num2 = randomInt(1, 100);
        std::string op = operators[randomInt(0, 3)];
        
        // 确保减法结果不为负数
        if (op == "-" && num1 < num2) {
            std::swap(num1, num2);
        }
        
        // 确保除法能整除
        if (op == "÷") {
            num2 = randomInt(1, 20);
            num1 = num2 * randomInt(1, 10);
            if (num1 > 100) num1 = num2 * randomInt(1, 5); // 确保不超过100
        }
        
        std::stringstream ss;
        ss << num1 << " " << op << " " << num2;
        q.content = ss.str();
        
        // 计算答案
        if (op == "+") {
            q.answer = std::to_string(num1 + num2);
        } else if (op == "-") {
            q.answer = std::to_string(num1 - num2);
        } else if (op == "×") {
            q.answer = std::to_string(num1 * num2);
        } else if (op == "÷") {
            q.answer = std::to_string(num1 / num2);
        }
        
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    
    return questions;
}

// 关卡3题目生成（多项式运算）
std::vector<Question> QuizManager::generateLevel3Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        int num1 = randomInt(1, 20);
        int num2 = randomInt(1, 20);
        int num3 = randomInt(1, 20);
        
        // 随机选择运算类型
        int pattern = randomInt(1, 4);
        std::stringstream ss;
        
        switch (pattern) {
            case 1: // a + b × c
                ss << num1 << " + " << num2 << " × " << num3;
                q.content = ss.str();
                q.answer = std::to_string(num1 + num2 * num3);
                q.explanation = "先乘除后加减: " + std::to_string(num2) + " × " + 
                               std::to_string(num3) + " = " + std::to_string(num2 * num3) + 
                               ", 然后 " + std::to_string(num1) + " + " + 
                               std::to_string(num2 * num3) + " = " + q.answer;
                break;
                
            case 2: // a × b + c
                ss << num1 << " × " << num2 << " + " << num3;
                q.content = ss.str();
                q.answer = std::to_string(num1 * num2 + num3);
                q.explanation = "先乘除后加减: " + std::to_string(num1) + " × " + 
                               std::to_string(num2) + " = " + std::to_string(num1 * num2) + 
                               ", 然后 " + std::to_string(num1 * num2) + " + " + 
                               std::to_string(num3) + " = " + q.answer;
                break;
                
            case 3: // (a + b) × c
                ss << "(" << num1 << " + " << num2 << ") × " << num3;
                q.content = ss.str();
                q.answer = std::to_string((num1 + num2) * num3);
                q.explanation = "先计算括号内: " + std::to_string(num1) + " + " + 
                               std::to_string(num2) + " = " + std::to_string(num1 + num2) + 
                               ", 然后 " + std::to_string(num1 + num2) + " × " + 
                               std::to_string(num3) + " = " + q.answer;
                break;
                
            case 4: // a × (b + c)
                ss << num1 << " × (" << num2 << " + " << num3 << ")";
                q.content = ss.str();
                q.answer = std::to_string(num1 * (num2 + num3));
                q.explanation = "先计算括号内: " + std::to_string(num2) + " + " + 
                               std::to_string(num3) + " = " + std::to_string(num2 + num3) + 
                               ", 然后 " + std::to_string(num1) + " × " + 
                               std::to_string(num2 + num3) + " = " + q.answer;
                break;
        }
        
        questions.push_back(q);
    }
    
    return questions;
}

// 关卡4题目生成（十以内小数运算）
std::vector<Question> QuizManager::generateLevel4Questions() {
    std::vector<Question> questions;
    std::vector<std::string> operators = {"+", "-", "×", "÷"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        // 生成1位小数
        double num1 = randomInt(1, 100) / 10.0;
        double num2 = randomInt(1, 100) / 10.0;
        std::string op = operators[randomInt(0, 3)];
        
        // 确保减法结果不为负数
        if (op == "-" && num1 < num2) {
            std::swap(num1, num2);
        }
        
        // 确保除法能整除
        if (op == "÷") {
            num2 = randomInt(1, 20) / 10.0;
            num1 = num2 * randomInt(1, 10);
        }
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << num1 << " " << op << " " << num2;
        q.content = ss.str();
        
        // 计算答案
        double result;
        if (op == "+") {
            result = num1 + num2;
        } else if (op == "-") {
            result = num1 - num2;
        } else if (op == "×") {
            result = num1 * num2;
        } else if (op == "÷") {
            result = num1 / num2;
        }
        
        // 格式化答案
        std::stringstream ans_ss;
        ans_ss << std::fixed << std::setprecision(1) << result;
        q.answer = ans_ss.str();
        
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    
    return questions;
}

// 关卡5题目生成（百以内小数运算）
std::vector<Question> QuizManager::generateLevel5Questions() {
    std::vector<Question> questions;
    std::vector<std::string> operators = {"+", "-", "×", "÷"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        // 生成2位小数
        double num1 = randomInt(10, 1000) / 100.0;
        double num2 = randomInt(1, 1000) / 100.0;
        std::string op = operators[randomInt(0, 3)];
        
        // 确保减法结果不为负数
        if (op == "-" && num1 < num2) {
            std::swap(num1, num2);
        }
        
        // 确保除法能整除
        if (op == "÷") {
            num2 = randomInt(1, 200) / 100.0;
            num1 = num2 * randomInt(1, 10);
        }
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << num1 << " " << op << " " << num2;
        q.content = ss.str();
        
        // 计算答案
        double result;
        if (op == "+") {
            result = num1 + num2;
        } else if (op == "-") {
            result = num1 - num2;
        } else if (op == "×") {
            result = num1 * num2;
        } else if (op == "÷") {
            result = num1 / num2;
        }
        
        // 格式化答案
        std::stringstream ans_ss;
        ans_ss << std::fixed << std::setprecision(2) << result;
        q.answer = ans_ss.str();
        
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    
    return questions;
}

// 关卡6题目生成（复杂小数运算）
std::vector<Question> QuizManager::generateLevel6Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        // 生成2位小数
        double num1 = randomInt(1, 200) / 100.0;
        double num2 = randomInt(1, 200) / 100.0;
        double num3 = randomInt(1, 200) / 100.0;
        
        // 随机选择运算类型
        int pattern = randomInt(1, 4);
        std::stringstream ss;
        double result;
        
        switch (pattern) {
            case 1: // a + b × c
                ss << std::fixed << std::setprecision(2) << num1 << " + " << num2 << " × " << num3;
                q.content = ss.str();
                result = num1 + num2 * num3;
                q.explanation = "先乘除后加减: " + std::to_string(num2) + " × " + 
                               std::to_string(num3) + " = " + std::to_string(num2 * num3) + 
                               ", 然后 " + std::to_string(num1) + " + " + 
                               std::to_string(num2 * num3) + " = " + std::to_string(result);
                break;
                
            case 2: // a × b + c
                ss << std::fixed << std::setprecision(2) << num1 << " × " << num2 << " + " << num3;
                q.content = ss.str();
                result = num1 * num2 + num3;
                q.explanation = "先乘除后加减: " + std::to_string(num1) + " × " + 
                               std::to_string(num2) + " = " + std::to_string(num1 * num2) + 
                               ", 然后 " + std::to_string(num1 * num2) + " + " + 
                               std::to_string(num3) + " = " + std::to_string(result);
                break;
                
            case 3: // (a + b) × c
                ss << std::fixed << std::setprecision(2) << "(" << num1 << " + " << num2 << ") × " << num3;
                q.content = ss.str();
                result = (num1 + num2) * num3;
                q.explanation = "先计算括号内: " + std::to_string(num1) + " + " + 
                               std::to_string(num2) + " = " + std::to_string(num1 + num2) + 
                               ", 然后 " + std::to_string(num1 + num2) + " × " + 
                               std::to_string(num3) + " = " + std::to_string(result);
                break;
                
            case 4: // a × (b + c)
                ss << std::fixed << std::setprecision(2) << num1 << " × (" << num2 << " + " << num3 << ")";
                q.content = ss.str();
                result = num1 * (num2 + num3);
                q.explanation = "先计算括号内: " + std::to_string(num2) + " + " + 
                               std::to_string(num3) + " = " + std::to_string(num2 + num3) + 
                               ", 然后 " + std::to_string(num1) + " × " + 
                               std::to_string(num2 + num3) + " = " + std::(result);
                break;
        }
        
        // 格式化答案
        std::stringstream ans_ss;
        ans_ss << std::fixed << std::setprecision(2) << result;
        q.answer = ans_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}

// 获取下一道题目
Question QuizManager::getNextQuestion() {
    if (currentQuestionIndex >= questionBank.size()) {
        return Question{-1, ARITHMETIC, "所有题目已完成!", "", ""};
    }
    
    return questionBank[currentQuestionIndex++];
}

// 检查答案是否正确
bool QuizManager::checkAnswer(const std::string& userAnswer) {
    if (currentQuestionIndex == 0) return false;
    
    Question current = questionBank[currentQuestionIndex - 1];
    return userAnswer == current.answer;
}

// 获取当前题目的解析
std::string QuizManager::getExplanation() {
    if (currentQuestionIndex == 0) return "";
    
    return questionBank[currentQuestionIndex - 1].explanation;
}

// 重置题目索引
void QuizManager::reset() {
    currentQuestionIndex = 0;
}

// 获取当前关卡ID
int QuizManager::getCurrentLevelId() const {
    return currentLevelId;
}