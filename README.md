# 数感闯关 — 工程说明

这是一个基于浏览器的轻量化项目，用于展示“数感闯关”式的练习与测评界面。项目前端与已有的 C++ 后端实现（仓库中 `src/`、`include/`）在逻辑上保持一致，前端代码将关卡、题库、错题管理等功能以现代浏览器模块化 JavaScript 的方式实现。

本文档覆盖：项目结构、依赖与运行、开发与调试指南、数据存储与重置、已实现的重要功能、常见问题排查以及贡献说明。

---

## 一、仓库概览

主要文件/目录：

- `index.html` — 单页应用入口（ES modules）
- `styles.css` — 全局样式
- `app.js` — 应用入口，初始化全局单例 `state`、路由与 UI 模块
- `app_state.js`、`game_state_manager.js` — 全局状态管理，负责将各管理器（`LevelManager`、`QuizManager`、`MistakeManager`）与 `StorageManager` 结合并持久化
- `level_manager.js` — 关卡数据/逻辑（移植自 C++）
- `quiz_manager.js`、`question_ui.js` — 题库与答题界面
- `mistake_manager.js` — 浏览器端错题管理（支持增、删、序列化/反序列化）
- `ui_controller.js`、`level_ui.js` — 页面视图控制与渲染协作
- `storage_manager.js` — localStorage 封装（含内存回退）
- `static_server.js` — 项目里新增的本地静态服务器脚本（用于正确返回 MIME 类型）
- `scripts/` 与 `src/`、`include/` — 辅助脚本和 C++ 源（保留供参考/对比）

注：项目使用原生 ES module（`<script type="module">`），因此要求静态服务器正确返回 `.js` 的 `Content-Type: text/javascript`，否则浏览器会拒绝作为模块加载。

---

## 二、快速开始（在 Windows PowerShell）

1. 打开 PowerShell，定位到本项目根目录：

```powershell
cd C:\Users\lenovo\Desktop\SL
```

2. 启动本地静态服务器（推荐，已包含对 MIME 的正确映射）：

```powershell
node static_server.js
```

会输出：
```
STATIC SERVER <project-path> -> http://localhost:8080
```

3. 在浏览器打开：

```
http://localhost:8080
```

4. 在页面中正常使用：切换“开始闯关”/“专项练习”/“错题集”/“闯关进度”。

---

## 三、数据存储与重置

前端状态（关卡、分数、错题）由 `GameStateManager` 管理并通过 `StorageManager` 写入 `localStorage`。默认 storage key：

- 已实例化时传入的 key：`nm_game_state_v1`（见 `app_state.js`）

重置/清空数据的两种方式：

A) 在浏览器控制台执行（快速、一次性）

```javascript
// 在浏览器 DevTools Console 中执行
if (window.state && typeof window.state.resetState === 'function') {
  window.state.resetState();
  window.state.saveState && window.state.saveState();
  location.reload();
} else {
  console.warn('window.state.resetState 不可用 — 确保 app 已加载');
}
```

B) 或者直接清空 localStorage（更暴力）

```javascript
localStorage.clear();
location.reload();
```

> 注意：以上操作会永久删除本地的数据，请谨慎执行。如果需要备份，可先在 Console 输出并保存当前状态：
>
> ```javascript
> console.log(JSON.stringify(localStorage.getItem('nm_game_state_v1')));
> ```

---

## 四、已实现的主要前端功能（摘要）

- 关卡管理（`LevelManager`）
  - 初始化 16 个关卡，包含 `id`、`title`、`description`、`status`、`score`、`stars`、`requiredScore` 与 `unlockRule`
  - 完成关卡时更新得分、星级，并尝试解锁下一关

- 题库与答题（`QuizManager` / `question_ui.js`）
  - 按关卡加载题目、检查答案、显示解析、完成题目后调用 `GameStateManager.completeQuestion()`

- 错题管理（`mistake_manager.js` / `ui_controller.js`）
  - 记录错题包含题目对象、用户答案与时间戳
  - 支持：添加(`addMistake`)、导出 JSON(`toJSON`)、从 JSON 导入(`fromJSON`)、清空(`clear`)、单项删除(`deleteMistake(index)`)
  - UI：在“错题集”页面以平铺列表显示每条错题并提供“删除”按钮（删除会触发 `state.saveState()` 持久化）

- 存储适配（`storage_manager.js`）
  - 封装 localStorage 使用，并在无 localStorage 环境下使用内存回退

- 开发辅助
  - `static_server.js`：小型静态文件服务器，正确设置常见文件扩展名的 Content-Type（解决浏览器模块加载时出现的 MIME 错误）

---

## 五、开发与调试建议

- 模块加载错误（典型）：
  - 错误举例：`Failed to load module script: Expected a JavaScript module script but the server responded with a MIME type of 'text/plain'`。
  - 排查：Network 面板 -> 找到 `.js` 请求 -> 检查 Response Header 中 `Content-Type`。
  - 解决：使用 `node static_server.js` 启动的服务器会返回正确的 MIME。或者在你自己的服务器上添加正确的 MIME 映射（`.js` -> `text/javascript`）。

- 检查/打印当前状态：
```javascript
// 在 Console 中
console.log(window.state ? window.state.getStateSnapshot() : 'state not available');
```

- 若希望自动化测试（如使用 Puppeteer 进行功能回归），建议新增 `package.json` scripts 并将测试依赖（puppeteer）加入；当前仓库未包含自动化测试脚本。

---

## 六、如何扩展或集成后端（说明性）

- 项目保留有 C++ 源代码（`src/`、`include/`），前端行为与部分逻辑参考了原 C++ 的实现（例如 `level_manager.cpp`、`menu.cpp`）。若要与真正的后端服务集成：
  - 需要将题库、关卡与错题存储改为通过 HTTP API 拉取/提交。
  - `GameStateManager` 可以扩展 `saveState()`/`loadState()` 调用远程 API 并做冲突/版本处理。

---

## 七、常见问题 (FAQ)

Q: 打开页面出现白屏或控制台模块加载错误？
- A: 请先确认你是通过 HTTP(S) 以模块方式加载页面（`http://localhost:8080`），不要直接用 `file://` 打开 `index.html`。如果自己搭静态服务器，确保 `.js` 文件返回 `Content-Type: text/javascript`。

Q: 我不小心删除了错题或清空了数据，可以恢复吗？
- A: localStorage 操作是本地且不可撤销的（除非你有事先备份），建议在执行 `resetState()` 等操作前导出当前 `nm_game_state_v1` 的值备份。

---

## 八、贡献与代码规范

- 代码使用原生 JavaScript（ES6 modules）与轻量 CSS。请保持模块化原则、避免在模块中创建全局副作用。
- 变更前请在本地运行 `static_server.js` 并在浏览器中手动验证主流程（关卡页面、答题流程、错题添加/删除、localStorage 持久化）。
- 贡献流程建议：
  1. Fork 仓库并创建 feature 分支
  2. 运行并手动验证你的修改
  3. 提交清晰的变更说明和必要的复现步骤

---

最后更新：2025-10-21



