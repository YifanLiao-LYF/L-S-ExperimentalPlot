// types_and_managers.ts
// 对应 C++ 项目的 Question 和 Level 数据结构，以及管理器类的骨架

export enum QuestionType {
  ARITHMETIC = 'ARITHMETIC',
  POWER = 'POWER',
  FRACTION_POWER = 'FRACTION_POWER',
  ROOT = 'ROOT',
  TRIGONOMETRY = 'TRIGONOMETRY',
  CONSTANT_CALCULATION = 'CONSTANT_CALCULATION',
  LOG_FACTORIAL = 'LOG_FACTORIAL'
}

export class Question {
  id: number;
  type: QuestionType;
  content: string;
  answer: string;
  explanation: string;

  constructor(
    id: number = -1,
    type: QuestionType = QuestionType.ARITHMETIC,
    content: string = '',
    answer: string = '',
    explanation: string = ''
  ) {
    this.id = id;
    this.type = type;
    this.content = content;
    this.answer = answer;
    this.explanation = explanation;
  }
}

export enum LevelStatus {
  LEVEL_LOCKED = 'LEVEL_LOCKED',
  LEVEL_UNLOCKED = 'LEVEL_UNLOCKED',
  LEVEL_COMPLETED = 'LEVEL_COMPLETED'
}

export class Level {
  id: number;
  title: string;
  description: string;
  status: LevelStatus;
  score: number;
  stars: number;
  requiredScore: number;

  constructor(
    id: number,
    title: string,
    description: string,
    status: LevelStatus = LevelStatus.LEVEL_LOCKED,
    score: number = 0,
    stars: number = 0,
    requiredScore: number = 0
  ) {
    this.id = id;
    this.title = title;
    this.description = description;
    this.status = status;
    this.score = score;
    this.stars = stars;
    this.requiredScore = requiredScore;
  }
}

// QuizManager skeleton：对应 C++ 中的类接口（loadQuestionsForLevel, reset, getNextQuestion, checkAnswer, getExplanation）
export class QuizManager {
  private questions: Question[] = [];
  private currentIndex: number = 0;
  private lastExplanation: string = '';

  // 加载指定关卡的题目（应由外部实现具体题库）
  loadQuestionsForLevel(levelId: number): void {
    // TODO: 根据 levelId 加载题目到 this.questions
    this.questions = [];
    this.currentIndex = 0;
    this.lastExplanation = '';
  }

  reset(): void {
    this.currentIndex = 0;
    this.lastExplanation = '';
  }

  getNextQuestion(): Question {
    if (this.currentIndex >= this.questions.length) {
      return new Question(-1);
    }
    return this.questions[this.currentIndex++];
  }

  checkAnswer(userAnswer: string): boolean {
    // TODO: 简单比较或更复杂的答案解析
    const q = this.questions[this.currentIndex - 1];
    if (!q) return false;
    const ok = (userAnswer.trim() === q.answer.trim());
    this.lastExplanation = q.explanation;
    return ok;
  }

  getExplanation(): string {
    return this.lastExplanation;
  }
}

// LevelManager skeleton：对应 C++ 中的方法（initializeLevels, getLevel, getAllLevels, unlockLevel, completeLevel, getTotalScore, addScore, displayLevelSelection, displayLevelProgress）
export class LevelManager {
  private levels: Level[] = [];
  private totalScore: number = 0;

  constructor() {
    this.totalScore = 0;
    this.initializeLevels();
  }

