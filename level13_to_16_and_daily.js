// level13_to_16_and_daily.js
// 实现关卡13-16及每日挑战的 JS 版本，移植自 C++ 源文件

import { randomInt } from './random_utils.js';
import { generateLevel1Questions, generateLevel2Questions, generateLevel3Questions, generateLevel4Questions, generateLevel5Questions, generateLevel6Questions } from './level1_to_6_questions.js';
import { generateLevel7Questions, generateLevel8Questions, generateLevel9Questions, generateLevel10Questions, generateLevel11Questions, generateLevel12Questions } from './level7_to_12_questions.js';

class Question {
  constructor(id = 0, type = '', content = '', answer = '', explanation = '') {
    this.id = id; this.type = type; this.content = content; this.answer = answer; this.explanation = explanation;
  }
}

// 关卡13：综合关卡一（随机混合前12关题目）
export function generateLevel13Questions() {
  const questions = [];

  // 收集前12关题目
  const allQuestions = [
    generateLevel1Questions(), generateLevel2Questions(), generateLevel3Questions(), generateLevel4Questions(), generateLevel5Questions(), generateLevel6Questions(),
    generateLevel7Questions(), generateLevel8Questions(), generateLevel9Questions(), generateLevel10Questions(), generateLevel11Questions(), generateLevel12Questions()
  ];

  const numQuestions = randomInt(10, 15);

  for (let i = 0; i < numQuestions; i++) {
    const levelIndex = randomInt(0, allQuestions.length - 1);
    if (allQuestions[levelIndex].length === 0) { i--; continue; }
    const qIndex = randomInt(0, allQuestions[levelIndex].length - 1);
    const sel = allQuestions[levelIndex].splice(qIndex, 1)[0];
    sel.id = i + 1;
    questions.push(sel);
  }

  return questions;
}

// 每日挑战：基于日期种子选择题目（每天固定5道）
// JS 没有内置可复现的随机种子，使用简单 LCG 根据日期生成伪随机序列
function seededRng(seed) {
  // 小型线性同余生成器
  let state = seed % 2147483647;
  if (state <= 0) state += 2147483646;
  return function() {
    state = (state * 16807) % 2147483647;
    return (state - 1) / 2147483646;
  };
}

export function generateDailyChallenge(date = new Date()) {
  // 生成 YYYYMMDD 作为种子
  const seed = date.getFullYear() * 10000 + (date.getMonth() + 1) * 100 + date.getDate();
  const rng = seededRng(seed);

  const dailyQuestions = [];
  const generators = [
    generateLevel1Questions, generateLevel2Questions, generateLevel3Questions, generateLevel4Questions, generateLevel5Questions, generateLevel6Questions,
    generateLevel7Questions, generateLevel8Questions, generateLevel9Questions, generateLevel10Questions, generateLevel11Questions, generateLevel12Questions
  ];

  for (let i = 0; i < 5; i++) {
    // 使用 rng 生成关卡索引
    const levelIdx = Math.floor(rng() * generators.length);
    const levelQuestions = generators[levelIdx]();
    if (levelQuestions.length === 0) { i--; continue; }
    const qIdx = Math.floor(rng() * levelQuestions.length);
    const sel = levelQuestions[qIdx];
    sel.id = i + 1;
    dailyQuestions.push(sel);
  }

  return dailyQuestions;
}

// 常见π倍数与名称
const piMultiples = [Math.PI/6, Math.PI/4, Math.PI/3, Math.PI/2, 2*Math.PI/3, 3*Math.PI/4, 5*Math.PI/6, Math.PI, 7*Math.PI/6, 5*Math.PI/4, 4*Math.PI/3, 3*Math.PI/2, 5*Math.PI/3, 7*Math.PI/4, 11*Math.PI/6, 2*Math.PI];
const piMultiplesNames = ['π/6','π/4','π/3','π/2','2π/3','3π/4','5π/6','π','7π/6','5π/4','4π/3','3π/2','5π/3','7π/4','11π/6','2π'];

// 常见勾股数
const pythagoreanTriples = [
  [3,4,5], [5,12,13], [6,8,10], [7,24,25], [8,15,17], [9,12,15], [9,40,41], [10,24,26], [12,16,20], [12,35,37]
];

