import assert from 'assert';
import { calculate } from '../calculator.js';
import { QuizManager } from '../quiz_manager.js';
import { LevelManager } from '../level_manager.js';
import { StorageManager } from '../storage_manager.js';

function testCalculator() {
  const cases = [
    { expr: '1+2', expect: '3' },
    { expr: '(3+4)*2/7', expect: '2' }, // (3+4)*2/7 = 14/7 = 2
    { expr: '2^3', expect: '8' },
    { expr: '3*4+1', expect: '13' },
    { expr: '0.5+0.25', expect: '0.75' },
  ];
  const results = [];
  for (const c of cases) {
    const out = calculate(c.expr);
    results.push({ expr: c.expr, out });
    // perform numeric comparison
    const numOut = Number(out);
    const numExpect = Number(c.expect);
    assert(!Number.isNaN(numOut), `Calculator returned NaN for ${c.expr}`);
    assert(Math.abs(numOut - numExpect) < 1e-8, `Calculator mismatch for ${c.expr}: got ${out}, want ${c.expect}`);
  }
  return results;
}

function testQuizManagerAllLevels() {
  const qm = new QuizManager();
  const report = [];
  for (let lvl = 1; lvl <= 16; lvl++) {
    qm.loadQuestionsForLevel(lvl);
    const n = qm.questionBank.length;
    if (n < 1) throw new Error(`Level ${lvl} produced no questions`);
    // iterate through questions and validate checkAnswer correctness
    qm.reset();
    let idx = 0;
    while (true) {
      const q = qm.getNextQuestion();
      if (!q || q.id === -1) break;
      idx++;
      // check answer by calling checkAnswer with correct answer
      const ok = qm.checkAnswer(q.answer);
      if (!ok) throw new Error(`QuizManager checkAnswer failed at level ${lvl} question ${idx}`);
    }
    report.push({ level: lvl, questions: n });
  }
  return report;
}

function testLevelManagerAndStorage() {
  const lm = new LevelManager();
  // initial: first unlocked, others locked
  const first = lm.getLevel(1);
  if (first.status !== 'LEVEL_UNLOCKED') throw new Error('Level 1 should be unlocked initially');
  // simulate completing level 1
  const ok = lm.completeLevel(1, 80, 2);
  if (!ok) throw new Error('Failed to complete level 1');
  // totalScore updated
  if (lm.getTotalScore() !== 80) throw new Error('Total score mismatch after completing level 1');
  // next level should be unlocked
  const lvl2 = lm.getLevel(2);
  if (lvl2.status !== 'LEVEL_UNLOCKED') throw new Error('Level 2 should have been unlocked');

  // test StorageManager fallback (no localStorage in Node) for save/load
  const sm = new StorageManager('test_key');
  const payload = { foo: 'bar', num: 123 };
  const saved = sm.save(payload);
  if (!saved) throw new Error('StorageManager.save returned false');
  const loaded = sm.load();
  assert(loaded.foo === 'bar' && loaded.num === 123, 'StorageManager load mismatch');
  sm.clear();
  const afterClear = sm.load();
  if (afterClear !== null) throw new Error('StorageManager.clear did not remove data');

  return true;
}

async function runAll() {
  const report = { calculator: null, quiz: null, level: null, success: true };
  try {
    report.calculator = testCalculator();
    report.quiz = testQuizManagerAllLevels();
    report.level = testLevelManagerAndStorage();
  } catch (e) {
    report.success = false;
    report.error = e.stack || e.message || String(e);
  }
  console.log('SYSTEM TEST REPORT');
  console.log(JSON.stringify(report, null, 2));
  if (!report.success) process.exit(2);
}

runAll();
