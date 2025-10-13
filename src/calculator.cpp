#include "calculator.h"
#include <iostream>
#include <stack>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <map>

using namespace std;

// 检查运算符优先级
int Calculator::getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// 执行基础二元运算
double Calculator::applyOperator(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("数学错误: 除数不能为零");
            return a / b;
        default: throw runtime_error("不支持的运算符");
    }
}

// 处理单个数字的读取
double Calculator::parseNumber(const string& expr, size_t& index) {
    string numStr;
    while (index < expr.length() && (isdigit(expr[index]) || expr[index] == '.')) {
        numStr += expr[index];
        index++;
    }
    index--; // 回退一位，因为外层循环会再前进一位
    
    try {
        return stod(numStr);
    } catch (...) {
        throw runtime_error("无效数字: " + numStr);
    }
}

// 核心求值函数
double Calculator::evalExpression(const string& expr) {
    stack<double> values;
    stack<char> operators;
    
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') continue; // 跳过空格

        if (expr[i] == '(') {
            operators.push('(');
        }
        else if (expr[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperator(a, b, op));
            }
            if (!operators.empty()) operators.pop(); // 弹出 '('
        }
        else if (isdigit(expr[i])) {
            values.push(parseNumber(expr, i));
        }
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            // 处理负号（一元运算符）
            if (expr[i] == '-' && (i == 0 || expr[i-1] == '(' || 
                (i>0 && (expr[i-1] == '+' || expr[i-1] == '-' || expr[i-1] == '*' || expr[i-1] == '/')))) {
                values.push(0.0); // 将 -x 转换为 0-x
            }
            // 根据优先级计算
            while (!operators.empty() && getPriority(operators.top()) >= getPriority(expr[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperator(a, b, op));
            }
            operators.push(expr[i]);
        }
        else {
            throw runtime_error("错误: 无效字符 '" + string(1, expr[i]) + "' 在表达式中");
        }
    }

    // 处理栈中剩余运算符
    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperator(a, b, op));
    }

    return values.top();
}

// 全局计算函数
string calculate(const string& expr) {
    Calculator calc;
    try {
        double result = calc.evalExpression(expr);
        
        // 处理浮点数精度问题，避免显示 -0.0
        if (abs(result) < 1e-10) result = 0.0;
        
        // 将结果转换为字符串
        stringstream ss;
        ss.precision(10); // 设置精度
        ss << result;
        return ss.str();
    } catch (const exception& e) {
        return "错误: " + string(e.what());
    }
}