// 关卡14：常数计算（π、e、勾股）
export function generateLevel14Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'CONSTANT_CALCULATION';
    const pattern = randomInt(1, 3);

    if (pattern === 1) {
      const idx = randomInt(0, piMultiplesNames.length - 1);
      const operation = randomInt(1, 3); // 1: sin, 2: cos, 3: tan
      if (operation === 1) {
        q.content = `计算: sin(${piMultiplesNames[idx]})`;
        q.answer = String(Math.sin(piMultiples[idx]));
      } else if (operation === 2) {
        q.content = `计算: cos(${piMultiplesNames[idx]})`;
        q.answer = String(Math.cos(piMultiples[idx]));
      } else {
        q.content = `计算: tan(${piMultiplesNames[idx]})`;
        q.answer = String(Math.tan(piMultiples[idx]));
      }

      // 简化常见值格式
      if (q.answer === '1.000000') q.answer = '1';
      else if (q.answer === '0.000000') q.answer = '0';
      else if (q.answer === '0.500000') q.answer = '0.5';
      else if (q.answer.startsWith('0.707107')) q.answer = '√2/2';
      else if (q.answer.startsWith('0.866025')) q.answer = '√3/2';

      q.explanation = '使用特殊角的三角函数值进行计算';
    } else if (pattern === 2) {
      const exponent = randomInt(1, 5);
      q.content = `计算: e^${exponent}`;
      const result = Math.exp(exponent);
      q.answer = result.toFixed(2);
      q.explanation = `e ≈ 2.71828, e^${exponent} = ${q.answer}`;
    } else {
      const triple = pythagoreanTriples[randomInt(0, pythagoreanTriples.length - 1)];
      const a = triple[0], b = triple[1], c = triple[2];
      const missing = randomInt(1, 3);
      if (missing === 1) {
        q.content = `直角三角形中，b = ${b}, c = ${c}，求a`;
        q.answer = String(a);
        q.explanation = `使用勾股定理: a = √(c² - b²) = √(${c*c} - ${b*b}) = √${c*c - b*b} = ${a}`;
      } else if (missing === 2) {
        q.content = `直角三角形中，a = ${a}, c = ${c}，求b`;
        q.answer = String(b);
        q.explanation = `使用勾股定理: b = √(c² - a²) = √(${c*c} - ${a*a}) = √${c*c - a*a} = ${b}`;
      } else {
        q.content = `直角三角形中，a = ${a}, b = ${b}，求c`;
        q.answer = String(c);
        q.explanation = `使用勾股定理: c = √(a² + b²) = √(${a*a} + ${b*b}) = √${a*a + b*b} = ${c}`;
      }
    }

    questions.push(q);
  }
  return questions;
}

// 关卡15：对数与阶乘（表格方式）
const commonLogsEntries = [
  [1,0],[2,1],[4,2],[8,3],[16,4],[32,5],[64,6],[10,1],[100,2],[1000,3],[10000,4],[3,1],[9,2],[27,3],[81,4],[243,5],[5,1],[25,2],[125,3],[625,4]
];
const commonFactorialsEntries = [
  [0,1],[1,1],[2,2],[3,6],[4,24],[5,120],[6,720],[7,5040],[8,40320],[9,362880],[10,3628800]
];

export function generateLevel15Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'LOG_FACTORIAL';
    const pattern = randomInt(1, 2);
    if (pattern === 1) {
      const baseType = randomInt(1, 3);
      if (baseType === 1 || baseType === 2) {
        const it = commonLogsEntries[randomInt(0, commonLogsEntries.length - 1)];
        const value = it[0], result = it[1];
        if (baseType === 1) {
          q.content = `计算: log₂(${value})`;
          q.answer = String(result);
          q.explanation = `因为 2^${result} = ${value}`;
        } else {
          q.content = `计算: log₁₀(${value})`;
          q.answer = String(result);
          q.explanation = `因为 10^${result} = ${value}`;
        }
      } else {
        const value = randomInt(1, 5);
        q.content = `计算: ln(e^${value})`;
        q.answer = String(value);
        q.explanation = `ln(e^x) = x, 所以 ln(e^${value}) = ${value}`;
      }
    } else {
      const it = commonFactorialsEntries[randomInt(0, commonFactorialsEntries.length - 1)];
      const n = it[0], result = it[1];
      q.content = `计算: ${n}!`;
      q.answer = String(result);
      q.explanation = `${n}! = ${n} × ... × 1 = ${result}`;
    }
    questions.push(q);
  }
  return questions;
}

// 关卡16：综合关卡二（随机从前15关中选择，排除13）
export function generateLevel16Questions() {
  const questions = [];
  const generators = [
    generateLevel1Questions, generateLevel2Questions, generateLevel3Questions, generateLevel4Questions, generateLevel5Questions, generateLevel6Questions,
    generateLevel7Questions, generateLevel8Questions, generateLevel9Questions, generateLevel10Questions, generateLevel11Questions, generateLevel12Questions,
    generateLevel14Questions, generateLevel15Questions
  ];

  const numQuestions = randomInt(10, 15);
  for (let i = 0; i < numQuestions; i++) {
    const gIdx = randomInt(0, generators.length - 1);
    const levelQuestions = generators[gIdx]();
    if (levelQuestions.length === 0) { i--; continue; }
    const qIdx = randomInt(0, levelQuestions.length - 1);
    const sel = levelQuestions[qIdx];
    sel.id = i + 1;
    questions.push(sel);
  }

  return questions;
}
