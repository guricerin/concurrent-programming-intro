// 最適化抑制読み書き用
use std::ptr::{read_volatile, write_volatile};
// メモリバリア用
use std::sync::atomic::{fence, Ordering};
use std::thread;

const NUM_THREADS: usize = 8;
const NUM_LOOP: usize = 100000;

/// マクロにしている理由
/// 1. unsafeの記述が冗長
/// 2. 関数のままだと、コンパイラによる最適化が行われる可能性がある
macro_rules! read_mem {
    ($addr: expr) => {
        unsafe { read_volatile($addr) }
    };
}

macro_rules! write_mem {
    ($addr: expr, $val: expr) => {
        unsafe { write_volatile($addr, $val) }
    };
}

/// パン屋のアルゴリズム
struct BakeryLock {
    /// true: チケット取得処理中
    entering: [bool; NUM_THREADS],
    /// チケット番号（整理券）。数値の少ないほうが優先される。
    tickets: [Option<u64>; NUM_THREADS],
}

impl BakeryLock {
    /// idx: スレッド番号
    fn lock(&mut self, idx: usize) -> LockGuard {
        fence(Ordering::SeqCst); // アウトオブオーダーでのメモリ読み書きを防ぐ（メモリバリア）
        write_mem!(&mut self.entering[idx], true); // 取得処理開始
        fence(Ordering::SeqCst);

        // 現在配布されているチケットの最大値を取得
        let mut max = 0;
        for i in 0..NUM_THREADS {
            if let Some(t) = read_mem!(&self.tickets[i]) {
                max = max.max(t);
            }
        }
        // 最大値 + 1 をスレッドidxのチケット番号とする
        let ticket = max + 1;
        write_mem!(&mut self.tickets[idx], Some(ticket));

        fence(Ordering::SeqCst); // アウトオブオーダーでのメモリ読み書きを防ぐ（メモリバリア）
        write_mem!(&mut self.entering[idx], false); // 取得処理終了
        fence(Ordering::SeqCst);

        // 待機処理。自分より若いチケット番号をもつスレッドがいる場合に待機する。
        for i in 0..NUM_THREADS {
            if i == idx {
                continue;
            }

            // スレッドiがチケット取得処理中なら待機
            while read_mem!(&self.entering[i]) {}

            loop {
                match read_mem!(&self.tickets[i]) {
                    Some(t) => {
                        // スレッドiのチケット番号より自分の番号のほうが若いか、
                        // チケット番号が同じかつ、自分のほうがスレッド番号が若い場合に待機終了
                        if ticket < t || (ticket == t && idx < i) {
                            break;
                        }
                    }
                    None => {
                        // スレッドiが処理中でない場合は待機終了
                        break;
                    }
                }
            }
        }

        fence(Ordering::SeqCst);
        LockGuard { idx: idx }
    }
}

/// ロック管理用
struct LockGuard {
    /// ロック獲得中のスレッド番号
    idx: usize,
}

impl Drop for LockGuard {
    fn drop(&mut self) {
        fence(Ordering::SeqCst);
        write_mem!(&mut LOCK.tickets[self.idx], None);
    }
}

static mut LOCK: BakeryLock = BakeryLock {
    entering: [false; NUM_THREADS],
    tickets: [None; NUM_THREADS],
};

static mut COUNT: u64 = 0;

fn main() {
    // スレッド生成
    let mut v = Vec::new();
    for i in 0..NUM_THREADS {
        let th = thread::spawn(move || {
            // NUM_LOOPだけループし、COUNTをインクリメント
            for _ in 0..NUM_LOOP {
                let _lock = unsafe { LOCK.lock(i) };
                unsafe {
                    let c = read_volatile(&COUNT);
                    write_volatile(&mut COUNT, c + 1);
                }
            }
        });
        v.push(th);
    }

    for th in v {
        th.join().unwrap();
    }

    println!(
        "COUNT = {} (expected = {})",
        unsafe { COUNT },
        NUM_LOOP * NUM_THREADS
    );
}
