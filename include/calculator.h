#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator {
public:
    double evalExpression(const std::string& expr);
};

std::string calculate(const std::string& expr);

#endif