use std::sync::{Arc, RwLock};
use std::thread;

fn main() {
    let val = Arc::new(RwLock::new(true));

    let t = thread::spawn(move || {
        let flag = *val.read().unwrap(); // Readロックを獲得して、値を取り出した後に即座にReadロックを解放
        if flag {
            *val.write().unwrap() = false; // Writeロックを獲得。デッドロックは発生しない。
            println!("flag is true");
        }
    });

    t.join().unwrap();
}
