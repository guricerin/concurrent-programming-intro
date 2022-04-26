use crate::semaphore::Semaphore;
use std::collections::LinkedList;
use std::sync::{Arc, Condvar, Mutex};

/// 送信端
#[derive(Clone)]
pub struct Sender<T> {
    /// 有限性を実現するセマフォ
    sem: Arc<Semaphore>,
    /// キュー
    buf: Arc<Mutex<LinkedList<T>>>,
    /// 読み込み側の条件変数
    cond: Arc<Condvar>,
}

impl<T> Sender<T>
where
    T: Send,
{
    pub fn send(&self, data: T) {
        self.sem.wait(); // キューの最大値に到達したら待機
        let mut buf = self.buf.lock().unwrap();
        buf.push_back(data); // エンキュー
        self.cond.notify_one(); // 読み込み側へ通知
    }
}

// 受信端
pub struct Receier<T> {
    /// 有限性を実現するセマフォ
    sem: Arc<Semaphore>,
    /// キュー
    buf: Arc<Mutex<LinkedList<T>>>,
    /// 読み込み側の条件変数
    cond: Arc<Condvar>,
}

impl<T> Receier<T> {
    pub fn recv(&self) -> T {
        let mut buf = self.buf.lock().unwrap();
        loop {
            if let Some(data) = buf.pop_front() {
                self.sem.post(); // セマフォのカウンタを1減らす
                return data;
            }
            // キューが空の場合は待機
            buf = self.cond.wait(buf).unwrap();
        }
    }
}

pub fn channel<T>(max: isize) -> (Sender<T>, Receier<T>) {
    assert!(max > 0);
    let sem = Arc::new(Semaphore::new(max));
    let buf = Arc::new(Mutex::new(LinkedList::new()));
    let cond = Arc::new(Condvar::new());
    let tx = Sender {
        sem: sem.clone(),
        buf: buf.clone(),
        cond: cond.clone(),
    };
    let rx = Receier {
        sem: sem,
        buf: buf,
        cond: cond,
    };
    (tx, rx)
}
