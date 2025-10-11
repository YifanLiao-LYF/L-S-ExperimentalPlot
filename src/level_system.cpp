#include "level_system.h"
#include <iostream>

// 构造函数（私有）
LevelSystem::LevelSystem() : totalScore(0) {
    initializeLevels();
    initializeAchievements();
}

// 获取单例实例
LevelSystem& LevelSystem::getInstance() {
    static LevelSystem instance; // 静态局部变量，保证线程安全(C++11及以上)
    return instance;
}

// 初始化关卡数据
void LevelSystem::initializeLevels() {
    levels = {
        {1, "有理数运算", "掌握基本的有理数加减乘除", true, 0, 0, 0},
        {2, "一元一次方程", "解一元一次方程的基础练习", false, 0, 0, 70},
        {3, "因式分解", "多项式因式分解技巧", false, 0, 0, 140},
        {4, "综合挑战", "综合能力测试", false, 0, 0, 210}
    };
}

// 初始化成就数据
void LevelSystem::initializeAchievements() {
    achievements = {
        {ACH_FIRST_BLOOD, "初战告捷", "完成第一个关卡", false},
        {ACH_PERFECT_SCORE, "完美无缺", "任何一个关卡获得满分", false},
        {ACH_MARATHON, "马拉松", "累计完成10个关卡", false},
        {ACH_MASTER, "闯关大师", "所有关卡达到3星评价", false}
    };
}

// 获取所有关卡
std::vector<Level> LevelSystem::getAllLevels() {
    return levels;
}

// 获取特定关卡
Level LevelSystem::getLevel(int levelId) {
    for (auto& level : levels) {
        if (level.levelId == levelId) {
            return level;
        }
    }
    return Level{0, "", "", false, 0, 0, 0};
}

// 更新关卡进度
void LevelSystem::updateLevelProgress(int levelId, int newScore, int newStars) {
    for (auto& level : levels) {
        if (level.levelId == levelId) {
            if (newScore > level.score) {
                level.score = newScore;
            }
            if (newStars > level.stars) {
                level.stars = newStars;
            }
            totalScore += newScore; // 更新总分
            break;
        }
    }
    checkUnlockConditions();
    checkAchievements();
}

// 检查关卡解锁条件
void LevelSystem::checkUnlockConditions() {
    for (auto& level : levels) {
        if (!level.unlocked && totalScore >= level.requiredScore) {
            level.unlocked = true;
            std::cout << "🎉 新关卡已解锁: " << level.title << std::endl;
        }
    }
}

// 检查成就达成
void LevelSystem::checkAchievements() {
    // 检查首次通关成就
    if (!achievements[ACH_FIRST_BLOOD].unlocked) {
        for (const auto& level : levels) {
            if (level.score > 0) {
                achievements[ACH_FIRST_BLOOD].unlocked = true;
                std::cout << "🎖️ 成就解锁: " << achievements[ACH_FIRST_BLOOD].name << std::endl;
                break;
            }
        }
    }
    
    // 检查完美得分成就
    if (!achievements[ACH_PERFECT_SCORE].unlocked) {
        for (const auto& level : levels) {
            if (level.score >= 100) { // 假设满分是100
                achievements[ACH_PERFECT_SCORE].unlocked = true;
                std::cout << "🎖️ 成就解锁: " << achievements[ACH_PERFECT_SCORE].name << std::endl;
                break;
            }
        }
    }
    
    // 可以继续添加其他成就的检查逻辑
}

// 显示关卡进度
void LevelSystem::displayProgress() {
    std::cout << "\n=== 我的闯关进度 ===\n";
    std::cout << "总分数: " << totalScore << std::endl;
    
    for (const auto& level : levels) {
        std::cout << "关卡 " << level.levelId << ": " << level.title;
        std::cout << " [" << (level.unlocked ? "已解锁" : "锁定中") << "]";
        std::cout << " ★" << level.stars;
        std::cout << " 分数: " << level.score << "/100";
        std::cout << std::endl;
    }
}