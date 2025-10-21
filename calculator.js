// calculator.js
// Browser-friendly JS port of calculator.ts

function getPriority(op) {
  if (op === '+' || op === '-') return 1;
  if (op === '*' || op === '/') return 2;
  if (op === '^') return 3;
  return 0;
}

function applyOperator(a, b, op) {
  switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
      if (Math.abs(b) < Number.EPSILON) throw new Error('数学错误: 除数不能为零');
      return a / b;
    case '^': return Math.pow(a, b);
    default: throw new Error('不支持的运算符');
  }
}

function isDigit(ch) { return ch >= '0' && ch <= '9'; }

function parseNumber(expr, index) {
  let numStr = '';
  const len = expr.length;
  while (index < len && (isDigit(expr[index]) || expr[index] === '.')) {
    numStr += expr[index];
    index++;
  }
  index--;
  if (numStr.length === 0) throw new Error('无效数字: ' + numStr);
  const value = Number(numStr);
  if (isNaN(value)) throw new Error('无效数字: ' + numStr);
  return { value: value, newIndex: index };
}

function popValue(values) {
  if (values.length === 0) throw new Error('数学错误: 操作数不足');
  return values.pop();
}

function evalExpression(expr) {
  const values = [];
  const operators = [];
  const s = expr;
  const len = s.length;

  for (let i = 0; i < len; i++) {
    const ch = s[i];
    if (ch === ' ') continue;
    if (ch === '(') {
      operators.push('(');
    } else if (ch === ')') {
      while (operators.length > 0 && operators[operators.length - 1] !== '(') {
        const b = popValue(values);
        const a = popValue(values);
        const op = operators.pop();
        values.push(applyOperator(a, b, op));
      }
      if (operators.length > 0) operators.pop();
    } else if (isDigit(ch)) {
      const parsed = parseNumber(s, i);
      values.push(parsed.value);
      i = parsed.newIndex;
    } else if (ch === '+' || ch === '-' || ch === '*' || ch === '/' || ch === '^') {
      if (ch === '-' && (i === 0 || s[i-1] === '(' || (i > 0 && (s[i-1] === '+' || s[i-1] === '-' || s[i-1] === '*' || s[i-1] === '/')))) {
        values.push(0.0);
      }
      while (operators.length > 0 && getPriority(operators[operators.length - 1]) >= getPriority(ch)) {
        const b = popValue(values);
        const a = popValue(values);
        const op = operators.pop();
        values.push(applyOperator(a, b, op));
      }
      operators.push(ch);
    } else {
      throw new Error("错误: 无效字符 '" + ch + "' 在表达式中");
    }
  }

  while (operators.length > 0) {
    const b = popValue(values);
    const a = popValue(values);
    const op = operators.pop();
    values.push(applyOperator(a, b, op));
  }

  if (values.length === 0) return 0;
  return values[values.length - 1];
}

function formatNumber(n, significantDigits) {
  if (!isFinite(n)) return String(n);
  let s = n.toPrecision(significantDigits);
  if (s.indexOf('.') >= 0) {
    s = s.replace(/(?:\.0+|0+)$/,'');
    s = s.replace(/\.$/, '');
  }
  return s;
}

export function calculate(expr) {
  try {
    const result = evalExpression(expr);
    const absRes = Math.abs(result);
    const res = (absRes < 1e-10) ? 0.0 : result;
    const out = formatNumber(res, 10);
    return out;
  } catch (e) {
    return '错误: ' + e.message;
  }
}

// expose for convenience
if (typeof window !== 'undefined') window.calculate = calculate;
