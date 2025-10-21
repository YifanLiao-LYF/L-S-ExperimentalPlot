// random_utils.js
// 对应 C++ 中 question.cpp 的随机函数实现，使用 Math.random()

/**
 * 返回 min 到 max（包含）的随机整数
 * 模拟 C++ std::uniform_int_distribution(min, max)
 */
export function randomInt(min, max) {
  if (!Number.isInteger(min) || !Number.isInteger(max)) {
    throw new TypeError('min 和 max 必须为整数');
  }
  if (max < min) {
    throw new RangeError('max 必须大于或等于 min');
  }
  // Math.random() 返回 [0,1)，将其映射到 [min, max]
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

/**
 * 返回保留 1 位小数的随机数，范围 [min, max) 类似于 C++ uniform_real_distribution
 * 最终结果四舍五入到 1 位小数。
 */
export function randomDecimal1(min, max) {
  if (typeof min !== 'number' || typeof max !== 'number') {
    throw new TypeError('min 和 max 必须为数字');
  }
  if (max < min) throw new RangeError('max 必须大于或等于 min');
  const r = Math.random() * (max - min) + min; // [min, max)
  // 四舍五入到 1 位小数
  return Math.round(r * 10) / 10;
}

/**
 * 返回保留 2 位小数的随机数，范围 [min, max)
 */
export function randomDecimal2(min, max) {
  if (typeof min !== 'number' || typeof max !== 'number') {
    throw new TypeError('min 和 max 必须为数字');
  }
  if (max < min) throw new RangeError('max 必须大于或等于 min');
  const r = Math.random() * (max - min) + min;
  return Math.round(r * 100) / 100;
}

// 默认导出可选
export default {
  randomInt,
  randomDecimal1,
  randomDecimal2
};
