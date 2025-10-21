// level_ui.js
// Renders level selection grid and progress/statistics view.
// Binds to GameStateManager events to update in real-time.

import { state } from './app_state.js';
import { toLatex } from './math_render.js';

const LEVELS_PER_PAGE = 8;
let currentPage = 1;

function formatStars(n) {
  if (!n || n <= 0) return '☆ ☆ ☆';
  const filled = '★'.repeat(Math.max(0, Math.min(3, n))).split('').join(' ');
  const empty = '☆'.repeat(Math.max(0, 3 - n)).split('').join(' ');
  return `${filled}${filled && empty ? ' ' : ''}${empty}`.trim();
}

function renderLevelGrid(container = document.querySelector('.level-grid')) {
  if (!container) return;
  container.innerHTML = '';
  const levels = state.levelManager.getAllLevels();
  const totalPages = Math.max(1, Math.ceil(levels.length / LEVELS_PER_PAGE));
  if (currentPage > totalPages) currentPage = totalPages;
  const start = (currentPage - 1) * LEVELS_PER_PAGE;
  const end = Math.min(start + LEVELS_PER_PAGE, levels.length);

  for (let i = start; i < end; i++) {
    const lvl = levels[i];
    const article = document.createElement('article');
    article.className = 'level-card';
    article.dataset.levelId = String(lvl.id);
    article.dataset.status = (lvl.status || 'LEVEL_LOCKED').toLowerCase();
    article.setAttribute('role', 'listitem');

    const top = document.createElement('div'); top.className = 'level-top';
    top.innerHTML = `<span class="level-number">${lvl.id}</span><span class="level-title">${lvl.title}</span>`;

  const body = document.createElement('div'); body.className = 'level-body';
  // convert symbols for display; avoid heavy MathJax typesetting for every card
  body.innerHTML = `<p class="level-desc">${toLatex(lvl.description)}</p>`;

    const footer = document.createElement('div'); footer.className = 'level-footer';

    const statusTag = document.createElement('span'); statusTag.className = 'status-tag';

    if (lvl.status === 'LEVEL_UNLOCKED') {
      article.classList.add('unlocked');
      statusTag.classList.add('green'); statusTag.textContent = '可挑战';
      const btn = document.createElement('button'); btn.className = 'btn btn-primary btn-sm'; btn.textContent = '开始';
      btn.addEventListener('click', () => {
        // delegate to ui_controller.play_level via global
        if (window.play_level) window.play_level(lvl.id);
      });
      footer.appendChild(statusTag); footer.appendChild(btn);
    } else if (lvl.status === 'LEVEL_COMPLETED') {
      article.classList.add('completed');
      statusTag.classList.add('yellow'); statusTag.textContent = `已完成：${lvl.score} 分`;
      const btn = document.createElement('button'); btn.className = 'btn btn-outline btn-sm'; btn.textContent = '再来一遍';
      btn.addEventListener('click', () => { if (window.play_level) window.play_level(lvl.id); });
      const stars = document.createElement('div'); stars.className = 'stars'; stars.textContent = formatStars(lvl.stars);
      footer.appendChild(statusTag); footer.appendChild(stars); footer.appendChild(btn);
    } else {
      article.classList.add('locked');
      statusTag.classList.add('red');
      // show unlock rule text if available
      if (lvl.unlockRule && lvl.unlockRule.type === 'prevStars') {
        statusTag.textContent = `锁定：需上一关获得 ${lvl.unlockRule.value} 星`;
      } else if (lvl.unlockRule && lvl.unlockRule.type === 'prevScore') {
        statusTag.textContent = `锁定：需上一关得分 ${lvl.unlockRule.value} 分`;
      } else {
        statusTag.textContent = `锁定：需 ${lvl.requiredScore} 分`;
      }
      const btn = document.createElement('button'); btn.className = 'btn btn-muted btn-sm'; btn.disabled = true; btn.textContent = '锁定';
      footer.appendChild(statusTag); footer.appendChild(btn);
    }

    article.appendChild(top); article.appendChild(body); article.appendChild(footer);
    container.appendChild(article);
  }

  // pagination controls
  const prevBtn = document.querySelector('.panel-actions .btn:nth-child(1)');
  const nextBtn = document.querySelector('.panel-actions .btn:nth-child(2)');
  if (prevBtn && nextBtn) {
    prevBtn.disabled = currentPage <= 1;
    nextBtn.disabled = currentPage >= totalPages;
    prevBtn.onclick = () => { currentPage = Math.max(1, currentPage - 1); renderLevelGrid(); };
    nextBtn.onclick = () => { currentPage = Math.min(totalPages, currentPage + 1); renderLevelGrid(); };
  }
}

