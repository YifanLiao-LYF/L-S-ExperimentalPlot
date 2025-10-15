#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>

// 生成随机整数
// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 生成关卡3题目（多项式运算）
std::vector<Question> generateLevel3Questions() {
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