// math_render.js
// 提供数学公式的符号映射、LaTeX 转换与 MathJax 渲染辅助（带去抖）

let mathJaxLoaded = false;
let typesetTimer = null;

// Ensure MathJax exists on page. If not, dynamically insert CDN script and wait for it to load.
export function ensureMathJax() {
  return new Promise((resolve) => {
    if (window.MathJax && window.MathJax.typesetPromise) {
      mathJaxLoaded = true;
      resolve(true);
      return;
    }
    // create simple config (if page has custom config this is fine)
    window.MathJax = window.MathJax || {
      tex: { inlineMath: [['$', '$'], ['\\(', '\\)']], displayMath: [['$$','$$']] },
      svg: { fontCache: 'global' }
    };
    const script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-svg.js';
    script.async = true;
    script.onload = () => { mathJaxLoaded = true; resolve(true); };
    script.onerror = () => { console.warn('MathJax 加载失败'); resolve(false); };
    document.head.appendChild(script);
  });
}

// mapping for special unicode symbols to LaTeX commands
const SYMBOL_MAP = {
  '×': '\\times',
  '÷': '\\div',
  '√': '\\sqrt',
  // caret ^ is OK, but ensure common full-width variants or special minus signs if needed
};

export function convertSymbolsToLatex(text) {
  if (!text || typeof text !== 'string') return text;
  // Replace fractions like a/b with LaTeX \frac{a}{b} only when pattern matches integer/decimal tokens
  // We'll avoid replacing things inside existing $...$ blocks by simple approach: process full string
  let out = text;

  // Replace special symbols
  Object.keys(SYMBOL_MAP).forEach(sym => {
    const re = new RegExp(escapeRegExp(sym), 'g');
    out = out.replace(re, SYMBOL_MAP[sym]);
  });

  // Replace occurrences of pattern like (a/b) or a/b but be conservative: require digits around slash
  // We'll convert patterns such as 3/4, 12/5, 7/8, optionally with parentheses
  out = out.replace(/(?<![\\\w\$])([0-9]+(?:\.[0-9]+)?)\s*\/\s*([0-9]+(?:\.[0-9]+)?)(?![\\\w\$])/g, (m, a, b) => {
    return `\\frac{${a}}{${b}}`;
  });

  // Convert leading sqrt symbol sequences like \sqrt or \sqrt{...} handled by MathJax; if we found literal \sqrt from SYMBOL_MAP we'll leave it
  return out;
}

function escapeRegExp(s) { return s.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'); }

export function setAndRender(container, htmlOrText, options = {}) {
  // options: { inline: boolean }
  if (!container) return Promise.resolve(false);
  let content = htmlOrText;
  // If text is plain, convert symbols
  content = convertSymbolsToLatex(content);

  // If inline requested, wrap with $...$, else use display $$...$$ only when string has newline or \frac etc.
  const wantsInline = !!options.inline;
  let wrapped = content;
  // If content already contains math delimiters, do not double-wrap.
  if (!/\$/.test(content)) {
    if (wantsInline) wrapped = `$${content}$`;
    else {
      // if contains \frac or \sqrt or large operators, wrap in display math for better rendering
      if (/\\frac|\\sqrt|\\times|\\div|\^/.test(content)) wrapped = `$$${content}$$`;
      else wrapped = content;
    }
  }

  // set into container safely
  container.innerHTML = wrapped;

  // debounce typeset calls for performance
  if (typesetTimer) clearTimeout(typesetTimer);
  return new Promise((resolve) => {
    typesetTimer = setTimeout(async () => {
      typesetTimer = null;
      const ok = await ensureMathJax();
      if (ok && window.MathJax && window.MathJax.typesetPromise) {
        try {
          await window.MathJax.typesetPromise([container]);
          resolve(true);
        } catch (e) {
          console.warn('MathJax.typesetPromise error', e);
          resolve(false);
        }
      } else resolve(false);
    }, options.debounce || 80);
  });
}

// Expose helper to convert inline text to LaTeX string without rendering
export function toLatex(text) { return convertSymbolsToLatex(text); }
