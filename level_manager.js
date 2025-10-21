// level_manager.js
// 移植自 C++ level_manager.cpp

export const LevelStatus = {
  LEVEL_LOCKED: 'LEVEL_LOCKED',
  LEVEL_UNLOCKED: 'LEVEL_UNLOCKED',
  LEVEL_COMPLETED: 'LEVEL_COMPLETED'
};

export class Level {
  // unlockRule: optional object { type: 'prevStars'|'prevScore', value: number }
  constructor(id, title, description, status = LevelStatus.LEVEL_LOCKED, score = 0, stars = 0, requiredScore = 0, unlockRule = null) {
    this.id = id;
    this.title = title;
    this.description = description;
    this.status = status;
    this.score = score;
    this.stars = stars;
    this.requiredScore = requiredScore;
    this.unlockRule = unlockRule; // new
  }
}

export class LevelManager {
  constructor() {
    this.levels = [];
    this.totalScore = 0;
    this.initializeLevels();
  }

  initializeLevels() {
    // By default, use unlockRule that requires previous level to have at least 1 star.
    // This can be customized per level by modifying unlockRule.
    this.levels = [
      new Level(1, '有理数运算', '掌握基本的有理数加减乘除', LevelStatus.LEVEL_UNLOCKED, 0, 0, 0, null),
      new Level(2, '一元一次方程', '解一元一次方程的基础练习', LevelStatus.LEVEL_LOCKED, 0, 0, 30, { type: 'prevStars', value: 1 }),
      new Level(3, '因式分解', '多项式因式分解技巧', LevelStatus.LEVEL_LOCKED, 0, 0, 60, { type: 'prevStars', value: 1 }),
      new Level(4, '小数运算', '十以内小数加减乘除', LevelStatus.LEVEL_LOCKED, 0, 0, 90, { type: 'prevStars', value: 1 }),
      new Level(5, '百以内小数', '百以内小数加减乘除', LevelStatus.LEVEL_LOCKED, 0, 0, 120, { type: 'prevStars', value: 1 }),
      new Level(6, '复杂小数运算', '多步复杂小数运算', LevelStatus.LEVEL_LOCKED, 0, 0, 150, { type: 'prevStars', value: 1 }),
      new Level(7, '幂函数计算', '掌握幂函数的基本运算', LevelStatus.LEVEL_LOCKED, 0, 0, 180, { type: 'prevStars', value: 1 }),
      new Level(8, '分数幂运算', '分数幂的运算与应用', LevelStatus.LEVEL_LOCKED, 0, 0, 210, { type: 'prevStars', value: 1 }),
      new Level(9, '平方根计算', '平方根的运算与应用', LevelStatus.LEVEL_LOCKED, 0, 0, 240, { type: 'prevStars', value: 1 }),
      new Level(10, '三角函数基础', '特殊角的三角函数值', LevelStatus.LEVEL_LOCKED, 0, 0, 270, { type: 'prevStars', value: 1 }),
      new Level(11, '三角方程应用', '简单三角方程求解', LevelStatus.LEVEL_LOCKED, 0, 0, 300, { type: 'prevStars', value: 1 }),
      new Level(12, '三角恒等式', '复合三角恒等式与不等式', LevelStatus.LEVEL_LOCKED, 0, 0, 330, { type: 'prevStars', value: 1 }),
      new Level(13, '综合关卡一', '综合前面所有知识点的挑战', LevelStatus.LEVEL_LOCKED, 0, 0, 360, { type: 'prevStars', value: 1 }),
      new Level(14, '常数计算', 'π值、e值和勾股定理的应用', LevelStatus.LEVEL_LOCKED, 0, 0, 390, { type: 'prevStars', value: 1 }),
      new Level(15, '对数与阶乘', '对数和阶乘的计算', LevelStatus.LEVEL_LOCKED, 0, 0, 420, { type: 'prevStars', value: 1 }),
      new Level(16, '综合关卡二', '终极综合挑战', LevelStatus.LEVEL_LOCKED, 0, 0, 450, { type: 'prevStars', value: 1 })
    ];
    this.totalScore = 0;
  }

  getLevel(levelId) {
    if (levelId < 1 || levelId > this.levels.length) return null;
    return this.levels[levelId - 1];
  }

  getAllLevels() {
    return this.levels.slice();
  }

