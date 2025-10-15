#include "question.h"
#include <vector>
#include <string>
#include <sstream>

// 生成关卡7题目（幂函数计算）
std::vector<Question> generateLevel7Questions() {
    std::vector<Question> questions;
    for (int i = 1; i <= 10; i++) {
        Question q;
        q.id = i;
        q.type = POWER;
        int base = randomInt(2, 5);
        int exp = randomInt(2, 4);
        q.content = std::to_string(base) + "^" + std::to_string(exp);
        int ans = 1;
        for (int j = 0; j < exp; j++) ans *= base;
        q.answer = std::to_string(ans);
        q.explanation = "计算幂: " + q.content + " = " + q.answer;
        questions.push_back(q);
    }
    return questions;
}
