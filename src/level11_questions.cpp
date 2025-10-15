#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>

// 生成随机整数
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 特殊角度三角函数值映射
std::map<std::string, std::map<int, std::string>> specialValues = {
    {"1/2", {{30, "sin"}, {150, "sin"}, {210, "sin"}, {330, "sin"}, {60, "cos"}, {300, "cos"}}},
    {"√2/2", {{45, "sin"}, {135, "sin"}, {225, "sin"}, {315, "sin"}, {45, "cos"}, {315, "cos"}}},
    {"√3/2", {{60, "sin"}, {120, "sin"}, {240, "sin"}, {300, "sin"}, {30, "cos"}, {330, "cos"}}},
    {"1", {{90, "sin"}, {0, "cos"}, {360, "cos"}, {45, "tan"}, {225, "tan"}}},
    {"0", {{0, "sin"}, {180, "sin"}, {360, "sin"}, {90, "cos"}, {270, "cos"}, {0, "tan"}, {180, "tan"}, {360, "tan"}}}
};

// 生成关卡11题目（简单三角方程及应用）
std::vector<Question> generateLevel11Questions() {
    std::vector<Question> questions;
    std::vector<std::string> functions = {"sin", "cos", "tan"};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = TRIGONOMETRY;
        
        // 随机选择三角函数和特殊值
        std::string func = functions[randomInt(0, 2)];
        int valueIndex = randomInt(0, 4);
        
        std::string value;
        switch(valueIndex) {
            case 0: value = "1/2"; break;
            case 1: value = "√2/2"; break;
            case 2: value = "√3/2"; break;
            case 3: value = "1"; break;
            case 4: value = "0"; break;
        }
        
        // 创建方程
        std::stringstream ss;
        ss << func << "(x) = " << value << "，求x的一个解 (0° ≤ x < 360°)";
        q.content = ss.str();
        
        // 获取答案（选择第一个可能的解）
        auto& angleMap = specialValues[value];
        for (const auto& pair : angleMap) {
            if (pair.second == func) {
                q.answer = std::to_string(pair.first);
                break;
            }
        }
        
        // 生成解析
        std::stringstream expl_ss;
        expl_ss << func << "(x) = " << value << " 的解有: ";
        bool first = true;
        for (const auto& pair : angleMap) {
            if (pair.second == func) {
                if (!first) expl_ss << ", ";
                expl_ss << pair.first << "°";
                first = false;
            }
        }
        q.explanation = expl_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}