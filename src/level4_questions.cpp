#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 生成关卡4题目（简单：十以内小数运算）
std::vector<Question> generateLevel4Questions() {
    std::vector<Question> questions;
    std::vector<std::string> operators = {"+", "-", "×", "÷"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        // 随机选择1位或2位小数
        bool useOneDecimal = (rand() % 2 == 0);
        double num1, num2;
        
        if (useOneDecimal) {
            num1 = randomDecimal1(1, 10);
            num2 = randomDecimal1(1, 10);
        } else {
            num1 = randomDecimal2(1, 10);
            num2 = randomDecimal2(1, 10);
        }
        
        std::string op = operators[rand() % 4];
        
        // 确保减法结果不为负数
        if (op == "-" && num1 < num2) {
            std::swap(num1, num2);
        }
        
        // 确保除法能整除或结果为有限小数
        if (op == "÷") {
            num2 = randomDecimal1(1, 5);
            num1 = num2 * (rand() % 9 + 1);
            if (useOneDecimal) {
                num1 = std::round(num1 * 10) / 10;
            } else {
                num1 = std::round(num1 * 100) / 100;
            }
        }
        
        std::stringstream ss;
        ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2);
        ss << num1 << " " << op << " " << num2;
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
        ans_ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2) << result;
        q.answer = ans_ss.str();
        
        q.explanation = "直接计算: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    
    return questions;
}