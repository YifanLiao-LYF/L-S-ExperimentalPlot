// storage_manager.js
// 使用 localStorage 保存与加载游戏数据（如果在 Node 环境中，没有 localStorage，则提供内存回退）

const STORAGE_KEY = 'math_game_state_v1';

function hasLocalStorage() {
  try {
    return typeof window !== 'undefined' && typeof window.localStorage !== 'undefined';
  } catch (e) {
    return false;
  }
}

export class StorageManager {
  constructor(storageKey = STORAGE_KEY) {
    this.storageKey = storageKey;
    this.fallback = {}; // 内存回退
    this.useLocal = hasLocalStorage();
  }

  save(obj) {
    try {
      const json = JSON.stringify(obj);
      if (this.useLocal) {
        window.localStorage.setItem(this.storageKey, json);
      } else {
        this.fallback[this.storageKey] = json;
      }
      return true;
    } catch (e) {
      console.error('StorageManager.save error:', e);
      return false;
    }
  }

  load() {
    try {
      let json = null;
      if (this.useLocal) {
        json = window.localStorage.getItem(this.storageKey);
      } else {
        json = this.fallback[this.storageKey] ?? null;
      }
      if (!json) return null;
      return JSON.parse(json);
    } catch (e) {
      console.error('StorageManager.load error:', e);
      return null;
    }
  }

  clear() {
    try {
      if (this.useLocal) window.localStorage.removeItem(this.storageKey);
      else delete this.fallback[this.storageKey];
      return true;
    } catch (e) {
      console.error('StorageManager.clear error:', e);
      return false;
    }
  }
}
