#include "quiz_manager.h"
#include "level1_questions.h"
#include "level2_questions.h"
#include "level3_questions.h"
#include "level4_questions.h"
#include "level5_questions.h"
#include "level6_questions.h"

void QuizManager::loadQuestionsForLevel(int levelId) {
    currentLevelId = levelId;
    questionBank.clear();
    currentQuestionIndex = 0;
    
    switch (levelId) {
        case 1:
            questionBank = generateLevel1Questions();
            break;
        case 2:
            questionBank = generateLevel2Questions();
            break;
        case 3:
            questionBank = generateLevel3Questions();
            break;
        case 4:
            questionBank = generateLevel4Questions();
            break;
        case 5:
            questionBank = generateLevel5Questions();
            break;
        case 6:
            questionBank = generateLevel6Questions();
            break;
        default:
            // 默认加载关卡1题目
            questionBank = generateLevel1Questions();
            break;
    }
}