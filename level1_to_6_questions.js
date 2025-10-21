// level1_to_6_questions.js
// 将 src/level1_questions.cpp .. src/level6_questions.cpp 的题目生成逻辑移植至 JS

import { randomInt, randomDecimal1, randomDecimal2 } from './random_utils.js';

// Question 数据结构（轻量）
class Question {
  constructor(id = 0, type = 'ARITHMETIC', content = '', answer = '', explanation = '') {
    this.id = id;
    this.type = type;
    this.content = content;
    this.answer = answer;
    this.explanation = explanation;
  }
}

// 关卡1：十以内加减乘除
export function generateLevel1Questions() {
  const questions = [];
  const operators = ['+', '-', '×', '÷'];

  for (let i = 1; i <= 10; i++) {
    const q = new Question();
    q.id = i;
    q.type = 'ARITHMETIC';

    let num1 = randomInt(1, 10);
    let num2 = randomInt(1, 10);
    const op = operators[randomInt(0, 3)];

    if (op === '-' && num1 < num2) {
      [num1, num2] = [num2, num1];
    }

    if (op === '÷') {
      num2 = randomInt(1, 10);
      num1 = num2 * randomInt(1, 10);
      if (num1 > 10) num1 = num2 * randomInt(1, 5);
    }

    q.content = `${num1} ${op} ${num2}`;

    if (op === '+') q.answer = String(num1 + num2);
    else if (op === '-') q.answer = String(num1 - num2);
    else if (op === '×') q.answer = String(num1 * num2);
    else if (op === '÷') q.answer = String(num1 / num2);

    q.explanation = `直接计算: ${q.content} = ${q.answer}`;
    questions.push(q);
  }

  return questions;
}

// 关卡2：百以内加减乘除
export function generateLevel2Questions() {
  const questions = [];
  const operators = ['+', '-', '×', '÷'];

  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ARITHMETIC';

    let num1 = randomInt(10, 100);
    let num2 = randomInt(1, 100);
    const op = operators[randomInt(0, 3)];

    if (op === '-' && num1 < num2) [num1, num2] = [num2, num1];

    if (op === '÷') {
      num2 = randomInt(1, 20);
      num1 = num2 * randomInt(1, 10);
      if (num1 > 100) num1 = num2 * randomInt(1, 5);
    }

    q.content = `${num1} ${op} ${num2}`;

    if (op === '+') q.answer = String(num1 + num2);
    else if (op === '-') q.answer = String(num1 - num2);
    else if (op === '×') q.answer = String(num1 * num2);
    else if (op === '÷') q.answer = String(num1 / num2);

    q.explanation = `直接计算: ${q.content} = ${q.answer}`;
    questions.push(q);
  }
  return questions;
}

// 关卡3：多项式运算
export function generateLevel3Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ARITHMETIC';
    const num1 = randomInt(1, 20);
    const num2 = randomInt(1, 20);
    const num3 = randomInt(1, 20);

    const pattern = randomInt(1, 4);
    let content = '';
    let answer = '';
    let explanation = '';

    switch (pattern) {
      case 1: // a + b × c
        content = `${num1} + ${num2} × ${num3}`;
        answer = String(num1 + num2 * num3);
        explanation = `先乘除后加减: ${num2} × ${num3} = ${num2 * num3}, 然后 ${num1} + ${num2 * num3} = ${answer}`;
        break;
      case 2: // a × b + c
        content = `${num1} × ${num2} + ${num3}`;
        answer = String(num1 * num2 + num3);
        explanation = `先乘除后加减: ${num1} × ${num2} = ${num1 * num2}, 然后 ${num1 * num2} + ${num3} = ${answer}`;
        break;
      case 3: // (a + b) × c
        content = `(${num1} + ${num2}) × ${num3}`;
        answer = String((num1 + num2) * num3);
        explanation = `先计算括号内: ${num1} + ${num2} = ${num1 + num2}, 然后 ${num1 + num2} × ${num3} = ${answer}`;
        break;
      case 4: // a × (b + c)
        content = `${num1} × (${num2} + ${num3})`;
        answer = String(num1 * (num2 + num3));
        explanation = `先计算括号内: ${num2} + ${num3} = ${num2 + num3}, 然后 ${num1} × ${num2 + num3} = ${answer}`;
        break;
    }

    q.content = content; q.answer = answer; q.explanation = explanation;
    questions.push(q);
  }
  return questions;
}

