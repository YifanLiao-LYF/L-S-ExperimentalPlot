#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <vector>
#include <string>

enum LevelStatus {
    LEVEL_LOCKED,
    LEVEL_UNLOCKED,
    LEVEL_COMPLETED
};

struct Level {
    int id;
    std::string title;
    std::string description;
    LevelStatus status;
    int stars;
    int score;
    int requiredScore;
};

class LevelManager {
private:
    std::vector<Level> levels;
    int totalScore;
    
    void initializeLevels();
    
public:
    LevelManager();
    
    // 关卡管理
    Level getLevel(int levelId);
    std::vector<Level> getAllLevels();
    bool unlockLevel(int levelId);
    bool completeLevel(int levelId, int score, int stars);
    
    // 进度管理
    int getTotalScore() const;
    void addScore(int score);
    
    // 显示功能
    void displayLevelSelection();
    void displayLevelProgress();
};

#endif