// mistake_manager.js
// 管理错题集，数据结构与 C++ 保持一致（保存 Question 对象和用户答案）

export class MistakeManager {
  constructor() {
    // mistakes: array of { question: {id,type,content,answer,explanation}, userAnswer: string, timestamp: ISO }
    this.mistakes = [];
  }

  addMistake(question, userAnswer) {
    try {
      const entry = {
        question: {
          id: question.id ?? -1,
          type: question.type ?? 'ARITHMETIC',
          content: question.content ?? '',
          answer: question.answer ?? '',
          explanation: question.explanation ?? ''
        },
        userAnswer: String(userAnswer ?? ''),
        timestamp: new Date().toISOString()
      };
      this.mistakes.push(entry);
      return entry;
    } catch (e) {
      console.error('addMistake error:', e);
      return null;
    }
  }

  getMistakes() {
    // return a shallow copy to avoid accidental external mutation
    return this.mistakes.slice();
  }

  clear() {
    this.mistakes = [];
  }

  toJSON() {
    return JSON.parse(JSON.stringify(this.mistakes));
  }

  fromJSON(arr) {
    if (!Array.isArray(arr)) return;
    // Basic validation
    this.mistakes = arr.map(item => ({
      question: {
        id: item?.question?.id ?? -1,
        type: item?.question?.type ?? 'ARITHMETIC',
        content: item?.question?.content ?? '',
        answer: item?.question?.answer ?? '',
        explanation: item?.question?.explanation ?? ''
      },
      userAnswer: item?.userAnswer ?? '',
      timestamp: item?.timestamp ?? new Date().toISOString()
    }));
  }

  // 删除指定索引处的错题，返回是否删除成功
  deleteMistake(index) {
    try {
      if (typeof index !== 'number' || index < 0 || index >= this.mistakes.length) return false;
      this.mistakes.splice(index, 1);
      return true;
    } catch (e) {
      console.error('deleteMistake error:', e);
      return false;
    }
  }

  // 删除指定关卡的所有错题
  deleteMistakesByLevel(levelId) {
    try {
      this.mistakes = this.mistakes.filter(m => m.question?.id !== levelId);
      return true;
    } catch (e) {
      console.error('deleteMistakesByLevel error:', e);
      return false;
    }
  }
}
