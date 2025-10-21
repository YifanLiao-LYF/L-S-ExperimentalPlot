// level7_to_12_questions.js
// 移植 level7_questions.cpp .. level12_questions.cpp 的题目生成逻辑到 JS

import { randomInt } from './random_utils.js';

class Question {
  constructor(id = 0, type = '', content = '', answer = '', explanation = '') {
    this.id = id;
    this.type = type;
    this.content = content;
    this.answer = answer;
    this.explanation = explanation;
  }
}

// 辅助：计算 gcd
function gcd(a, b) {
  a = Math.abs(a); b = Math.abs(b);
  while (b !== 0) {
    const t = a % b; a = b; b = t;
  }
  return a;
}

// 关卡7：幂函数计算
export function generateLevel7Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'POWER';
    const base = randomInt(2, 5);
    const exp = randomInt(2, 4);
    q.content = `${base}^${exp}`;
    let ans = 1;
    for (let j = 0; j < exp; j++) ans *= base;
    q.answer = String(ans);
    q.explanation = `计算幂: ${q.content} = ${q.answer}`;
    questions.push(q);
  }
  return questions;
}

// 关卡8：分数幂运算（分数底数的整数次幂），分数化简到最简
export function generateLevel8Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'FRACTION_POWER';

    let numerator = randomInt(1, 10);
    let denominator = randomInt(2, 10);
    // 确保分数最简
    while (gcd(numerator, denominator) !== 1) {
      numerator = randomInt(1, 10);
      denominator = randomInt(2, 10);
    }

    const exponent = (randomInt(1, 10) <= 7) ? 2 : 3;
    q.content = `(${numerator}/${denominator})^${exponent}`;

    let num_result = 1;
    let den_result = 1;
    for (let j = 0; j < exponent; j++) {
      num_result *= numerator;
      den_result *= denominator;
    }

    const g = gcd(num_result, den_result);
    num_result = Math.floor(num_result / g);
    den_result = Math.floor(den_result / g);

    if (den_result === 1) q.answer = String(num_result);
    else q.answer = `${num_result}/${den_result}`;

    let expl = `分子分母分别求幂: (${numerator}^${exponent})/(${denominator}^${exponent}) = ${num_result}/${den_result}`;
    if (g !== 1) expl += ` = ${q.answer} (已化简)`;
    q.explanation = expl;

    questions.push(q);
  }
  return questions;
}

// 关卡9：平方根计算（完全平方数）
export function generateLevel9Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ROOT';
    const num = randomInt(1, 20);
    const square = num * num;
    q.content = `√${square}`;
    q.answer = String(num);
    q.explanation = `因为 ${num} × ${num} = ${square}, 所以 √${square} = ${num}`;
    questions.push(q);
  }
  return questions;
}

// 特殊角映射（与 C++ 保持一致）
const specialAngles = new Map([
  [0, { sin: '0', cos: '1', tan: '0' }],
  [30, { sin: '1/2', cos: '√3/2', tan: '√3/3' }],
  [45, { sin: '√2/2', cos: '√2/2', tan: '1' }],
  [60, { sin: '√3/2', cos: '1/2', tan: '√3' }],
  [90, { sin: '1', cos: '0', tan: '∞' }]
]);

// 关卡10：特殊三角函数值
export function generateLevel10Questions() {
  const questions = [];
  const functions = ['sin', 'cos', 'tan'];
  const angles = [0, 30, 45, 60, 90];

  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'TRIGONOMETRY';
    let func = functions[randomInt(0, 2)];
    let angle = angles[randomInt(0, 4)];

    // 避免 tan(90°)
    while (func === 'tan' && angle === 90) {
      angle = angles[randomInt(0, 3)];
    }

    q.content = `${func}(${angle}°)`;
    q.answer = specialAngles.get(angle)[func];
    q.explanation = `特殊角 ${angle}° 的 ${func} 值为 ${q.answer}`;
    questions.push(q);
  }
  return questions;
}

