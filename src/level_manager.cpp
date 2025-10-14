#include "level_manager.h"
#include <iostream>

LevelManager::LevelManager() : totalScore(0) {
    initializeLevels();
}

void LevelManager::initializeLevels() {
    levels = {
        {1, "有理数运算", "掌握基本的有理数加减乘除", LEVEL_UNLOCKED, 0, 0, 0},
        {2, "一元一次方程", "解一元一次方程的基础练习", LEVEL_LOCKED, 0, 0, 30},
        {3, "因式分解", "多项式因式分解技巧", LEVEL_LOCKED, 0, 0, 60},
        {4, "小数运算", "十以内小数加减乘除", LEVEL_LOCKED, 0, 0, 90},
        {5, "百以内小数", "百以内小数加减乘除", LEVEL_LOCKED, 0, 0, 120},
        {6, "复杂小数运算", "多步复杂小数运算", LEVEL_LOCKED, 0, 0, 150}
    };
}

Level LevelManager::getLevel(int levelId) {
    for (auto& level : levels) {
        if (level.id == levelId) {
            return level;
        }
    }
    return Level{0, "", "", LEVEL_LOCKED, 0, 0, 0};
}

std::vector<Level> LevelManager::getAllLevels() {
    return levels;
}

bool LevelManager::unlockLevel(int levelId) {
    for (auto& level : levels) {
        if (level.id == levelId) {
            if (level.status == LEVEL_LOCKED && totalScore >= level.requiredScore) {
                level.status = LEVEL_UNLOCKED;
                return true;
            }
            break;
        }
    }
    return false;
}

bool LevelManager::completeLevel(int levelId, int score, int stars) {
    for (auto& level : levels) {
        if (level.id == levelId) {
            if (level.status == LEVEL_UNLOCKED) {
                level.status = LEVEL_COMPLETED;
                level.score = score;
                level.stars = stars;
                totalScore += score;
                
                // 尝试解锁下一关
                if (levelId < 6) {
                    unlockLevel(levelId + 1);
                }
                
                return true;
            }
            break;
        }
    }
    return false;
}

int LevelManager::getTotalScore() const {
    return totalScore;
}

void LevelManager::addScore(int score) {
    totalScore += score;
}

void LevelManager::displayLevelSelection() {
    std::cout << "\n=== 选择闯关关卡 ===\n";
    std::cout << "当前总分: " << totalScore << "\n\n";
    
    for (const auto& level : levels) {
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
        for (int i = 0; i < level.stars; i++) {
            std::cout << "★";
        }
        std::cout << std::endl;
    }
}

void LevelManager::displayLevelProgress() {
    std::cout << "\n=== 我的闯关进度 ===\n";
    for (const auto& level : levels) {
        std::cout << "关卡 " << level.id << ": " << level.title;
        
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