  initializeLevels(): void {
    this.levels = [];
    // 在此初始化 16 个关卡（示例），实际可按 C++ 代码填充
    this.levels = [
      new Level(1, '有理数运算', '掌握基本的有理数加减乘除', LevelStatus.LEVEL_UNLOCKED, 0, 0, 0),
      new Level(2, '一元一次方程', '解一元一次方程的基础练习', LevelStatus.LEVEL_LOCKED, 0, 0, 30),
      new Level(3, '因式分解', '多项式因式分解技巧', LevelStatus.LEVEL_LOCKED, 0, 0, 60),
      new Level(4, '小数运算', '十以内小数加减乘除', LevelStatus.LEVEL_LOCKED, 0, 0, 90),
      new Level(5, '百以内小数', '百以内小数加减乘除', LevelStatus.LEVEL_LOCKED, 0, 0, 120),
      new Level(6, '复杂小数运算', '多步复杂小数运算', LevelStatus.LEVEL_LOCKED, 0, 0, 150),
      new Level(7, '幂函数计算', '掌握幂函数的基本运算', LevelStatus.LEVEL_LOCKED, 0, 0, 180),
      new Level(8, '分数幂运算', '分数幂的运算与应用', LevelStatus.LEVEL_LOCKED, 0, 0, 210),
      new Level(9, '平方根计算', '平方根的运算与应用', LevelStatus.LEVEL_LOCKED, 0, 0, 240),
      new Level(10, '三角函数基础', '特殊角的三角函数值', LevelStatus.LEVEL_LOCKED, 0, 0, 270),
      new Level(11, '三角方程应用', '简单三角方程求解', LevelStatus.LEVEL_LOCKED, 0, 0, 300),
      new Level(12, '三角恒等式', '复合三角恒等式与不等式', LevelStatus.LEVEL_LOCKED, 0, 0, 330),
      new Level(13, '综合关卡一', '综合前面所有知识点的挑战', LevelStatus.LEVEL_LOCKED, 0, 0, 360),
      new Level(14, '常数计算', 'π值、e值和勾股定理的应用', LevelStatus.LEVEL_LOCKED, 0, 0, 390),
      new Level(15, '对数与阶乘', '对数和阶乘的计算', LevelStatus.LEVEL_LOCKED, 0, 0, 420),
      new Level(16, '综合关卡二', '终极综合挑战', LevelStatus.LEVEL_LOCKED, 0, 0, 450)
    ];
  }

  getLevel(levelId: number): Level | null {
    if (levelId < 1 || levelId > this.levels.length) return null;
    return this.levels[levelId - 1];
  }

  getAllLevels(): Level[] {
    return this.levels.slice();
  }

  unlockLevel(levelId: number): boolean {
    if (levelId < 1 || levelId > this.levels.length) return false;
    const level = this.levels[levelId - 1];
    if (level.status === LevelStatus.LEVEL_LOCKED && this.totalScore >= level.requiredScore) {
      level.status = LevelStatus.LEVEL_UNLOCKED;
      return true;
    }
    return false;
  }

  completeLevel(levelId: number, score: number, stars: number): boolean {
    if (levelId < 1 || levelId > this.levels.length) return false;
    const level = this.levels[levelId - 1];
    if (level.status === LevelStatus.LEVEL_UNLOCKED) {
      level.status = LevelStatus.LEVEL_COMPLETED;
      level.score = score;
      level.stars = stars;
      this.totalScore += score;
      // 尝试解锁下一关
      if (levelId < this.levels.length) this.unlockLevel(levelId + 1);
      return true;
    }
    return false;
  }

  getTotalScore(): number {
    return this.totalScore;
  }

  addScore(score: number): void {
    this.totalScore += score;
  }

  // UI / debug helpers (skeletons to match C++ 方法名)
  displayLevelSelection(): void {
    console.log('显示关卡选择（模拟）：总分', this.totalScore);
  }

  displayLevelProgress(): void {
    console.log('显示关卡进度（模拟）');
  }
}

// MistakeManager skeleton：对应 C++ 的错题管理（addMistake, getMistakes）
export class MistakeManager {
  private mistakes: Question[] = [];

  addMistake(q: Question, userAnswer: string): void {
    // 在 C++ 中可能会保存用户错误答案、时间等；这里简单保存 question（你可以扩展为 MistakeRecord）
    this.mistakes.push(q);
  }

  getMistakes(): Question[] {
    return this.mistakes.slice();
  }

  clearMistakes(): void {
    this.mistakes = [];
  }
}

// Calculator skeleton：保持与 C++ 中的 calculate 函数同名接口
export class Calculator {
  // 简单解析数学表达式并返回字符串结果（可使用第三方库或实现更复杂的解析）
  static calculate(expr: string): string {
    // WARNING: 使用 eval 有安全风险。这里只作示例用途；生产请替换为安全解析器。
    try {
      // 基本清理：替换中文符号、禁止字母等
      const safe = expr.replace(/，/g, ',').replace(/×/g, '*').replace(/÷/g, '/');
      // 禁止字母以避免注入
      if (/[a-zA-Z]/.test(safe)) return '错误的表达式';
      // eslint-disable-next-line no-eval
      const val = eval(safe);
      return String(val);
    } catch (e) {
      return '表达式解析错误';
    }
  }
}
