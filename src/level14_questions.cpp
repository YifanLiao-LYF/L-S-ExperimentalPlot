#include "question.h"
#include <random>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <map>

// 生成随机整数
int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// 常见π倍数
std::vector<double> piMultiples = {M_PI/6, M_PI/4, M_PI/3, M_PI/2, 2*M_PI/3, 3*M_PI/4, 5*M_PI/6, M_PI, 7*M_PI/6, 5*M_PI/4, 4*M_PI/3, 3*M_PI/2, 5*M_PI/3, 7*M_PI/4, 11*M_PI/6, 2*M_PI};
std::vector<std::string> piMultiplesNames = {"π/6", "π/4", "π/3", "π/2", "2π/3", "3π/4", "5π/6", "π", "7π/6", "5π/4", "4π/3", "3π/2", "5π/3", "7π/4", "11π/6", "2π"};

// 常见勾股数
std::vector<std::tuple<int, int, int>> pythagoreanTriples = {
    {3, 4, 5}, {5, 12, 13}, {6, 8, 10}, {7, 24, 25}, {8, 15, 17}, 
    {9, 12, 15}, {9, 40, 41}, {10, 24, 26}, {12, 16, 20}, {12, 35, 37}
};

// 生成关卡14题目（π值、e值、勾股定理）
std::vector<Question> generateLevel14Questions() {
    std::vector<Question> questions;
    
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = CONSTANT_CALCULATION;
        
        int pattern = randomInt(1, 3);
        std::stringstream ss;
        
        switch (pattern) {
            case 1: // π值计算
            {
                int idx = randomInt(0, piMultiplesNames.size() - 1);
                int operation = randomInt(1, 3); // 1: sin, 2: cos, 3: tan
                
                switch (operation) {
                    case 1:
                        ss << "计算: sin(" << piMultiplesNames[idx] << ")";
                        q.content = ss.str();
                        q.answer = std::to_string(sin(piMultiples[idx]));
                        break;
                    case 2:
                        ss << "计算: cos(" << piMultiplesNames[idx] << ")";
                        q.content = ss.str();
                        q.answer = std::to_string(cos(piMultiples[idx]));
                        break;
                    case 3:
                        ss << "计算: tan(" << piMultiplesNames[idx] << ")";
                        q.content = ss.str();
                        q.answer = std::to_string(tan(piMultiples[idx]));
                        break;
                }
                
                // 简化答案格式
                if (q.answer == "1.000000") q.answer = "1";
                else if (q.answer == "0.000000") q.answer = "0";
                else if (q.answer == "0.500000") q.answer = "0.5";
                else if (q.answer == "0.707107") q.answer = "√2/2";
                else if (q.answer == "0.866025") q.answer = "√3/2";
                
                q.explanation = "使用特殊角的三角函数值进行计算";
                break;
            }
                
            case 2: // e值计算
            {
                int exponent = randomInt(1, 5);
                ss << "计算: e^" << exponent;
                q.content = ss.str();
                
                double result = exp(exponent);
                std::stringstream ans_ss;
                ans_ss << std::fixed << std::setprecision(2) << result;
                q.answer = ans_ss.str();
                
                q.explanation = "e ≈ 2.71828, e^" + std::to_string(exponent) + " = " + q.answer;
                break;
            }
                
            case 3: // 勾股定理
            {
                auto triple = pythagoreanTriples[randomInt(0, pythagoreanTriples.size() - 1)];
                int a = std::get<0>(triple);
                int b = std::get<1>(triple);
                int c = std::get<2>(triple);
                
                int missingSide = randomInt(1, 3); // 1: 缺a, 2: 缺b, 3: 缺c
                
                if (missingSide == 1) {
                    ss << "直角三角形中，b = " << b << ", c = " << c << "，求a";
                    q.content = ss.str();
                    q.answer = std::to_string(a);
                    q.explanation = "使用勾股定理: a = √(c² - b²) = √(" + 
                                   std::to_string(c*c) + " - " + std::to_string(b*b) + 
                                   ") = √" + std::to_string(c*c - b*b) + " = " + std::to_string(a);
                } else if (missingSide == 2) {
                    ss << "直角三角形中，a = " << a << ", c = " << c << "，求b";
                    q.content = ss.str();
                    q.answer = std::to_string(b);
                    q.explanation = "使用勾股定理: b = √(c² - a²) = √(" + 
                                   std::to_string(c*c) + " - " + std::to_string(a*a) + 
                                   ") = √" + std::to_string(c*c - a*a) + " = " + std::to_string(b);
                } else {
                    ss << "直角三角形中，a = " << a << ", b = " << b << "，求c";
                    q.content = ss.str();
                    q.answer = std::to_string(c);
                    q.explanation = "使用勾股定理: c = √(a² + b²) = √(" + 
                                   std::to_string(a*a) + " + " + std::to_string(b*b) + 
                                   ") = √" + std::to_string(a*a + b*b) + " = " + std::to_string(c);
                }
                break;
            }
        }
        
        questions.push_back(q);
    }
    
    return questions;
}