function renderProgressView() {
  // update stat cards
  const snap = state.getStateSnapshot();
  // update stat cards: prefer scoped #progress selectors but fall back to any .stat-card in document
  let statCards = Array.from(document.querySelectorAll('#progress .stat-card .stat-value'));
  if (!statCards || statCards.length < 3) {
    statCards = Array.from(document.querySelectorAll('.stat-card .stat-value'));
  }
  // assume order: 总分, 已完成关卡, 总星数
  if (statCards && statCards.length >= 3) {
    statCards[0].textContent = String(snap.totalScore || 0);
    const completed = snap.levels.filter(l => l.status === 'LEVEL_COMPLETED').length;
    statCards[1].textContent = `${completed} / ${snap.levels.length}`;
    const totalStars = snap.levels.reduce((s, l) => s + (l.stars || 0), 0);
    statCards[2].textContent = `${totalStars} / ${snap.levels.length * 3}`;
  } else {
    console.warn('renderProgressView: stat-card elements not found or incomplete');
  }

  // populate table
  const tbody = document.querySelector('.progress-table tbody');
  if (!tbody) {
    console.warn('renderProgressView: .progress-table tbody not found');
  } else {
    tbody.innerHTML = '';
    for (const l of snap.levels) {
      const tr = document.createElement('tr');
      const statusText = l.status === 'LEVEL_LOCKED' ? '锁定' : (l.status === 'LEVEL_UNLOCKED' ? '未完成' : '已完成');
      const starsText = l.stars > 0 ? String(l.stars) : '—';
      const scoreText = l.status === 'LEVEL_COMPLETED' ? String(l.score) : '—';
      let unlockReq = '—';
      if (l.status === 'LEVEL_LOCKED') {
        if (l.unlockRule && l.unlockRule.type === 'prevStars') unlockReq = `需上一关 ${l.unlockRule.value} 星`;
        else if (l.unlockRule && l.unlockRule.type === 'prevScore') unlockReq = `需上一关 ${l.unlockRule.value} 分`;
        else unlockReq = `${l.requiredScore} 分`;
      }
      tr.innerHTML = `<td>${l.id}</td><td>${l.title}</td><td>${statusText}</td><td>${starsText}</td><td>${scoreText}</td><td>${unlockReq}</td>`;
      tbody.appendChild(tr);
    }
  }

  // progress bar overall (percentage of completed levels)
  const completedCount = snap.levels.filter(l => l.status === 'LEVEL_COMPLETED').length;
  const pct = Math.round((completedCount / snap.levels.length) * 100);
  let overallBar = document.querySelector('#progress .overall-progress .progress-fill');
  if (!overallBar) {
    // create simple overall progress below stats
    const wrap = document.querySelector('#progress .stats-grid');
    if (wrap) {
      const node = document.createElement('div'); node.className = 'overall-progress';
      node.innerHTML = `<h4>闯关完成进度</h4><div class="progress-line"><div class="progress-fill" style="width:${pct}%"></div></div><div class="meta">${pct}% 完成 (${completedCount}/${snap.levels.length} 关)</div>`;
      wrap.parentNode.insertBefore(node, wrap.nextSibling);
    }
  } else {
    overallBar.style.width = `${pct}%`;
    const meta = document.querySelector('#progress .overall-progress .meta');
    if (meta) meta.textContent = `${pct}% 完成 (${completedCount}/${snap.levels.length} 关)`;
  }
}

function init() {
  // initial render
  renderLevelGrid();
  renderProgressView();
  // listen to state changes
  state.events.addEventListener('statechange', () => {
    renderLevelGrid();
    renderProgressView();
  });
}

// expose for ui_controller to call
export { init, renderLevelGrid, renderProgressView };