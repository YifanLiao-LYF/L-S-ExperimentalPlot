#include <iostream>
#include "menu.h"
#include "calculator.h" // 为了调用计算功能
#include "quiz_manager.h"
#include "mistake_manager.h"
#include "level_system.h" // 新增这行

// 全局变量（实际项目中可以考虑使用单例模式或依赖注入）
QuizManager quizManager;
MistakeManager mistakeManager;

// 在 show_main_menu 函数前添加答题函数
void start_quiz() {
    std::cout << "\n=== 开始答题 ===\n";
    quizManager.reset();
    
    int questionCount = 0;
    int correctCount = 0;
    
    while (true) {
        Question q = quizManager.getNextQuestion();
        if (q.id == -1) {
            break; // 所有题目已完成
        }
        
        questionCount++;
        std::cout << "\n题目 " << questionCount << ": " << q.content << std::endl;
        std::cout << "请输入你的答案: ";
        
        std::string userAnswer;
        std::getline(std::cin, userAnswer);
        
        if (quizManager.checkAnswer(userAnswer)) {
            std::cout << "✓ 正确！" << std::endl;
            correctCount++;
        } else {
            std::cout << "✗ 错误！" << std::endl;
            std::cout << "正确答案: " << q.answer << std::endl;
            std::cout << "解析: " << quizManager.getExplanation() << std::endl;
            
            // 添加到错题集
            mistakeManager.addMistake(q, userAnswer);
        }
    }
    
    std::cout << "\n答题完成！得分: " << correctCount << "/" << questionCount << std::endl;
    std::cout << "按回车键返回主菜单...";
    std::cin.get();
}

// 添加查看错题集函数
void show_mistakes() {
    std::vector<Question> mistakes = mistakeManager.getMistakes();
    
    if (mistakes.empty()) {
        std::cout << "\n目前没有错题记录，继续保持！" << std::endl;
        return;
    }
    
    std::cout << "\n=== 错题集 ===\n";
    for (int i = 0; i < mistakes.size(); i++) {
        std::cout << "\n错题 " << (i + 1) << ":" << std::endl;
        std::cout << "题目: " << mistakes[i].content << std::endl;
        std::cout << "正确答案: " << mistakes[i].answer << std::endl;
        std::cout << "解析: " << mistakes[i].explanation << std::endl;
        std::cout << "----------------" << std::endl;
    }
    
    std::cout << "按回车键返回主菜单...";
    std::cin.get();
}

// ... 您已有的 start_quiz() 和 show_mistakes() 函数 ...

// ======== 新增的函数 ========
// 添加关卡选择函数
void show_level_selection() {
    auto levels = levelSystem.getAllLevels();
    
    std::cout << "\n=== 选择闯关关卡 ===\n";
    for (const auto& level : levels) {
        std::cout << level.levelId << ". " << level.title;
        if (!level.unlocked) {
            std::cout << " [锁定]";
        }
        std::cout << " ";
        // 显示星级
        for (int i = 0; i < level.stars; i++) {
            std::cout << "★";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n请选择关卡编号 (0返回主菜单): ";
    int choice;
    std::cin >> choice;
    std::cin.ignore();
    
    if (choice == 0) return;
    
    Level selectedLevel = levelSystem.getLevel(choice);
    if (selectedLevel.levelId == 0) {
        std::cout << "无效的关卡选择！" << std::endl;
        return;
    }
    
    if (!selectedLevel.unlocked) {
        std::cout << "该关卡尚未解锁！" << std::endl;
        return;
    }
    
    // 这里可以调用专门的关卡答题函数
    std::cout << "开始挑战: " << selectedLevel.title << std::endl;
    // start_level_quiz(selectedLevel.levelId);
}

// 添加查看成就函数
void show_achievements() {
    std::cout << "\n=== 我的成就 ===\n";
    // 这里显示成就列表
    std::cout << "成就系统开发中..." << std::endl;
}


void show_main_menu() {
    int choice = 0;
    
    while (true) { // 主循环
        // ... 显示菜单的代码 ...
        
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: {
                // ... 选项1的处理代码 ...
                break;
            }
            case 2: {
                // ... 选项2的处理代码 ...
                break;
            }
            case 3: {
                // ... 选项3的处理代码 ...
                break;
            }
            case 4: {
                // ... 选项4的处理代码 ...
                break;
            }
            case 5: {
                // ... 选项5的处理代码 ...
                break;
            }
            case 6: {
                std::cout << "感谢使用，再见！\n";
                return; // 修改这里：使用 return 退出函数
                // 或者使用：exit(0); // 直接终止程序
            }
            default: {
                std::cout << "输入无效，请重新选择！\n";
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
                break;
            }
        }
    }
}