#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>

// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 生成关卡8题目（分数幂计算）
std::vector<Question> generateLevel8Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = FRACTION_POWER;
        
        // 生成分数底数
        int numerator = randomInt(1, 10);
        int denominator = randomInt(2, 10);
        
        // 确保分数是最简形式
        while (std::gcd(numerator, denominator) != 1) {
            numerator = randomInt(1, 10);
            denominator = randomInt(2, 10);
        }
        
        // 生成指数（2或3）
        int exponent = (randomInt(1, 10) <= 7) ? 2 : 3;
        
        std::stringstream ss;
        ss << "(" << numerator << "/" << denominator << ")^" << exponent;
        q.content = ss.str();
        
        // 计算答案
        long long num_result = 1;
        long long den_result = 1;
        
        for (int j = 0; j < exponent; j++) {
            num_result *= numerator;
            den_result *= denominator;
        }
        
        // 简化分数
        long long gcd_val = std::gcd(num_result, den_result);
        num_result /= gcd_val;
        den_result /= gcd_val;
        
        if (den_result == 1) {
            q.answer = std::to_string(num_result);
        } else {
            q.answer = std::to_string(num_result) + "/" + std::to_string(den_result);
        }
        
        // 生成解析
        std::stringstream expl_ss;
        expl_ss << "分子分母分别求幂: (" << numerator << "^" << exponent << ")/(";
        expl_ss << denominator << "^" << exponent << ") = ";
        expl_ss << num_result << "/" << den_result;
        
        if (gcd_val != 1) {
            expl_ss << " = " << q.answer << " (已化简)";
        }
        
        q.explanation = expl_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}