use std::sync::{Arc, Mutex};
use std::thread;

fn some_func(lock: Arc<Mutex<u64>>) {
    loop {
        // ロックしないとMutex型の中の値は参照不可。
        let mut val = lock.lock().unwrap();
        *val += 1;
        println!("{}", *val);
    } // MutexGuard型がスコープを抜けると、自動的にロックが解放される。
}

fn main() {
    // Arc : スレッドセーフな参照カウンタ型のスマートポインタ
    let lock0 = Arc::new(Mutex::new(0));
    // 参照カウンタがインクリメントされるのみで、中身はクローンされない。
    let lock1 = lock0.clone();

    // スレッド生成
    // クロージャ内変数へ所有権を移動
    let th0 = thread::spawn(move || {
        some_func(lock0);
    });
    let th1 = thread::spawn(move || {
        some_func(lock1);
    });

    // 待ち合わせ
    th0.join().unwrap();
    th1.join().unwrap();
}
