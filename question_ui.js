// question_ui.js
// 完整答题界面与交互逻辑：渲染题目（MathJax）、答案输入、提交与验证、计时、得分、错题记录、动画过渡

// Version log to help debugging caching issues
console.info('[question_ui] version: v3 loaded');

import { calculate } from './calculator.js';
import { state } from './app_state.js';
import { setAndRender, ensureMathJax, toLatex } from './math_render.js';

// Configuration
const QUESTION_TIME_LIMIT = 30; // seconds per question default
const SCORE_PER_QUESTION = 20;

// Helpers: normalize answers (fractions, decimals)
function normalizeAnswer(ans) {
  if (ans === null || ans === undefined) return null;
  ans = String(ans).trim();
  // try fraction like a/b
  const fracMatch = ans.match(/^(-?\d+)\s*\/\s*(\d+)$/);
  if (fracMatch) {
    const a = parseInt(fracMatch[1], 10); const b = parseInt(fracMatch[2], 10);
    if (b === 0) return null;
    return (a / b).toString();
  }
  // try pure number
  const num = Number(ans);
  if (!Number.isNaN(num)) return num.toString();
  // else return trimmed string
  return ans;
}

function answersEqual(a, b) {
  const na = normalizeAnswer(a);
  const nb = normalizeAnswer(b);
  if (na === null || nb === null) return false;
  // numeric comparison with tolerance
  const naNum = Number(na); const nbNum = Number(nb);
  if (!Number.isNaN(naNum) && !Number.isNaN(nbNum)) {
    return Math.abs(naNum - nbNum) < 1e-8;
  }
  return na === nb;
}

// Ensure MathJax is available early to avoid first-render delays
ensureMathJax().then(ok => { if (!ok) console.warn('MathJax may not be available, formula rendering degraded.'); });

// UI Elements (lazy-initialized after DOM ready)
let metaEl = null;
let textEl = null;
let progressFill = null;
let progressMeta = null;
let form = null;
let input = null;
let submitBtn = null;
let giveupBtn = null;
let resultCorrect = null;
let resultWrong = null;
 

function initDOMElements() {
  if (metaEl) return; // already initialized
  metaEl = document.querySelector('.question-meta');
  textEl = document.querySelector('.question-text');
  progressFill = document.querySelector('.progress-fill');
  progressMeta = document.querySelector('.progress-box .meta');
  form = document.querySelector('.answer-form');
  input = document.getElementById('answer-input');
  resultCorrect = document.querySelector('.result.correct');
  resultWrong = document.querySelector('.result.wrong');
  if (form) {
    submitBtn = form.querySelector('.btn-primary');
    giveupBtn = form.querySelector('.btn-outline');
  }
}

let currentQuestion = null;
let timer = null;
let timeLeft = QUESTION_TIME_LIMIT;

function resetQuestionUI() {
  resultCorrect.hidden = true; resultWrong.hidden = true;
  // remove any explanation nodes
  const old = document.querySelectorAll('.explain');
  old.forEach(n => n.remove());
}

function startTimer(sec = QUESTION_TIME_LIMIT, onExpire) {
  clearTimer();
  timeLeft = sec;
  const countdown = document.createElement('div'); countdown.className = 'countdown'; countdown.id = 'question-countdown';
  countdown.textContent = `剩余: ${timeLeft}s`;
  const sidebar = document.querySelector('.question-sidebar .progress-box');
  if (sidebar) sidebar.appendChild(countdown);
  timer = setInterval(() => {
    timeLeft -= 1; if (timeLeft < 0) { clearTimer(); if (onExpire) onExpire(); return; }
    if (countdown) countdown.textContent = `剩余: ${timeLeft}s`;
  }, 1000);
}
function clearTimer() {
  if (timer) clearInterval(timer); timer = null;
  const cd = document.getElementById('question-countdown'); if (cd) cd.remove();
}

