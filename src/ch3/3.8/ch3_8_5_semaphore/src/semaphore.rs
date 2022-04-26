use std::sync::{Condvar, Mutex};

/// セマフォ用の型
pub struct Semaphore {
    /// 中身の値は、クリティカルセクションを実行中のプロセス数
    mutex: Mutex<isize>,
    cond: Condvar,
    /// ロック獲得可能なプロセスの最大数
    max: isize,
}

impl Semaphore {
    pub fn new(max: isize) -> Self {
        Self {
            mutex: Mutex::new(0),
            cond: Condvar::new(),
            max,
        }
    }

    pub fn wait(&self) {
        let mut cnt = self.mutex.lock().unwrap();
        // カウンタが最大値以上なら待機
        while *cnt >= self.max {
            cnt = self.cond.wait(cnt).unwrap();
        }
        // カウンタの変更はミューテックスでロック獲得中に行われるため、排他的に実行されることが保証される
        *cnt += 1; // カウンタをインクリメントしてからクリティカルセクションに移行
    }

    pub fn post(&self) {
        let mut cnt = self.mutex.lock().unwrap();
        // カウンタの変更はミューテックスでロック獲得中に行われるため、排他的に実行されることが保証される
        *cnt -= 1;
        if *cnt <= self.max {
            // 待機中のスレッドに通知
            self.cond.notify_one();
        }
    }
}
