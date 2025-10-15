#include <iostream>
#include "menu.h"
#include "calculator.h"
#include "quiz_manager.h"
#include "mistake_manager.h"
#include "level_manager.h"

// 全局实例
QuizManager quizManager;
MistakeManager mistakeManager;
LevelManager levelManager;

// 闯关游戏主函数
void play_level(int levelId) {
    // 检查关卡是否有效
    if (levelId < 1 || levelId > 16) {
        std::cout << "无效的关卡编号！" << std::endl;
        return;
    }
    
    // 获取关卡信息
    Level level = levelManager.getLevel(levelId);
    
    if (level.status == LEVEL_LOCKED) {
        std::cout << "该关卡尚未解锁！需要 " << level.requiredScore << " 分才能解锁。" << std::endl;
        return;
    }
    
    std::cout << "\n🎯 开始挑战: 关卡 " << levelId << std::endl;
    
    // 加载该关卡的题目
    quizManager.loadQuestionsForLevel(levelId);
    quizManager.reset();
    
    int questionCount = 0;
    int correctCount = 0;
    const int questionsPerLevel = 5; // 每关5道题
    
    // 答题循环
    while (questionCount < questionsPerLevel) {
        Question q = quizManager.getNextQuestion();
        if (q.id == -1) break;
        
        questionCount++;
        std::cout << "\n题目 " << questionCount << "/" << questionsPerLevel << ": " << q.content << std::endl;
        std::cout << "请输入你的答案: ";
        
        std::string userAnswer;
        std::getline(std::cin, userAnswer);
        
        if (quizManager.checkAnswer(userAnswer)) {
            std::cout << "✅ 正确！" << std::endl;
            correctCount++;
        } else {
            std::cout << "❌ 错误！" << std::endl;
            std::cout << "正确答案: " << q.answer << std::endl;
            std::cout << "💡 解析: " << quizManager.getExplanation() << std::endl;
            
            // 添加到错题集
            mistakeManager.addMistake(q, userAnswer);
        }
    }
    
    // 计算得分和星级
    int score = (correctCount * 100) / questionsPerLevel;
    int stars = 0;
    if (score >= 90) stars = 3;
    else if (score >= 70) stars = 2;
    else if (score >= 50) stars = 1;
    
    // 完成关卡
    levelManager.completeLevel(levelId, score, stars);
    
    // 显示结果
    std::cout << "\n🎊 关卡完成！" << std::endl;
    std::cout << "✅ 正确: " << correctCount << "/" << questionsPerLevel << std::endl;
    std::cout << "📊 得分: " << score << "/100" << std::endl;
    std::cout << "⭐ 星级: ";
    for (int i = 0; i < stars; i++) std::cout << "★";
    std::cout << std::endl;
    
    // 检查是否有下一关
    if (levelId < 16) {
        Level nextLevel = levelManager.getLevel(levelId + 1);
        if (nextLevel.status == LEVEL_UNLOCKED) {
            std::cout << "\n🎯 下一关已解锁: 关卡 " << (levelId + 1) << std::endl;
            std::cout << "是否立即挑战下一关？(y/n): ";
            
            std::string choice;
            std::getline(std::cin, choice);
            
            if (choice == "y" || choice == "Y") {
                play_level(levelId + 1); // 进入下一关
            }
        }
    }
    
    std::cout << "按回车键返回主菜单...";
    std::cin.get();
}

// 关卡选择菜单
void show_level_selection() {
    int choice = 0;
    int page = 1; // 当前页码
    const int levelsPerPage = 8; // 每页显示的关卡数
    
    do {
        system("cls");
        std::cout << "==============================\n";
        std::cout << "         关卡选择 (第 " << page << " 页)\n";
        std::cout << "==============================\n";
        
        // 计算当前页的起始和结束关卡
        int startLevel = (page - 1) * levelsPerPage + 1;
        int endLevel = std::min(page * levelsPerPage, 16);
        
        // 显示当前页的关卡选项
        for (int i = startLevel; i <= endLevel; i++) {
            Level level = levelManager.getLevel(i);
            std::cout << i << ". 关卡 " << i;
            
            if (level.status == LEVEL_LOCKED) {
                std::cout << " [锁定 - 需要 " << level.requiredScore << " 分]";
            } else if (level.status == LEVEL_COMPLETED) {
                std::cout << " [已完成 - " << level.score << "分]";
            } else {
                std::cout << " [可挑战]";
            }
            
            std::cout << std::endl;
        }
        
        // 显示导航选项
        std::cout << "==============================\n";
        if (page > 1) {
            std::cout << "p. 上一页\n";
        }
        if (endLevel < 16) {
            std::cout << "n. 下一页\n";
        }
        std::cout << "0. 返回主菜单\n";
        std::cout << "==============================\n";
        std::cout << "请选择关卡 (1-16) 或导航选项: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // 处理导航选项
        if (input == "p" || input == "P") {
            if (page > 1) page--;
            continue;
        } else if (input == "n" || input == "N") {
            if (endLevel < 16) page++;
            continue;
        } else if (input == "0") {
            return;
        }
        
        // 尝试将输入转换为整数
        try {
            choice = std::stoi(input);
        } catch (...) {
            choice = -1; // 无效输入
        }
        
        if (choice >= 1 && choice <= 16) {
            play_level(choice);
        } else {
            std::cout << "无效选择！请选择 1-16 或导航选项。" << std::endl;
            std::cout << "按回车键继续...";
            std::cin.get();
        }
    } while (true);
}

// 查看错题集
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

// 专项练习函数
void practice_mode() {
    std::cout << "\n--> 专项练习模式\n";
    std::cout << "请输入数学表达式 (输入 'q' 返回主菜单):\n";
    
    std::string input;
    while (true) {
        std::cout << "\n请输入表达式 (或输入 'q' 返回): ";
        std::getline(std::cin, input);
        
        if (input == "q" || input == "Q") {
            break;
        }
        
        std::string result = calculate(input);
        std::cout << "结果: " << result << std::endl;
    }
}

// 主菜单
void show_main_menu() {
    int choice = 0;
    
    while (true) {
        system("cls");
        std::cout << "\n========== 数感闯关 ==========\n";
        std::cout << "1. 开始闯关\n";
        std::cout << "2. 专项练习\n";
        std::cout << "3. 查看错题集\n";
        std::cout << "4. 闯关进度\n";
        std::cout << "5. 退出程序\n";
        std::cout << "请选择 [1-5]: ";
        
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1:
                show_level_selection();
                break;
            case 2:
                practice_mode();
                break;
            case 3:
                show_mistakes();
                break;
            case 4:
                levelManager.displayLevelProgress();
                std::cout << "按回车键返回主菜单...";
                std::cin.get();
                break;
            case 5:
                std::cout << "感谢使用，再见！\n";
                return;
            default:
                std::cout << "输入无效，请重新选择！\n";
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                }
                std::cout << "按回车键继续...";
                std::cin.get();
                break;
        }
    }
}