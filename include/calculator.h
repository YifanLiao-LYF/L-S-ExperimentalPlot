#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator {
private:
    int getPriority(char op);
    double applyOperator(double a, double b, char op);
    double parseNumber(const std::string& expr, size_t& index);
    
public:
    double evalExpression(const std::string& expr);
};

// 全局计算函数
std::string calculate(const std::string& expr);

#endif