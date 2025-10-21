// quiz_manager.js
// QuizManager JS 移植，保持与 C++ 类接口一致

import { randomInt } from './random_utils.js';
import { generateLevel1Questions, generateLevel2Questions, generateLevel3Questions, generateLevel4Questions, generateLevel5Questions, generateLevel6Questions } from './level1_to_6_questions.js';
import { generateLevel7Questions, generateLevel8Questions, generateLevel9Questions, generateLevel10Questions, generateLevel11Questions, generateLevel12Questions } from './level7_to_12_questions.js';
import { generateLevel13Questions, generateLevel16Questions, generateDailyChallenge } from './level13_to_16_and_daily.js';
import { generateLevel14Questions, generateLevel15Questions } from './level13_to_16_and_daily.js';

export class QuizManager {
  constructor() {
    this.questionBank = [];
    this.currentQuestionIndex = 0;
    this.currentLevelId = 1;
    this.loadQuestionsForLevel(1);
  }

  loadQuestionsForLevel(levelId) {
    this.currentLevelId = levelId;
    this.questionBank = [];
    this.currentQuestionIndex = 0;

    switch (levelId) {
      case 1: this.questionBank = generateLevel1Questions(); break;
      case 2: this.questionBank = generateLevel2Questions(); break;
      case 3: this.questionBank = generateLevel3Questions(); break;
      case 4: this.questionBank = generateLevel4Questions(); break;
      case 5: this.questionBank = generateLevel5Questions(); break;
      case 6: this.questionBank = generateLevel6Questions(); break;
      case 7: this.questionBank = generateLevel7Questions(); break;
      case 8: this.questionBank = generateLevel8Questions(); break;
      case 9: this.questionBank = generateLevel9Questions(); break;
      case 10: this.questionBank = generateLevel10Questions(); break;
      case 11: this.questionBank = generateLevel11Questions(); break;
      case 12: this.questionBank = generateLevel12Questions(); break;
      case 13: this.questionBank = generateLevel13Questions(); break;
      case 14: this.questionBank = generateLevel14Questions(); break;
      case 15: this.questionBank = generateLevel15Questions(); break;
      case 16: this.questionBank = generateLevel16Questions(); break;
      default: this.questionBank = generateLevel1Questions(); break;
    }

    if (this.questionBank.length < 5) {
      const fallback = this.generateFallbackQuestions();
      this.questionBank = this.questionBank.concat(fallback);
    }
  }

  loadDailyChallenge(challengeType) {
    // challengeType 1 -> level13, else level16
    this.currentLevelId = challengeType === 1 ? 13 : 16;
    this.questionBank = [];
    this.currentQuestionIndex = 0;
    if (challengeType === 1) {
      this.questionBank = generateLevel13Questions();
    } else {
      // generateDailyChallenge returns 5 questions seeded by date
      this.questionBank = generateDailyChallenge(new Date());
    }
    if (this.questionBank.length < 5) {
      const fallback = this.generateFallbackQuestions();
      this.questionBank = this.questionBank.concat(fallback);
    }
  }

  generateFallbackQuestions() {
    const questions = [];
    for (let i = 1; i <= 5; i++) {
      const a = randomInt(1, 10);
      const b = randomInt(1, 10);
      const q = {
        id: i,
        type: 'ARITHMETIC',
        content: `${a} + ${b}`,
        answer: String(a + b),
        explanation: `直接计算: ${a} + ${b} = ${a + b}`
      };
      questions.push(q);
    }
    return questions;
  }

  getNextQuestion() {
    console.debug('[QuizManager] getNextQuestion index', this.currentQuestionIndex, 'of', this.questionBank.length);
    if (this.currentQuestionIndex >= this.questionBank.length) {
      console.debug('[QuizManager] no more questions, returning sentinel');
      return { id: -1, type: 'ARITHMETIC', content: '所有题目已完成!', answer: '', explanation: '' };
    }
    const q = this.questionBank[this.currentQuestionIndex++];
    console.debug('[QuizManager] returning question id', q && q.id, 'new index', this.currentQuestionIndex);
    return q;
  }

  checkAnswer(userAnswer) {
    console.debug('[QuizManager] checkAnswer called, currentIndex', this.currentQuestionIndex);
    if (this.currentQuestionIndex === 0) return false;
    const current = this.questionBank[this.currentQuestionIndex - 1];
    const ok = userAnswer === current.answer;
    console.debug('[QuizManager] checking answer', userAnswer, 'against', current.answer, '->', ok);
    return ok;
  }

  getExplanation() {
    if (this.currentQuestionIndex === 0) return '';
    return this.questionBank[this.currentQuestionIndex - 1].explanation;
  }

  reset() {
    this.currentQuestionIndex = 0;
  }

  getCurrentLevelId() {
    return this.currentLevelId;
  }

  // 保留随机整数接口以兼容 C++ 接口（内部使用 random_utils）
  randomInt(min, max) {
    return randomInt(min, max);
  }
}
