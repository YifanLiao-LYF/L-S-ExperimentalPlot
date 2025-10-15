#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <map>
#include <iomanip>  // 添加这行
// 生成随机整数
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 生成关卡12题目（复合三角恒等式与不等式）
std::vector<Question> generateLevel12Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = TRIGONOMETRY;
        
        int pattern = randomInt(1, 3);
        std::stringstream ss;
        
        switch (pattern) {
            case 1: // 恒等式验证
                ss << "验证恒等式: sin²θ + cos²θ = 1";
                q.content = ss.str();
                q.answer = "1";
                q.explanation = "这是三角函数的基本恒等式，对于任意角θ都成立";
                break;
                
            case 2: // 不等式求解
            {
                int angle = randomInt(0, 90);
                std::string func = (randomInt(0, 1) == 0) ? "sin" : "cos";
                
                ss << "解不等式: " << func << "θ > " << func << "(" << angle << "°), 0° ≤ θ ≤ 90°";
                q.content = ss.str();
                
                if (func == "sin") {
                    q.answer = angle + 1 < 90 ? std::to_string(angle + 1) + "° < θ ≤ 90°" : "无解";
                    q.explanation = "在0°到90°范围内，sinθ是增函数，所以θ > " + std::to_string(angle) + "°";
                } else {
                    q.answer = "0° ≤ θ < " + std::to_string(angle) + "°";
                    q.explanation = "在0°到90°范围内，cosθ是减函数，所以θ < " + std::to_string(angle) + "°";
                }
                break;
            }
                
            case 3: // 复合函数求值
            {
                int angle = randomInt(0, 90);
                int pattern2 = randomInt(1, 2);
                
                if (pattern2 == 1) {
                    ss << "计算: sin(" << angle << "° + 30°)";
                    q.content = ss.str();
                    
                    // 使用和角公式: sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
                    double sinA = sin(angle * M_PI / 180);
                    double cosA = cos(angle * M_PI / 180);
                    double sin30 = 0.5;
                    double cos30 = sqrt(3)/2;
                    
                    double result = sinA * cos30 + cosA * sin30;
                    
                    // 转换为分数形式（如果是特殊角）
                    if (angle == 0) q.answer = "1/2";
                    else if (angle == 30) q.answer = "√3/2";
                    else if (angle == 60) q.answer = "1";
                    else if (angle == 90) q.answer = "1/2";
                    else {
                        std::stringstream ans_ss;
                        ans_ss << std::fixed << std::setprecision(2) << result;
                        q.answer = ans_ss.str();
                    }
                    
                    q.explanation = "使用和角公式: sin(a+b) = sin(a)cos(b) + cos(a)sin(b)";
                } else {
                    ss << "计算: cos(" << angle << "° - 45°)";
                    q.content = ss.str();
                    
                    // 使用差角公式: cos(a-b) = cos(a)cos(b) + sin(a)sin(b)
                    double sinA = sin(angle * M_PI / 180);
                    double cosA = cos(angle * M_PI / 180);
                    double sin45 = sqrt(2)/2;
                    double cos45 = sqrt(2)/2;
                    
                    double result = cosA * cos45 + sinA * sin45;
                    
                    // 转换为分数形式（如果是特殊角）
                    if (angle == 45) q.answer = "1";
                    else if (angle == 90) q.answer = "√2/2";
                    else {
                        std::stringstream ans_ss;
                        ans_ss << std::fixed << std::setprecision(2) << result;
                        q.answer = ans_ss.str();
                    }
                    
                    q.explanation = "使用差角公式: cos(a-b) = cos(a)cos(b) + sin(a)sin(b)";
                }
                break;
            }
        }
        
        questions.push_back(q);
    }
    
    return questions;
}