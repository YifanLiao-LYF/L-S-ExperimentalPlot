// app_state.js
// 单例共享的 GameStateManager 实例（应用全局状态）
import { GameStateManager } from './game_state_manager.js';

// 使用一个固定的 storage key，便于未来版本管理/迁移
const state = new GameStateManager('nm_game_state_v1');

export { state };