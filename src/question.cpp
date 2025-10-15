#include "question.h"
#include <random>

// 统一实现所有关卡通用的随机数工具函数

double randomDecimal1(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    double value = distrib(gen);
    return std::round(value * 10) / 10; // 保留1位小数
}

double randomDecimal2(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(min, max);
    double value = distrib(gen);
    return std::round(value * 100) / 100; // 保留2位小数
}

int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}