  unlockLevel(levelId) {
    if (levelId < 1 || levelId > this.levels.length) return false;
    const level = this.levels[levelId - 1];
    if (level.status !== LevelStatus.LEVEL_LOCKED) return false;
    // If unlockRule exists, evaluate based on that first
    if (level.unlockRule && typeof level.unlockRule === 'object') {
      const rule = level.unlockRule;
      if (rule.type === 'prevStars') {
        const prev = this.getLevel(levelId - 1);
        const prevStars = prev ? (prev.stars || 0) : 0;
        console.log(`[level_manager] unlockLevel check prevStars for level ${levelId}: prevStars=${prevStars}, required=${rule.value}`);
        if (prev && prevStars >= (rule.value || 0)) {
          level.status = LevelStatus.LEVEL_UNLOCKED;
          console.log(`[level_manager] level ${levelId} unlocked by prevStars`);
          return true;
        }
        console.log(`[level_manager] level ${levelId} remains locked by prevStars`);
        return false;
      }
      if (rule.type === 'prevScore') {
        const prev = this.getLevel(levelId - 1);
        const prevScore = prev ? (prev.score || 0) : 0;
        console.log(`[level_manager] unlockLevel check prevScore for level ${levelId}: prevScore=${prevScore}, required=${rule.value}`);
        if (prev && prevScore >= (rule.value || 0)) {
          level.status = LevelStatus.LEVEL_UNLOCKED;
          console.log(`[level_manager] level ${levelId} unlocked by prevScore`);
          return true;
        }
        console.log(`[level_manager] level ${levelId} remains locked by prevScore`);
        return false;
      }
    }
    // Fallback: unlock by totalScore threshold
    if (this.totalScore >= level.requiredScore) {
      level.status = LevelStatus.LEVEL_UNLOCKED;
      return true;
    }
    return false;
  }

  completeLevel(levelId, score, stars) {
    if (levelId < 1 || levelId > this.levels.length) return false;
    const level = this.levels[levelId - 1];
    // Allow completing a level that is unlocked or already completed (replay case).
    if (level.status === LevelStatus.LEVEL_UNLOCKED || level.status === LevelStatus.LEVEL_COMPLETED) {
      console.log(`[level_manager] completeLevel called for ${levelId} score=${score} stars=${stars} (prevStatus=${level.status}, prevScore=${level.score}, prevStars=${level.stars})`);
      // Mark as completed
      const prevScore = (level.status === LevelStatus.LEVEL_COMPLETED && typeof level.score === 'number') ? level.score : 0;
      // Only update stored score and totalScore if new score is higher than previous best
      if (score > prevScore) {
        // apply difference to totalScore
        this.totalScore += (score - prevScore);
        level.score = score;
        // keep the best star count (or prefer new if higher)
        level.stars = Math.max(level.stars || 0, stars || 0);
      } else {
        // do not lower stored score; but still consider increasing stars if provided
        level.stars = Math.max(level.stars || 0, stars || 0);
      }
      level.status = LevelStatus.LEVEL_COMPLETED;
      console.log(`[level_manager] level ${levelId} marked completed: newScore=${level.score}, newStars=${level.stars}, totalScore=${this.totalScore}`);
      // ensure next level is unlocked if possible
      if (levelId < this.levels.length) this.unlockLevel(levelId + 1);
      return true;
    }
    return false;
  }

  getTotalScore() {
    return this.totalScore;
  }

  addScore(score) {
    this.totalScore += score;
  }

  displayLevelSelection() {
    console.log('\n=== 选择闯关关卡 ===\n总分: ' + this.totalScore + '\n');
    const levelsPerPage = 8;
    const totalPages = Math.ceil(this.levels.length / levelsPerPage);
    for (let page = 1; page <= totalPages; page++) {
      console.log(`--- 第 ${page} 页/共 ${totalPages} 页 ---`);
      const start = (page - 1) * levelsPerPage;
      const end = Math.min(start + levelsPerPage, this.levels.length);
      for (let i = start; i < end; i++) {
        const level = this.levels[i];
        console.log(`${level.id}. ${level.title} [${level.status}] ${'★'.repeat(level.stars)}`);
      }
    }
  }

  displayLevelProgress() {
    console.log('\n=== 我的闯关进度 ===');
    console.log('当前总分: ' + this.totalScore + '\n');
    console.log('ID 关卡名称                 状态       星级  分数   解锁要求');
    console.log('-----------------------------------------------------------------');
    let completed = 0, totalStars = 0;
    for (const level of this.levels) {
      const starsStr = level.stars > 0 ? '★'.repeat(level.stars) : '无';
      const scoreStr = level.status === LevelStatus.LEVEL_COMPLETED ? `${level.score}/100` : '-';
      const unlockReq = level.status === LevelStatus.LEVEL_LOCKED ? `${level.requiredScore}分` : '已解锁';
      console.log(`${level.id.toString().padEnd(3)} ${level.title.padEnd(20)} ${level.status.padEnd(10)} ${starsStr.padEnd(4)} ${scoreStr.padEnd(6)} ${unlockReq}`);
      if (level.status === LevelStatus.LEVEL_COMPLETED) { completed++; totalStars += level.stars; }
    }
    console.log(`\n统计: ${completed}/${this.levels.length} 关卡已完成, 总共获得 ${totalStars}/${this.levels.length * 3} 颗星\n`);
  }

  getTotalLevels() {
    return this.levels.length;
  }

  debugLevels() {
    console.log('DEBUG: 当前关卡状态:');
    for (const level of this.levels) {
      console.log(`关卡 ${level.id}: ${level.title} (状态: ${level.status}, 分数: ${level.score}, 星级: ${level.stars}, 解锁要求: ${level.requiredScore})`);
    }
  }
}
