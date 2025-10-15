#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>

// 生成随机整数
// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 常见对数值
std::map<int, int> commonLogs = {
    {1, 0}, {2, 1}, {4, 2}, {8, 3}, {16, 4}, {32, 5}, {64, 6},
    {10, 1}, {100, 2}, {1000, 3}, {10000, 4},
    {3, 1}, {9, 2}, {27, 3}, {81, 4}, {243, 5},
    {5, 1}, {25, 2}, {125, 3}, {625, 4}
};

// 常见阶乘值
std::map<int, long long> commonFactorials = {
    {0, 1}, {1, 1}, {2, 2}, {3, 6}, {4, 24}, {5, 120}, 
    {6, 720}, {7, 5040}, {8, 40320}, {9, 362880}, {10, 3628800}
};

// 生成关卡15题目（对数、阶乘）
std::vector<Question> generateLevel15Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = LOG_FACTORIAL;
        
        int pattern = randomInt(1, 2);
        std::stringstream ss;
        
        switch (pattern) {
            case 1: // 对数计算
            {
                int baseType = randomInt(1, 3); // 1: 以2为底, 2: 以10为底, 3: 以e为底
                
                if (baseType == 1 || baseType == 2) {
                    // 从常见对数值中选择
                    auto it = commonLogs.begin();
                    std::advance(it, randomInt(0, commonLogs.size() - 1));
                    
                    int value = it->first;
                    int result = it->second;
                    
                    if (baseType == 1) {
                        ss << "计算: log₂(" << value << ")";
                        q.content = ss.str();
                        q.answer = std::to_string(result);
                        q.explanation = "因为 2^" + std::to_string(result) + " = " + std::to_string(value);
                    } else {
                        ss << "计算: log₁₀(" << value << ")";
                        q.content = ss.str();
                        q.answer = std::to_string(result);
                        q.explanation = "因为 10^" + std::to_string(result) + " = " + std::to_string(value);
                    }
                } else {
                    // 自然对数
                    int value = randomInt(1, 5);
                    ss << "计算: ln(e^" << value << ")";
                    q.content = ss.str();
                    q.answer = std::to_string(value);
                    q.explanation = "ln(e^x) = x, 所以 ln(e^" + std::to_string(value) + ") = " + std::to_string(value);
                }
                break;
            }
                
            case 2: // 阶乘计算
            {
                auto it = commonFactorials.begin();
                std::advance(it, randomInt(0, commonFactorials.size() - 1));
                
                int n = it->first;
                long long result = it->second;
                
                ss << "计算: " << n << "!";
                q.content = ss.str();
                q.answer = std::to_string(result);
                
                // 生成解析
                std::stringstream expl_ss;
                expl_ss << n << "! = ";
                for (int j = n; j >= 1; j--) {
                    expl_ss << j;
                    if (j > 1) expl_ss << " × ";
                }
                expl_ss << " = " << result;
                q.explanation = expl_ss.str();
                break;
            }
        }
        
        questions.push_back(q);
    }
    
    return questions;
}