// 关卡4：十以内小数运算（1位或2位小数）
export function generateLevel4Questions() {
  const questions = [];
  const operators = ['+', '-', '×', '÷'];

  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ARITHMETIC';

    const useOneDecimal = Math.random() < 0.5;
    let num1, num2;
    if (useOneDecimal) {
      num1 = randomDecimal1(1, 10);
      num2 = randomDecimal1(1, 10);
    } else {
      num1 = randomDecimal2(1, 10);
      num2 = randomDecimal2(1, 10);
    }

    const op = operators[randomInt(0, 3)];

    if (op === '-' && num1 < num2) [num1, num2] = [num2, num1];

    if (op === '÷') {
      num2 = randomDecimal1(1, 5);
      num1 = num2 * randomInt(1, 9 + 1);
      if (useOneDecimal) num1 = Math.round(num1 * 10) / 10;
      else num1 = Math.round(num1 * 100) / 100;
    }

    q.content = (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` ${op} ` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2));

    let result;
    if (op === '+') result = num1 + num2;
    else if (op === '-') result = num1 - num2;
    else if (op === '×') result = num1 * num2;
    else if (op === '÷') result = num1 / num2;

    q.answer = (useOneDecimal ? result.toFixed(1) : result.toFixed(2));
    q.explanation = `直接计算: ${q.content} = ${q.answer}`;
    questions.push(q);
  }
  return questions;
}

// 关卡5：百以内小数运算
export function generateLevel5Questions() {
  const questions = [];
  const operators = ['+', '-', '×', '÷'];

  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ARITHMETIC';
    const useOneDecimal = Math.random() < 0.5;
    let num1, num2;
    if (useOneDecimal) {
      num1 = randomDecimal1(10, 100);
      num2 = randomDecimal1(1, 100);
    } else {
      num1 = randomDecimal2(10, 100);
      num2 = randomDecimal2(1, 100);
    }

    const op = operators[randomInt(0, 3)];
    if (op === '-' && num1 < num2) [num1, num2] = [num2, num1];

    if (op === '÷') {
      num2 = randomDecimal1(1, 20);
      num1 = num2 * randomInt(1, 10);
      if (useOneDecimal) num1 = Math.round(num1 * 10) / 10;
      else num1 = Math.round(num1 * 100) / 100;
    }

    q.content = (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` ${op} ` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2));

    let result;
    if (op === '+') result = num1 + num2;
    else if (op === '-') result = num1 - num2;
    else if (op === '×') result = num1 * num2;
    else if (op === '÷') result = num1 / num2;

    q.answer = (useOneDecimal ? result.toFixed(1) : result.toFixed(2));
    q.explanation = `直接计算: ${q.content} = ${q.answer}`;
    questions.push(q);
  }
  return questions;
}

// 关卡6：复杂小数运算（多步）
export function generateLevel6Questions() {
  const questions = [];
  for (let i = 1; i <= 10; i++) {
    const q = new Question(); q.id = i; q.type = 'ARITHMETIC';
    const useOneDecimal = Math.random() < 0.5;
    let num1, num2, num3;
    if (useOneDecimal) {
      num1 = randomDecimal1(1, 20);
      num2 = randomDecimal1(1, 20);
      num3 = randomDecimal1(1, 20);
    } else {
      num1 = randomDecimal2(1, 20);
      num2 = randomDecimal2(1, 20);
      num3 = randomDecimal2(1, 20);
    }

    const pattern = randomInt(0, 3);
    let content = '';
    let result = 0;
    let explanation = '';

    switch (pattern) {
      case 0:
        content = (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` + ` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2)) + ` × ` + (useOneDecimal ? num3.toFixed(1) : num3.toFixed(2));
        result = num1 + num2 * num3;
        explanation = `先乘除后加减: ${num2} × ${num3} = ${num2 * num3}, 然后 ${num1} + ${num2 * num3} = ${result}`;
        break;
      case 1:
        content = (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` × ` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2)) + ` + ` + (useOneDecimal ? num3.toFixed(1) : num3.toFixed(2));
        result = num1 * num2 + num3;
        explanation = `先乘除后加减: ${num1} × ${num2} = ${num1 * num2}, 然后 ${num1 * num2} + ${num3} = ${result}`;
        break;
      case 2:
        content = `(` + (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` + ` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2)) + `) × ` + (useOneDecimal ? num3.toFixed(1) : num3.toFixed(2));
        result = (num1 + num2) * num3;
        explanation = `先计算括号内: ${num1} + ${num2} = ${num1 + num2}, 然后 ${num1 + num2} × ${num3} = ${result}`;
        break;
      case 3:
        content = (useOneDecimal ? num1.toFixed(1) : num1.toFixed(2)) + ` × (` + (useOneDecimal ? num2.toFixed(1) : num2.toFixed(2)) + ` + ` + (useOneDecimal ? num3.toFixed(1) : num3.toFixed(2)) + `)`;
        result = num1 * (num2 + num3);
        explanation = `先计算括号内: ${num2} + ${num3} = ${num2 + num3}, 然后 ${num1} × ${num2 + num3} = ${result}`;
        break;
    }

    q.content = content;
    q.answer = (useOneDecimal ? result.toFixed(1) : result.toFixed(2));
    q.explanation = explanation;
    questions.push(q);
  }
  return questions;
}
