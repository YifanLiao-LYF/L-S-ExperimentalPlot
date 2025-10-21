// ui_controller.js
// 基于 C++ menu.cpp 的界面控制模块
// 提供：show_main_menu, show_level_selection, play_level, show_mistakes, practice_mode

import { state } from './app_state.js';
import './question_ui.js';
import * as LevelUI from './level_ui.js';

 

// Simple view management: show one .view panel at a time
function showView(viewId) {
  document.querySelectorAll('.view').forEach(v => v.classList.remove('active'));
  const target = document.getElementById(viewId);
  if (target) target.classList.add('active');
}

// Main menu show (maps to show_main_menu)
export function show_main_menu() {
  showView('levels');
}

// Level selection with pagination
const LEVELS_PER_PAGE = 8;
let currentPage = 1;
function renderLevelGrid() {
  const grid = document.querySelector('.level-grid');
  if (!grid) return;
  // clear existing cards
  grid.innerHTML = '';
  const levels = state.levelManager.getAllLevels();
  const totalPages = Math.ceil(levels.length / LEVELS_PER_PAGE) || 1;
  if (currentPage > totalPages) currentPage = totalPages;
  const start = (currentPage - 1) * LEVELS_PER_PAGE;
  const end = Math.min(start + LEVELS_PER_PAGE, levels.length);

  for (let i = start; i < end; i++) {
    const lvl = levels[i];
    const article = document.createElement('article');
    article.className = 'level-card';
    article.dataset.levelId = String(lvl.id);
    article.dataset.status = lvl.status === undefined ? 'locked' : lvl.status.toLowerCase();

    const top = document.createElement('div'); top.className = 'level-top';
    top.innerHTML = `<span class="level-number">${lvl.id}</span><span class="level-title">${lvl.title}</span>`;

    const body = document.createElement('div'); body.className = 'level-body';
    body.innerHTML = `<p class="level-desc">${lvl.description}</p>`;

    const footer = document.createElement('div'); footer.className = 'level-footer';

    const statusTag = document.createElement('span'); statusTag.className = 'status-tag';
    if (lvl.status === 'LEVEL_UNLOCKED') {
      article.classList.add('unlocked'); statusTag.classList.add('green'); statusTag.textContent = '可挑战';
      const btn = document.createElement('button'); btn.className = 'btn btn-primary btn-sm'; btn.textContent = '开始';
      btn.addEventListener('click', () => play_level(lvl.id));
      footer.appendChild(statusTag); footer.appendChild(btn);
    } else if (lvl.status === 'LEVEL_COMPLETED') {
      article.classList.add('completed'); statusTag.classList.add('yellow'); statusTag.textContent = `已完成：${lvl.score} 分`;
      const btn = document.createElement('button'); btn.className = 'btn btn-outline btn-sm'; btn.textContent = '再来一遍';
      btn.addEventListener('click', () => play_level(lvl.id));
      const stars = document.createElement('div'); stars.className = 'stars'; stars.textContent = '★'.repeat(lvl.stars) + '☆'.repeat(Math.max(0, 3 - lvl.stars));
      footer.appendChild(statusTag); footer.appendChild(stars); footer.appendChild(btn);
    } else {
      article.classList.add('locked'); statusTag.classList.add('red'); statusTag.textContent = `锁定：需 ${lvl.requiredScore} 分`;
      const btn = document.createElement('button'); btn.className = 'btn btn-muted btn-sm'; btn.disabled = true; btn.textContent = '锁定';
      footer.appendChild(statusTag); footer.appendChild(btn);
    }

    article.appendChild(top); article.appendChild(body); article.appendChild(footer);
    grid.appendChild(article);
  }

  // update pagination buttons
  const prevBtn = document.querySelector('.panel-actions .btn:nth-child(1)');
  const nextBtn = document.querySelector('.panel-actions .btn:nth-child(2)');
  if (prevBtn && nextBtn) {
    prevBtn.disabled = currentPage <= 1;
    nextBtn.disabled = currentPage >= totalPages;
    prevBtn.onclick = () => { currentPage = Math.max(1, currentPage - 1); renderLevelGrid(); };
    nextBtn.onclick = () => { currentPage = Math.min(totalPages, currentPage + 1); renderLevelGrid(); };
  }
}

export function show_level_selection() {
  showView('levels');
  // delegate to level_ui
  LevelUI.renderLevelGrid();
}

// Play level: initializes quiz and shows question view
export function play_level(levelId) {
  const started = state.startLevel(levelId);
  if (!started) {
    alert('无法开始此关卡（可能被锁定）');
    return;
  }
  // show question view
  showView('question');
  // reset question UI and load first question
  // question_ui exposes loadNextQuestion on window
  if (window.loadNextQuestion) window.loadNextQuestion();
}

