#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>

// 生成随机整数
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成关卡1题目（十以内加减乘除）
std::vector<Question> generateLevel1Questions() {
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