// 关卡11：三角方程应用（使用特殊角度映射表）
// 这里实现与 C++ 中 specialValues 数据结构的映射
const specialValues = new Map([
  ['1/2', new Map([[30, 'sin'], [150, 'sin'], [210, 'sin'], [330, 'sin'], [60, 'cos'], [300, 'cos']])],
  ['√2/2', new Map([[45, 'sin'], [135, 'sin'], [225, 'sin'], [315, 'sin'], [45, 'cos'], [315, 'cos']])],
  ['√3/2', new Map([[60, 'sin'], [120, 'sin'], [240, 'sin'], [300, 'sin'], [30, 'cos'], [330, 'cos']])],
  ['1', new Map([[90, 'sin'], [0, 'cos'], [360, 'cos'], [45, 'tan'], [225, 'tan']])],
  ['0', new Map([[0, 'sin'], [180, 'sin'], [360, 'sin'], [90, 'cos'], [270, 'cos'], [0, 'tan'], [180, 'tan'], [360, 'tan']])]
]);

export function generateLevel11Questions() {
  const questions = [];
  const functions = ['sin', 'cos', 'tan'];
  const values = ['1/2', '√2/2', '√3/2', '1', '0'];

  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'TRIGONOMETRY';
    const func = functions[randomInt(0, 2)];
    const value = values[randomInt(0, values.length - 1)];

    q.content = `${func}(x) = ${value}，求x的一个解 (0° ≤ x < 360°)`;

    // 选择第一个匹配的角度
    const angleMap = specialValues.get(value);
    let ansAngle = null;
    for (const [angle, f] of angleMap.entries()) {
      if (f === func) { ansAngle = angle; break; }
    }
    q.answer = ansAngle !== null ? String(ansAngle) : '';

    // 生成解析：列出所有解
    const explParts = [];
    for (const [angle, f] of angleMap.entries()) {
      if (f === func) explParts.push(`${angle}°`);
    }
    q.explanation = `${func}(x) = ${value} 的解有: ${explParts.join(', ')}`;
    questions.push(q);
  }
  return questions;
}

// 关卡12：复合三角恒等式与不等式
export function generateLevel12Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'TRIGONOMETRY';
    const pattern = randomInt(1, 3);

    if (pattern === 1) {
      q.content = '验证恒等式: sin²θ + cos²θ = 1';
      q.answer = '1';
      q.explanation = '这是三角函数的基本恒等式，对于任意角θ都成立';
    } else if (pattern === 2) {
      const angle = randomInt(0, 90);
      const func = (randomInt(0, 1) === 0) ? 'sin' : 'cos';
      q.content = `解不等式: ${func}θ > ${func}(${angle}°), 0° ≤ θ ≤ 90°`;
      if (func === 'sin') {
        q.answer = (angle + 1 < 90) ? `${angle + 1}° < θ ≤ 90°` : '无解';
        q.explanation = `在0°到90°范围内，sinθ是增函数，所以θ > ${angle}°`;
      } else {
        q.answer = `0° ≤ θ < ${angle}°`;
        q.explanation = `在0°到90°范围内，cosθ是减函数，所以θ < ${angle}°`;
      }
    } else {
      const angle = randomInt(0, 90);
      const pattern2 = randomInt(1, 2);
      if (pattern2 === 1) {
        q.content = `计算: sin(${angle}° + 30°)`;
        // sin(a+b) = sin(a)cos(b) + cos(a)sin(b)
        const sinA = Math.sin(angle * Math.PI / 180);
        const cosA = Math.cos(angle * Math.PI / 180);
        const sin30 = 0.5;
        const cos30 = Math.sqrt(3) / 2;
        const result = sinA * cos30 + cosA * sin30;

        if (angle === 0) q.answer = '1/2';
        else if (angle === 30) q.answer = '√3/2';
        else if (angle === 60) q.answer = '1';
        else if (angle === 90) q.answer = '1/2';
        else q.answer = result.toFixed(2);

        q.explanation = '使用和角公式: sin(a+b) = sin(a)cos(b) + cos(a)sin(b)';
      } else {
        q.content = `计算: cos(${angle}° - 45°)`;
        const sinA = Math.sin(angle * Math.PI / 180);
        const cosA = Math.cos(angle * Math.PI / 180);
        const sin45 = Math.sqrt(2) / 2;
        const cos45 = Math.sqrt(2) / 2;
        const result = cosA * cos45 + sinA * sin45;

        if (angle === 45) q.answer = '1';
        else if (angle === 90) q.answer = '√2/2';
        else q.answer = result.toFixed(2);

        q.explanation = '使用差角公式: cos(a-b) = cos(a)cos(b) + sin(a)sin(b)';
      }
    }

    questions.push(q);
  }
  return questions;
}
