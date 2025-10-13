#ifndef LEVEL_SYSTEM_H
#define LEVEL_SYSTEM_H

#include <string>
#include <vector>
#include <iostream>

// 关卡状态枚举
enum LevelStatus {
    LEVEL_LOCKED,     // 未解锁
    LEVEL_UNLOCKED,   // 已解锁但未完成
    LEVEL_COMPLETED   // 已完成
};

// 关卡结构体
struct Level {
    int levelId;            // 关卡ID
    std::string title;      // 关卡名称
    std::string description;// 关卡描述
    LevelStatus status;     // 关卡状态
    int stars;              // 获得星级
    int score;              // 关卡分数
    int requiredScore;      // 解锁所需分数
    int nextLevelId;        // 下一关ID (-1表示最后一关)
};

class LevelSystem {
private:
    std::vector<Level> levels;
    int currentLevelId;     // 当前正在玩的关卡
    int totalScore;
    
    void initializeLevels();
    
public:
    LevelSystem();
    
    // 关卡管理
    Level getLevel(int levelId);
    std::vector<Level> getAllLevels();
    bool unlockLevel(int levelId);
    bool completeLevel(int levelId, int score, int stars);
    Level getNextLevel(int levelId);
    
    // 进度管理
    int getCurrentLevelId() const { return currentLevelId; }
    void setCurrentLevelId(int levelId) { currentLevelId = levelId; }
    int getTotalScore() const { return totalScore; }
    void addScore(int score) { totalScore += score; }
    
    // 显示功能
    void displayLevelSelection();
    void displayLevelProgress();
};

#endif