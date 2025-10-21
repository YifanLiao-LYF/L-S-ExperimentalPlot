// game_state_manager.js
// 将 LevelManager, QuizManager, MistakeManager 与 StorageManager 结合，负责保存/加载游戏状态

import { LevelManager } from './level_manager.js';
import { QuizManager } from './quiz_manager.js';
import { MistakeManager } from './mistake_manager.js';
import { StorageManager } from './storage_manager.js';

export class GameStateManager {
  constructor(storageKey) {
    this.levelManager = new LevelManager();
    this.quizManager = new QuizManager();
    this.mistakeManager = new MistakeManager();
    this.storage = new StorageManager(storageKey);
    this.currentLevelId = 1;
    this.currentScoreThisLevel = 0; // accumulate during a level
    // Simple event target so UI can listen for changes
    // Using a DOM EventTarget to avoid adding external libs
    this.events = new EventTarget();
    // try load existing state
    const loaded = this.loadState();
    if (loaded) {
      // applied in loadState
      // notify listeners
      this.events.dispatchEvent(new CustomEvent('statechange', { detail: this.getStateSnapshot() }));
    }
  }

  startLevel(levelId) {
    const level = this.levelManager.getLevel(levelId);
    if (!level) return false;
    if (level.status === 'LEVEL_LOCKED') return false;
    this.currentLevelId = levelId;
    this.currentScoreThisLevel = 0;
    this.quizManager.loadQuestionsForLevel(levelId);
    this.saveState();
    return true;
  }

  startDailyChallenge(type) {
    // type 1 or 2
    this.quizManager.loadDailyChallenge(type);
    this.currentLevelId = this.quizManager.getCurrentLevelId();
    this.currentScoreThisLevel = 0;
    this.saveState();
  }

  completeQuestion(answerCorrect, question, userAnswer, earned = 0) {
    if (!answerCorrect) {
      this.mistakeManager.addMistake(question, userAnswer);
    } else {
      this.currentScoreThisLevel += earned;
    }
    // persist small-changes
    this.saveState();
    this.events.dispatchEvent(new CustomEvent('statechange', { detail: this.getStateSnapshot() }));
  }

  completeLevel(levelId, score, stars) {
    const ok = this.levelManager.completeLevel(levelId, score, stars);
    if (ok) this.saveState();
    if (ok) this.events.dispatchEvent(new CustomEvent('statechange', { detail: this.getStateSnapshot() }));
    return ok;
  }

  saveState() {
    try {
      const payload = {
        levels: this.levelManager.getAllLevels(),
        totalScore: this.levelManager.getTotalScore(),
        currentLevelId: this.currentLevelId,
        currentScoreThisLevel: this.currentScoreThisLevel,
        mistakes: this.mistakeManager.toJSON()
      };
      const ok = this.storage.save(payload);
      console.log('[game_state_manager] saveState called - OK=', ok, 'payloadSummary=', { currentLevelId: this.currentLevelId, currentScoreThisLevel: this.currentScoreThisLevel, totalScore: this.levelManager.getTotalScore() });
      return ok;
    } catch (e) {
      console.error('GameStateManager.saveState error:', e);
      return false;
    }
  }

  loadState() {
    try {
      const data = this.storage.load();
      if (!data) return false;
      // apply levels if valid
      if (Array.isArray(data.levels) && data.levels.length === this.levelManager.getTotalLevels()) {
        // overwrite levels
        this.levelManager.levels = data.levels.map(l => {
          const LevelCtor = this.levelManager.getLevel(1).constructor;
          const level = new LevelCtor(l.id, l.title, l.description, l.status, l.score, l.stars, l.requiredScore, l.unlockRule ?? null);
          return level;
        });
      }
      if (typeof data.totalScore === 'number') this.levelManager.totalScore = data.totalScore;
      if (typeof data.currentLevelId === 'number') this.currentLevelId = data.currentLevelId;
      if (typeof data.currentScoreThisLevel === 'number') this.currentScoreThisLevel = data.currentScoreThisLevel;
      if (Array.isArray(data.mistakes)) this.mistakeManager.fromJSON(data.mistakes);
      return true;
    } catch (e) {
      console.error('GameStateManager.loadState error:', e);
      return false;
    }
  }

  resetState() {
    this.levelManager.initializeLevels();
    this.mistakeManager.clear();
    this.currentLevelId = 1;
    this.currentScoreThisLevel = 0;
    this.storage.clear();
    return true;
  }

  getStateSnapshot() {
    return {
      levels: this.levelManager.getAllLevels(),
      totalScore: this.levelManager.getTotalScore(),
      currentLevelId: this.currentLevelId,
      currentScoreThisLevel: this.currentScoreThisLevel,
      mistakes: this.mistakeManager.getMistakes()
    };
  }
}