// Load and display next question
export function loadNextQuestion() {
  initDOMElements();
  console.log('[question_ui] loadNextQuestion START - currentLevelId=', state.currentLevelId, 'currentScoreThisLevel=', state.currentScoreThisLevel);
  resetQuestionUI();
  clearTimer();
  const q = state.quizManager.getNextQuestion();
  currentQuestion = q;
  console.debug('[question_ui] loadNextQuestion -> index', state.quizManager.currentQuestionIndex, 'q.id', q && q.id);
  if (!q || q.id === -1) {
    metaEl.textContent = '已完成本关所有题目';
    textEl.textContent = '恭喜你完成了本关卡！即将返回关卡选择界面...';
    progressFill.style.width = '100%';
    progressMeta.textContent = '已完成';
    // finalize level: compute final score and stars from accumulated currentScoreThisLevel
    try {
      const levelId = state.currentLevelId;
  // allow future loads
      const finalScore = state.currentScoreThisLevel || 0;
      // map score to stars using explicit thresholds so 100 -> 3 stars
      // 90-100: 3 stars, 70-89: 2 stars, 50-69: 1 star, <50: 0 star
      const stars = finalScore >= 90 ? 3 : finalScore >= 70 ? 2 : finalScore >= 50 ? 1 : 0;
      console.log('[question_ui] Finalizing level', levelId, 'finalScore=', finalScore, 'computedStars=', stars);
      const ok = state.completeLevel(levelId, finalScore, stars);
      if (!ok) console.warn('completeLevel returned false for level', levelId);
      // reset currentScoreThisLevel for safety and ensure saved state / UI reflect this
      state.currentScoreThisLevel = 0;
      try {
        console.log('[question_ui] Saving state after level completion');
        if (typeof state.saveState === 'function') state.saveState();
        if (state.events) state.events.dispatchEvent(new CustomEvent('statechange', { detail: state.getStateSnapshot && state.getStateSnapshot() }));
        console.log('[question_ui] Dispatched statechange after completion');
      } catch (e) {
        console.warn('Failed to persist/dispatch state after level completion:', e);
      }
      
      // 在关卡完成后延迟2秒返回关卡选择界面（带回退）
      setTimeout(() => {
        if (window.show_level_selection) {
          try { window.show_level_selection(); return; } catch (e) { console.warn('show_level_selection threw', e); }
        }
        // fallback: set hash to levels so router will show level selection
        try { location.hash = '#levels'; } catch (e) { console.warn('fallback navigation failed', e); }
      }, 2000);
    } catch (e) {
      console.error('Error finalizing level:', e);
    }
    return;
  }
  console.log('[question_ui] loadNextQuestion DONE - loaded q.id=', q && q.id);

  const total = state.quizManager.questionBank.length || 5;
  const idx = state.quizManager.currentQuestionIndex;
  metaEl.textContent = `题目 ${idx} / ${total}`;
  // allow math expressions in content (use $$...$$ for MathJax)
  // convert special symbols and render via MathJax (debounced inside setAndRender)
  // prefer showing LaTeX for fractions/root/exponents
  setAndRender(textEl, q.content, { inline: false, debounce: 60 });
  progressFill.style.width = `${Math.round((idx / total) * 100)}%`;
  progressMeta.textContent = `题目 ${idx} / ${total}  •  得分 ${state.currentScoreThisLevel ?? 0}`;

  input.value = '';
  startTimer(QUESTION_TIME_LIMIT, () => {
    // auto mark wrong and move on
    handleAnswerSubmission('', true);
  });
}

function handleAnswerSubmission(userInput, expired = false) {
  initDOMElements();
  if (!currentQuestion) {
    console.debug('[question_ui] submit ignored: no currentQuestion');
    return;
  }
  clearTimer();
  // prevent re-entrancy: immediately remove reference
  const answeringQuestion = currentQuestion;
  currentQuestion = null;
  // disable input/buttons to avoid duplicate submits
  try {
    if (form) {
      const btn = form.querySelector('.btn-primary'); if (btn) btn.disabled = true;
      const gbtn = form.querySelector('.btn-outline'); if (gbtn) gbtn.disabled = true;
    }
  } catch (e) { /* ignore */ }
    console.debug('[question_ui] handleAnswerSubmission for question id', answeringQuestion && answeringQuestion.id, 'expired=', expired);
    const correct = !expired && answersEqual(userInput, answeringQuestion.answer);
  if (correct) {
    resultCorrect.hidden = false; resultCorrect.classList.add('fade-in','pulse');
    state.completeQuestion(true, answeringQuestion, userInput, SCORE_PER_QUESTION);
  } else {
    resultWrong.hidden = false; resultWrong.classList.add('fade-in','pulse');
    resultWrong.textContent = expired ? `超时！正确答案：${answeringQuestion.answer}` : `❌ 错误！正确答案：${answeringQuestion.answer}`;
    state.completeQuestion(false, answeringQuestion, userInput, 0);
  }
  // show explanation
  if (answeringQuestion && answeringQuestion.explanation) {
    const p = document.createElement('p'); p.className = 'explain muted fade-in'; p.textContent = `解析：${answeringQuestion.explanation}`;
    form.parentNode.appendChild(p);
  }
  // auto-next after short delay
  setTimeout(() => {
    // remove animation classes
    resultCorrect.classList.remove('pulse'); resultWrong.classList.remove('pulse');
    // re-enable buttons before loading next
    try {
      if (form) {
        const btn = form.querySelector('.btn-primary'); if (btn) btn.disabled = false;
        const gbtn = form.querySelector('.btn-outline'); if (gbtn) gbtn.disabled = false;
      }
    } catch (e) { /* ignore */ }
    
    // 删除导致跳转回关卡选择的代码
    // 用户将继续答题直到完成整个关卡
    
    loadNextQuestion();
  }, 900);
}

// Events will be bound after DOMContentLoaded to ensure elements exist

// Expose for external control
window.loadNextQuestion = loadNextQuestion;
window.handleAnswerSubmission = handleAnswerSubmission;

// When script loads, initialize first question if in question view
document.addEventListener('DOMContentLoaded', () => {
  // ensure MathJax config exists (if not, user will include MathJax script in index.html)
  if (!window.MathJax) console.warn('MathJax 未检测到：公式渲染需要在 index.html 中加入 MathJax 或 KaTeX');
  // initialize if the question panel is active
  // initialize DOM refs and bind events
  initDOMElements();
  if (form) {
    form.addEventListener('submit', (e) => {
      e.preventDefault();
      const val = input.value.trim();
      handleAnswerSubmission(val, false);
    });
    if (giveupBtn) giveupBtn.addEventListener('click', (e) => { e.preventDefault(); handleAnswerSubmission('', false); });
  }
  if (document.getElementById('question')?.classList.contains('active')) {
    loadNextQuestion();
  }
});