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
    int score;
    int stars;
    int requiredScore;
};

class LevelManager {
private:
    std::vector<Level> levels;
    int totalScore;
    
    void initializeLevels();

public:
    LevelManager();
    Level getLevel(int levelId);
    std::vector<Level> getAllLevels();
    bool unlockLevel(int levelId);
    bool completeLevel(int levelId, int score, int stars);
    int getTotalScore() const;
    void addScore(int score);
    void displayLevelSelection();
    void displayLevelProgress();
    int getTotalLevels() const;
    
    // 添加调试函数
    void debugLevels();
};

#endif // LEVEL_MANAGER_H