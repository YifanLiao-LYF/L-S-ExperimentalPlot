// app.js
// 主应用入口：初始化管理器实例、路由、事件绑定、错误边界与自动保存

// Version log to help debugging caching / stale module loads
console.info('[app.js] version: v2 loaded');

import { state } from './app_state.js';
import * as LevelUI from './level_ui.js';
import './ui_controller.js'; // already imports GameStateManager in its own scope
import './question_ui.js';
import { calculate } from './calculator.js';

// expose global managers per requirement (参考 C++ menu.cpp 的全局实例)
window.levelManager = state.levelManager;
window.quizManager = state.quizManager;
window.mistakeManager = state.mistakeManager;
window.calculator = { calculate };

// Simple routing: maps hash to view id
function route() {
  const hash = location.hash.replace('#', '') || 'levels';
  // hide all views then show
  document.querySelectorAll('.view').forEach(v => v.classList.remove('active'));
  const target = document.getElementById(hash);
  if (target) target.classList.add('active');
  // let ui_controller handle initialization side-effects via exported functions
  if (hash === 'levels') LevelUI.renderLevelGrid();
  if (hash === 'progress') LevelUI.renderProgressView();
  if (hash === 'question') {
    if (window.loadNextQuestion) window.loadNextQuestion();
  }
}

// Error boundary: catch unhandled errors and show a user-friendly panel
window.addEventListener('error', (e) => {
  console.error('Unhandled error:', e.error || e.message);
  alert('出现错误：' + (e.error?.message || e.message || '未知错误'));
});
window.addEventListener('unhandledrejection', (e) => {
  console.error('Unhandled promise rejection:', e.reason);
  alert('发生未处理的 Promise 错误：' + (e.reason?.message || e.reason));
});

// Auto-save: throttle save calls
let saveTimeout = null;
state.events.addEventListener('statechange', () => {
  if (saveTimeout) clearTimeout(saveTimeout);
  saveTimeout = setTimeout(() => {
    state.saveState();
  }, 300);
});

// When DOM loaded initialize UI modules and routing
document.addEventListener('DOMContentLoaded', () => {
  try {
    LevelUI.init();
    // ensure route according to hash
    window.addEventListener('hashchange', route);
    route();
  } catch (err) {
    console.error('App init failed:', err);
    alert('初始化失败：' + err.message);
  }
});

export default { state };
