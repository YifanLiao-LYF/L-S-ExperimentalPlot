#include <iostream>
#include <string>
#include <cmath>
#include <stack>
#include <cctype>
#include <sstream>
#include <map>
#include <stdexcept>

using namespace std;

class Calculator {
private:
    // 检查运算符优先级
    int getPriority(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        if (op == '^') return 3; // 支持指数运算
        return 0;
    }

    // 执行基础二元运算
    double applyOperator(double a, double b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("数学错误: 除数不能为零");
                return a / b;
            case '^': return pow(a, b); // 支持指数运算，如 2^3
            default: throw runtime_error("不支持的运算符");
        }
    }

    // 处理单个数字或变量的读取
    double parseNumber(const string& expr, size_t& index) {
        string numStr;
        while (index < expr.length() && (isdigit(expr[index]) || expr[index] == '.')) {
            numStr += expr[index++];
        }
        index--; // 回退一位，因为外层循环会再前进一位
        return stod(numStr);
    }

public:
    // 核心求值函数
    double evalExpression(const string& expr) {
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
            else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
                // 处理负号（一元运算符）：如果'-'是第一个字符，或前一个字符是'('或其他运算符
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
};

int main() {
    Calculator calc;
    string input;

    cout << "=== 初高中代数计算器 ===" << endl;
    cout << "支持功能: +, -, *, /, ^(指数), (), 负数" << endl;
    cout << "输入 'exit' 退出程序" << endl << endl;

    while (true) {
        cout << "请输入表达式: ";
        getline(cin, input);

        if (input == "exit") break;
        if (input.empty()) continue;

        try {
            double result = calc.evalExpression(input);
            cout << "结果: " << result << endl << endl;
        } 
        catch (const exception& e) {
            cerr << "计算错误: " << e.what() << endl << endl;
        }
    }

    return 0;
}