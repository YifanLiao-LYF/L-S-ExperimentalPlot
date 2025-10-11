#ifndef CALCULATOR_H // 头文件保护，防止重复包含
#define CALCULATOR_H

#include <string>

// 声明一个 calculate 函数
// 功能：接收一个字符串表达式，返回一个计算结果字符串
// 输入：std::string expr - 数学表达式字符串，如 "1+2 * 3"
// 返回：std::string - 计算结果的字符串，如 "7"
std::string calculate(const std::string& expr);

#endif