#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

// 生成随机整数
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成关卡9题目（平方根计算）
std::vector<Question> generateLevel9Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = ROOT;
        
        // 生成完全平方数
        int num = randomInt(1, 20);
        int square = num * num;
        
        std::stringstream ss;
        ss << "√" << square;
        q.content = ss.str();
        q.answer = std::to_string(num);
        
        // 生成解析
        std::stringstream expl_ss;
        expl_ss << "因为 " << num << " × " << num << " = " << square;
        expl_ss << ", 所以 √" << square << " = " << num;
        q.explanation = expl_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}