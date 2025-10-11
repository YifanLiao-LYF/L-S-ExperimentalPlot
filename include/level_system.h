#ifndef LEVEL_SYSTEM_H
#define LEVEL_SYSTEM_H

#include <string>
#include <vector>

// 关卡状态结构体
struct Level {
    int levelId;            // 关卡ID
    std::string title;      // 关卡名称
    std::string description;// 关卡描述
    bool unlocked;          // 是否已解锁
    int stars;              // 获得星级
    int score;              // 关卡分数
    int requiredScore;      // 解锁所需分数
};

// 成就类型枚举
enum AchievementType {
    ACH_FIRST_BLOOD,        // 首次完成关卡
    ACH_PERFECT_SCORE,      // 单个关卡满分
    ACH_MARATHON,           // 累计完成10个关卡
    ACH_MASTER              // 所有关卡达到3星
};

// 成就结构体
struct Achievement {
    AchievementType id;     // 成就ID
    std::string name;       // 成就名称
    std::string description;// 成就描述
    bool unlocked;          // 是否已解锁
};

// 完整的 LevelSystem 类定义
class LevelSystem {
private:
    std::vector<Level> levels;
    std::vector<Achievement> achievements;
    int totalScore;
    
    // 私有构造函数防止外部实例化
    LevelSystem();
    
    // 删除拷贝构造函数和赋值操作符防止复制
    LevelSystem(const LevelSystem&) = delete;
    LevelSystem& operator=(const LevelSystem&) = delete;
    
public:
    // 获取单例实例的静态方法
    static LevelSystem& getInstance();
    
    void initializeLevels();
    void initializeAchievements();
    std::vector<Level> getAllLevels();
    Level getLevel(int levelId);
    void updateLevelProgress(int levelId, int newScore, int newStars);
    void checkUnlockConditions();
    void checkAchievements();
    void displayProgress();
    
    // 获取总分数
    int getTotalScore() const { return totalScore; }
};

#endif