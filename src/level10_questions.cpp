#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>

// 生成随机整数
// ...已移除重复的随机数函数，统一用question.h声明的实现...

// 特殊角度三角函数值映射
std::map<int, std::map<std::string, std::string>> specialAngles = {
    {0, {{"sin", "0"}, {"cos", "1"}, {"tan", "0"}}},
    {30, {{"sin", "1/2"}, {"cos", "√3/2"}, {"tan", "√3/3"}}},
    {45, {{"sin", "√2/2"}, {"cos", "√2/2"}, {"tan", "1"}}},
    {60, {{"sin", "√3/2"}, {"cos", "1/2"}, {"tan", "√3"}}},
    {90, {{"sin", "1"}, {"cos", "0"}, {"tan", "∞"}}}
};

// 生成关卡10题目（简单特殊三角函数值）
std::vector<Question> generateLevel10Questions() {
    std::vector<Question> questions;
    std::vector<std::string> functions = {"sin", "cos", "tan"};
    std::vector<int> angles = {0, 30, 45, 60, 90};
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = TRIGONOMETRY;
        
        // 随机选择三角函数和角度
        std::string func = functions[randomInt(0, 2)];
        int angle = angles[randomInt(0, 4)];
        
        // 避免tan(90°)的情况
        while (func == "tan" && angle == 90) {
            angle = angles[randomInt(0, 3)]; // 重新选择非90度的角度
        }
        
        std::stringstream ss;
        ss << func << "(" << angle << "°)";
        q.content = ss.str();
        
        // 获取答案
        q.answer = specialAngles[angle][func];
        
        // 生成解析
        std::stringstream expl_ss;
        expl_ss << "特殊角 " << angle << "° 的 " << func << " 值为 " << q.answer;
        q.explanation = expl_ss.str();
        
        questions.push_back(q);
    }
    
    return questions;
}