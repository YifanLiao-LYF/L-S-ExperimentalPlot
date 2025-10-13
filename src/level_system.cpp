#include "level_system.h"
#include <iostream>
#include <algorithm>

LevelSystem::LevelSystem() : currentLevelId(1), totalScore(0) {
    initializeLevels();
}

void LevelSystem::initializeLevels() {
    levels = {
        {1, "有理数运算", "掌握基本的有理数加减乘除", LEVEL_UNLOCKED, 0, 0, 0, 2},
        {2, "一元一次方程", "解一元一次方程的基础练习", LEVEL_LOCKED, 0, 0, 30, 3},
        {3, "因式分解", "多项式因式分解技巧", LEVEL_LOCKED, 0, 0, 60, 4},
        {4, "综合挑战", "综合能力测试", LEVEL_LOCKED, 0, 0, 90, -1}
    };
}

Level LevelSystem::getLevel(int levelId) {
    for (auto& level : levels) {
        if (level.levelId == levelId) {
            return level;
        }
    }
    return Level{0, "", "", LEVEL_LOCKED, 0, 0, 0, -1};
}

std::vector<Level> LevelSystem::getAllLevels() {
    return levels;
}

bool LevelSystem::unlockLevel(int levelId) {
    for (auto& level : levels) {
        if (level.levelId == levelId) {
            if (level.status == LEVEL_LOCKED && totalScore >= level.requiredScore) {
                level.status = LEVEL_UNLOCKED;
                std::cout << "🎉 新关卡已解锁: " << level.title << std::endl;
                return true;
            }
            break;
        }
    }
    return false;
}

bool LevelSystem::completeLevel(int levelId, int score, int stars) {
    for (auto& level : levels) {
        if (level.levelId == levelId) {
            if (level.status == LEVEL_UNLOCKED) {
                level.status = LEVEL_COMPLETED;
                level.score = score;
                level.stars = stars;
                
                // 更新总分
                totalScore += score;
                
                // 尝试解锁下一关
                if (level.nextLevelId != -1) {
                    unlockLevel(level.nextLevelId);
                }
                
                std::cout << "🎊 关卡完成! 获得 " << score << " 分，星级: ";
                for (int i = 0; i < stars; i++) std::cout << "★";
                std::cout << std::endl;
                
                return true;
            }
            break;
        }
    }
    return false;
}

Level LevelSystem::getNextLevel(int levelId) {
    for (const auto& level : levels) {
        if (level.levelId == levelId && level.nextLevelId != -1) {
            return getLevel(level.nextLevelId);
        }
    }
    return Level{0, "", "", LEVEL_LOCKED, 0, 0, 0, -1};
}

void LevelSystem::displayLevelSelection() {
    std::cout << "\n=== 选择闯关关卡 ===\n";
    std::cout << "当前总分: " << totalScore << "\n\n";
    
    for (const auto& level : levels) {
        std::cout << level.levelId << ". " << level.title;
        
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
        for (int i = 0; i < level.stars; i++) {
            std::cout << "★";
        }
        std::cout << std::endl;
    }
}

void LevelSystem::displayLevelProgress() {
    std::cout << "\n=== 我的闯关进度 ===\n";
    std::cout << "总分数: " << totalScore << std::endl;
    
    for (const auto& level : levels) {
        std::cout << "关卡 " << level.levelId << ": " << level.title;
        
        switch (level.status) {
            case LEVEL_LOCKED:
                std::cout << " [锁定]";
                break;
            case LEVEL_UNLOCKED:
                std::cout << " [进行中]";
                break;
            case LEVEL_COMPLETED:
                std::cout << " [已完成]";
                break;
        }
        
        std::cout << " ★" << level.stars;
        std::cout << " 分数: " << level.score << "/100";
        std::cout << std::endl;
    }
}