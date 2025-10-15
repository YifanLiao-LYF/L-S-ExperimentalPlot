#include "level_manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

LevelManager::LevelManager() : totalScore(0) {
    initializeLevels();
}

void LevelManager::initializeLevels() {
    // 确保清空现有关卡
    levels.clear();
    
    // 添加16个关卡
    levels = {
        // 基础关卡 (1-6)
        {1, "有理数运算", "掌握基本的有理数加减乘除", LEVEL_UNLOCKED, 0, 0, 0},
        {2, "一元一次方程", "解一元一次方程的基础练习", LEVEL_LOCKED, 0, 0, 30},
        {3, "因式分解", "多项式因式分解技巧", LEVEL_LOCKED, 0, 0, 60},
        {4, "小数运算", "十以内小数加减乘除", LEVEL_LOCKED, 0, 0, 90},
        {5, "百以内小数", "百以内小数加减乘除", LEVEL_LOCKED, 0, 0, 120},
        {6, "复杂小数运算", "多步复杂小数运算", LEVEL_LOCKED, 0, 0, 150},
        
        // 新增关卡 (7-12)
        {7, "幂函数计算", "掌握幂函数的基本运算", LEVEL_LOCKED, 0, 0, 180},
        {8, "分数幂运算", "分数幂的运算与应用", LEVEL_LOCKED, 0, 0, 210},
        {9, "平方根计算", "平方根的运算与应用", LEVEL_LOCKED, 0, 0, 240},
        {10, "三角函数基础", "特殊角的三角函数值", LEVEL_LOCKED, 0, 0, 270},
        {11, "三角方程应用", "简单三角方程求解", LEVEL_LOCKED, 0, 0, 300},
        {12, "三角恒等式", "复合三角恒等式与不等式", LEVEL_LOCKED, 0, 0, 330},
        
        // 综合关卡 (13-16)
        {13, "综合关卡一", "综合前面所有知识点的挑战", LEVEL_LOCKED, 0, 0, 360},
        {14, "常数计算", "π值、e值和勾股定理的应用", LEVEL_LOCKED, 0, 0, 390},
        {15, "对数与阶乘", "对数和阶乘的计算", LEVEL_LOCKED, 0, 0, 420},
        {16, "综合关卡二", "终极综合挑战", LEVEL_LOCKED, 0, 0, 450}
    };
    
    // 调试输出，确认关卡数量
    std::cout << "DEBUG: 初始化了 " << levels.size() << " 个关卡" << std::endl;
}

Level LevelManager::getLevel(int levelId) {
    if (levelId < 1 || levelId > levels.size()) {
        std::cout << "DEBUG: 请求的关卡ID " << levelId << " 超出范围 (1-" << levels.size() << ")" << std::endl;
        return Level{0, "", "", LEVEL_LOCKED, 0, 0, 0};
    }
    
    return levels[levelId - 1];
}

std::vector<Level> LevelManager::getAllLevels() {
    std::cout << "DEBUG: getAllLevels() 返回 " << levels.size() << " 个关卡" << std::endl;
    return levels;
}

bool LevelManager::unlockLevel(int levelId) {
    if (levelId < 1 || levelId > levels.size()) {
        std::cout << "DEBUG: 尝试解锁无效关卡ID " << levelId << std::endl;
        return false;
    }
    
    Level& level = levels[levelId - 1];
    if (level.status == LEVEL_LOCKED && totalScore >= level.requiredScore) {
        level.status = LEVEL_UNLOCKED;
        std::cout << "DEBUG: 已解锁关卡 " << levelId << std::endl;
        return true;
    }
    
    return false;
}

bool LevelManager::completeLevel(int levelId, int score, int stars) {
    if (levelId < 1 || levelId > levels.size()) {
        std::cout << "DEBUG: 尝试完成无效关卡ID " << levelId << std::endl;
        return false;
    }
    
    Level& level = levels[levelId - 1];
    if (level.status == LEVEL_UNLOCKED) {
        level.status = LEVEL_COMPLETED;
        level.score = score;
        level.stars = stars;
        totalScore += score;
        
        std::cout << "DEBUG: 已完成关卡 " << levelId << ", 得分: " << score << ", 星级: " << stars << std::endl;
        
        // 尝试解锁下一关
        if (levelId < levels.size()) {
            unlockLevel(levelId + 1);
        }
        
        return true;
    }
    
    return false;
}

int LevelManager::getTotalScore() const {
    return totalScore;
}

void LevelManager::addScore(int score) {
    totalScore += score;
    std::cout << "DEBUG: 添加分数 " << score << ", 当前总分: " << totalScore << std::endl;
}

