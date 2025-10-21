// calculator.ts
// 基于 C++ calculator.cpp 的移植实现
// 提供：evalExpression(expr), getPriority(op), applyOperator(a,b,op), parseNumber(expr, index)

export function getPriority(op: string): number {
  if (op === '+' || op === '-') return 1;
  if (op === '*' || op === '/') return 2;
  if (op === '^') return 3;
  return 0;
}

export function applyOperator(a: number, b: number, op: string): number {
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

// parseNumber: 返回解析出的数字与新的索引位置（模拟 C++ 中 index 引用行为）
export function parseNumber(expr: string, index: number): { value: number; newIndex: number } {
  let numStr = '';
  const len = expr.length;
  while (index < len && (isDigit(expr[index]) || expr[index] === '.')) {
    numStr += expr[index];
    index++;
  }
  index--; // 回退一位以匹配 C++ 行为

  if (numStr.length === 0) {
    throw new Error('无效数字: ' + numStr);
  }

  const value = Number(numStr);
  if (isNaN(value)) throw new Error('无效数字: ' + numStr);
  return { value, newIndex: index };
}

function isDigit(ch: string): boolean { return ch >= '0' && ch <= '9'; }

export function evalExpression(expr: string): number {
  const values: number[] = [];
  const operators: string[] = [];
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
        const op = operators.pop()!;
        values.push(applyOperator(a, b, op));
      }
      if (operators.length > 0) operators.pop(); // 弹出 '('
    } else if (isDigit(ch)) {
      const { value, newIndex } = parseNumber(s, i);
      values.push(value);
      i = newIndex;
    } else if (ch === '+' || ch === '-' || ch === '*' || ch === '/' || ch === '^') {
      // 处理负号（一元）
      if (ch === '-' && (i === 0 || s[i-1] === '(' || (i > 0 && (s[i-1] === '+' || s[i-1] === '-' || s[i-1] === '*' || s[i-1] === '/')))) {
        values.push(0.0);
      }
      while (operators.length > 0 && getPriority(operators[operators.length - 1]) >= getPriority(ch)) {
        const b = popValue(values);
        const a = popValue(values);
        const op = operators.pop()!;
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
    const op = operators.pop()!;
    values.push(applyOperator(a, b, op));
  }

  if (values.length === 0) return 0;
  return values[values.length - 1];
}

function popValue(values: number[]): number {
  if (values.length === 0) throw new Error('数学错误: 操作数不足');
  return values.pop()!;
}

// calculate: 包装函数，返回字符串，处理浮点数精度与错误消息格式
export function calculate(expr: string): string {
  try {
    const result = evalExpression(expr);
    // 处理浮点误差：接近 0 的值归零
    const absRes = Math.abs(result);
    const res = (absRes < 1e-10) ? 0.0 : result;

    // 精度控制，输出与 C++ 中 precision(10) 类似的表现
    // toPrecision 会以科学计数或固定小数形式输出，使用 toString 结合 rounding
    // 我们以 10 位有效数字为准
    const out = formatNumber(res, 10);
    return out;
  } catch (e: any) {
    return '错误: ' + e.message;
  }
}

function formatNumber(n: number, significantDigits: number): string {
  if (!isFinite(n)) return String(n);
  // 使用 toPrecision, 但清理不必要的 0
  let s = n.toPrecision(significantDigits);
  // 去掉末尾的零和可能的点
  if (s.indexOf('.') >= 0) {
    s = s.replace(/(?:\.0+|0+)$/,'');
    s = s.replace(/\.$/, '');
  }
  return s;
}
