use std::sync::{Arc, RwLock};
use std::thread;

fn main() {
    let val = Arc::new(RwLock::new(true));

    let t = thread::spawn(move || {
        let _ = val.read().unwrap(); // Readロック獲得。変数は_なので即座にロック解放。
        *val.write().unwrap() = false; // Writeロック獲得。デッドロックは発生しない。
        println!("not deadlock");
    });

    t.join().unwrap();
}