function renderQuestion() {
  const meta = document.querySelector('.question-meta');
  const text = document.querySelector('.question-text');
  const progressFill = document.querySelector('.progress-fill');
  const progressMeta = document.querySelector('.progress-box .meta');
  const q = state.quizManager.getNextQuestion();
  if (!q || q.id === -1) {
    meta.textContent = '题目 0 / 0';
    text.textContent = '本关所有题目已完成';
    progressFill.style.width = '100%';
    progressMeta.textContent = '已完成';
    return;
  }
  // For simplicity assume 5 questions per level in UI; if quiz has different length adapt
  const total = state.quizManager.questionBank.length || 5;
  const idx = state.quizManager.currentQuestionIndex; // after getNextQuestion it advanced
  meta.textContent = `题目 ${idx} / ${total}`;
  text.textContent = q.content;
  progressFill.style.width = `${Math.round((idx / total) * 100)}%`;
  progressMeta.textContent = `题目 ${idx} / ${total}  •  得分 ${state.currentScoreThisLevel ?? 0}`;

  // wire form
  const form = document.querySelector('.answer-form');
  const input = document.getElementById('answer-input');
  const submitBtn = form.querySelector('.btn-primary');
  const giveupBtn = form.querySelector('.btn-outline');
  const resultCorrect = document.querySelector('.result.correct');
  const resultWrong = document.querySelector('.result.wrong');
  resultCorrect.hidden = true; resultWrong.hidden = true;

  function submitHandler(e) {
    e.preventDefault();
    const val = input.value.trim();
    const ok = state.quizManager.checkAnswer(val);
    if (ok) {
      resultCorrect.hidden = false;
      resultCorrect.textContent = '✅ 正确！';
      state.completeQuestion(true, q, val, 20);
    } else {
      resultWrong.hidden = false;
      resultWrong.textContent = `❌ 错误！正确答案：${q.answer}`;
      state.completeQuestion(false, q, val, 0);
    }
    // show explanation (if any)
    const explain = state.quizManager.getExplanation();
    if (explain) {
      const p = document.createElement('p'); p.className = 'explain muted'; p.textContent = `解析：${explain}`; 
      form.parentNode.appendChild(p);
    }
    // load next after short delay
    setTimeout(() => {
      renderQuestion();
    }, 900);
  }

  function giveupHandler(e) {
    e.preventDefault();
    const explain = state.quizManager.getExplanation();
    resultWrong.hidden = false;
    resultWrong.textContent = `放弃：正确答案 ${q.answer}`;
    const p = document.createElement('p'); p.className = 'explain muted'; p.textContent = `解析：${explain}`;
    form.parentNode.appendChild(p);
    state.completeQuestion(false, q, '', 0);
    setTimeout(() => renderQuestion(), 900);
  }

  // detach previous handlers
  form.onsubmit = submitHandler;
  submitBtn.onclick = submitHandler;
  giveupBtn.onclick = giveupHandler;
}

// Show mistakes
export function show_mistakes() {
  showView('mistakes');
  const list = document.querySelector('.mistake-list');
  list.innerHTML = '';
  const arr = state.mistakeManager.getMistakes();
  if (!arr || arr.length === 0) {
    list.innerHTML = '<p class="muted">暂无错题，继续练习吧！</p>';
    return;
  }
  const levels = state.levelManager.getAllLevels();
  const getLevelTitle = (id) => {
    const lv = levels.find(l => l.id === id);
    return lv ? `${lv.title}` : `关卡 ${id}`;
  };

  arr.forEach((m, i) => {
    const art = document.createElement('article'); art.className = 'mistake-item';
    art.innerHTML = `<div class="mistake-head"><span class="mistake-number">错题 ${i+1}</span></div>`;
    const body = document.createElement('div'); body.className = 'mistake-body';
    body.innerHTML = `<div class="mistake-question">题目：${m.question.content}</div><div class="mistake-your">你的答案：${m.userAnswer}</div><div class="mistake-correct">正确答案：${m.question.answer}</div><div class="mistake-explain">解析：${m.question.explanation}</div>`;
    const delBtn = document.createElement('button'); delBtn.className = 'btn btn-danger btn-sm mistake-delete-item'; delBtn.textContent = '删除';
    delBtn.addEventListener('click', () => {
      if (!confirm('确定要删除此错题吗？')) return;
      const ok = state.mistakeManager.deleteMistake(i);
      if (ok) {
        state.saveState();
        show_mistakes();
      } else alert('删除失败');
    });
    const foot = document.createElement('div'); foot.className = 'mistake-footer';
    foot.appendChild(delBtn);
    art.appendChild(body);
    art.appendChild(foot);
    list.appendChild(art);
  });
}

// Practice mode (calculator)
export function practice_mode() {
  showView('question');
  // Replace main area with calculator UI (simple): reuse question view area but indicate practice
  const title = document.getElementById('question-title');
  if (title) title.textContent = '专项练习 — 计算器模式';
  const qtext = document.querySelector('.question-text');
  qtext.innerHTML = '<div class="calculator-prompt">计算器已就绪，可输入表达式进行计算（示例： (3+4)*2/7 ）</div>';
  // wire calculator evaluation to the submit button
  const form = document.querySelector('.answer-form');
  const input = document.getElementById('answer-input');
  form.onsubmit = (e) => { e.preventDefault(); try { const out = window.calculate ? window.calculate(input.value) : '未加载 calculate'; const res = document.querySelector('.result.correct'); res.hidden = false; res.textContent = `计算结果：${out}`; } catch (err) { const wr = document.querySelector('.result.wrong'); wr.hidden = false; wr.textContent = `计算错误：${err}`; } };
}

// Wire top navigation buttons
function wireNav() {
  document.querySelectorAll('.main-nav .nav-btn').forEach(btn => {
    btn.addEventListener('click', () => {
      const view = btn.dataset.view;
      if (view === 'levels') show_level_selection();
      else if (view === 'practice') practice_mode();
      else if (view === 'mistakes') show_mistakes();
      else if (view === 'progress') { showView('progress'); LevelUI.renderProgressView(); }
    });
  });
}

// On DOM ready
document.addEventListener('DOMContentLoaded', () => {
  wireNav();
  // initialize level UI module and default view
  LevelUI.init();
  show_level_selection();
});

// Expose for console debugging like C++ names
window.show_main_menu = show_main_menu;
window.show_level_selection = show_level_selection;
window.play_level = play_level;
window.show_mistakes = show_mistakes;
window.practice_mode = practice_mode;
