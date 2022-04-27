use std::sync::{Arc, RwLock};
use std::thread;

fn main() {
    let val = Arc::new(RwLock::new(true));

    let t = thread::spawn(move || {
        let _flag = val.read().unwrap(); // Readロック獲得。変数のスコープが外れるまでロックは解放されない。
        *val.write().unwrap() = false; // Writeロック獲得。デッドロック発生。
        println!("deadlock");
    });

    t.join().unwrap();
}
