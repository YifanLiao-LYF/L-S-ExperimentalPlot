#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>

// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 生成关卡6题目（困难：多步复杂小数运算）
std::vector<Question> generateLevel6Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ARITHMETIC;
        
        // 随机选择1位或2位小数
        bool useOneDecimal = (rand() % 2 == 0);
        double num1, num2, num3;
        
        if (useOneDecimal) {
            num1 = randomDecimal1(1, 20);
            num2 = randomDecimal1(1, 20);
            num3 = randomDecimal1(1, 20);
        } else {
            num1 = randomDecimal2(1, 20);
            num2 = randomDecimal2(1, 20);
            num3 = randomDecimal2(1, 20);
        }
        
        // 随机选择运算类型
        int pattern = rand() % 4;
        std::stringstream ss;
        double result;
        
        switch (pattern) {
            case 0: // a + b × c
                ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2);
                ss << num1 << " + " << num2 << " × " << num3;
                q.content = ss.str();
                result = num1 + num2 * num3;
                q.explanation = "先乘除后加减: " + std::to_string(num2) + " × " + 
                               std::to_string(num3) + " = " + std::to_string(num2 * num3) + 
                               ", 然后 " + std::to_string(num1) + " + " + 
                               std::to_string(num2 * num3) + " = " + std::to_string(result);
                break;
                
            case 1: // a × b + c
                ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2);
                ss << num1 << " × " << num2 << " + " << num3;
                q.content = ss.str();
                result = num1 * num2 + num3;
                q.explanation = "先乘除后加减: " + std::to_string(num1) + " × " + 
                               std::to_string(num2) + " = " + std::to_string(num1 * num2) + 
                               ", 然后 " + std::to_string(num1 * num2) + " + " + 
                               std::to_string(num3) + " = " + std::to_string(result);
                break;
                
            case 2: // (a + b) × c
                ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2);
                ss << "(" << num1 << " + " << num2 << ") × " << num3;
                q.content = ss.str();
                result = (num1 + num2) * num3;
                q.explanation = "先计算括号内: " + std::to_string(num1) + " + " + 
                               std::to_string(num2) + " = " + std::to_string(num1 + num2) + 
                               ", 然后 " + std::to_string(num1 + num2) + " × " + 
                               std::to_string(num3) + " = " + std::to_string(result);
                break;
                
            case 3: // a × (b + c)
                ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2);
                ss << num1 << " × (" << num2 << " + " << num3 << ")";
                q.content = ss.str();
                result = num1 * (num2 + num3);
                q.explanation = "先计算括号内: " + std::to_string(num2) + " + " + 
                               std::to_string(num3) + " = " + std::to_string(num2 + num3) + 
                               ", 然后 " + std::to_string(num1) + " × " + 
                               std::to_string(num2 + num3) + " = " + std::to_string(result);
                break;
        }
        
        // 格式化答案
        std::stringstream ans_ss;
        ans_ss << std::fixed << std::setprecision(useOneDecimal ? 1 : 2) << result;
        q.answer = ans_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}