void LevelManager::displayLevelSelection() {
    std::cout << "\n=== 选择闯关关卡 ===\n";
    std::cout << "当前总分: " << totalScore << "\n";
    std::cout << "DEBUG: 显示关卡选择, 共有 " << levels.size() << " 个关卡\n\n";
    
    // 分页显示关卡
    int currentPage = 1;
    const int levelsPerPage = 8;
    int totalPages = (levels.size() + levelsPerPage - 1) / levelsPerPage;
    
    while (true) {
        std::cout << "--- 第 " << currentPage << " 页/共 " << totalPages << " 页 ---\n";
        
        int startIndex = (currentPage - 1) * levelsPerPage;
        int endIndex = std::min(startIndex + levelsPerPage - 1, static_cast<int>(levels.size()) - 1);
        
        for (int i = startIndex; i <= endIndex; i++) {
            const auto& level = levels[i];
            std::cout << level.id << ". " << level.title;
            
            switch (level.status) {
                case LEVEL_LOCKED:
                    std::cout << " [锁定 - 需要 " << level.requiredScore << " 分]";
                    break;
                case LEVEL_UNLOCKED:
                    std::cout << " [进行中]";
                    break;
                case LEVEL_COMPLETED:
                    std::cout << " [已完成]";
                    break;
            }
            
            std::cout << " ";
            for (int j = 0; j < level.stars; j++) {
                std::cout << "★";
            }
            std::cout << std::endl;
        }
        
        // 显示导航选项
        std::cout << "\n导航选项:\n";
        if (currentPage > 1) {
            std::cout << "P. 上一页\n";
        }
        if (currentPage < totalPages) {
            std::cout << "N. 下一页\n";
        }
        std::cout << "0. 返回主菜单\n";
        std::cout << "请选择关卡编号或操作: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // 处理导航选项
        if (input == "P" || input == "p") {
            if (currentPage > 1) currentPage--;
            continue;
        } else if (input == "N" || input == "n") {
            if (currentPage < totalPages) currentPage++;
            continue;
        } else if (input == "0") {
            return;
        }
        
        // 尝试将输入转换为关卡编号
        try {
            int choice = std::stoi(input);
            if (choice >= 1 && choice <= levels.size()) {
                std::cout << "已选择关卡 " << choice << std::endl;
                // 这里应该调用关卡开始函数
                return;
            } else {
                std::cout << "无效的关卡编号! 请输入 1-" << levels.size() << " 之间的数字。\n";
            }
        } catch (...) {
            std::cout << "无效输入! 请输入数字或操作指令。\n";
        }
        
        std::cout << "按回车键继续...";
        std::cin.get();
    }
}

void LevelManager::displayLevelProgress() {
    std::cout << "\n=== 我的闯关进度 ===\n";
    std::cout << "当前总分: " << totalScore << "\n";
    std::cout << "DEBUG: 显示关卡进度, 共有 " << levels.size() << " 个关卡\n\n";
    
    // 表头
    std::cout << std::left << std::setw(4) << "ID" 
              << std::setw(20) << "关卡名称" 
              << std::setw(10) << "状态" 
              << std::setw(8) << "星级" 
              << std::setw(10) << "分数" 
              << std::setw(15) << "解锁要求" 
              << std::endl;
    std::cout << std::string(67, '-') << std::endl;
    
    // 显示所有关卡
    for (const auto& level : levels) {
        std::cout << std::left << std::setw(4) << level.id;
        std::cout << std::setw(20) << (level.title.length() > 18 ? level.title.substr(0, 15) + "..." : level.title);
        
        // 状态
        std::string statusStr;
        switch (level.status) {
            case LEVEL_LOCKED: statusStr = "锁定"; break;
            case LEVEL_UNLOCKED: statusStr = "进行中"; break;
            case LEVEL_COMPLETED: statusStr = "已完成"; break;
        }
        std::cout << std::setw(10) << statusStr;
        
        // 星级
        std::string starsStr;
        for (int i = 0; i < level.stars; i++) {
            starsStr += "★";
        }
        if (starsStr.empty()) starsStr = "无";
        std::cout << std::setw(8) << starsStr;
        
        // 分数
        std::cout << std::setw(10) << (level.status == LEVEL_COMPLETED ? std::to_string(level.score) + "/100" : "-");
        
        // 解锁要求
        std::cout << std::setw(15) << (level.status == LEVEL_LOCKED ? std::to_string(level.requiredScore) + "分" : "已解锁");
        
        std::cout << std::endl;
    }
    
    // 统计信息
    int completed = 0;
    int totalStars = 0;
    for (const auto& level : levels) {
        if (level.status == LEVEL_COMPLETED) {
            completed++;
            totalStars += level.stars;
        }
    }
    
    std::cout << "\n统计: " << completed << "/" << levels.size() << " 关卡已完成, ";
    std::cout << "总共获得 " << totalStars << "/" << (levels.size() * 3) << " 颗星\n";
    
    std::cout << "\n按回车键返回主菜单...";
    std::cin.get();
}

int LevelManager::getTotalLevels() const {
    std::cout << "DEBUG: getTotalLevels() 返回 " << levels.size() << std::endl;
    return levels.size();
}

// 添加一个调试函数，用于检查关卡状态
void LevelManager::debugLevels() {
    std::cout << "DEBUG: 当前关卡状态:\n";
    for (const auto& level : levels) {
        std::cout << "关卡 " << level.id << ": " << level.title 
                  << " (状态: " << level.status 
                  << ", 分数: " << level.score 
                  << ", 星级: " << level.stars 
                  << ", 解锁要求: " << level.requiredScore << ")\n";
    }
}